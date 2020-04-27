// 7-February-2020 - Program for intermediate presentation
// Developer: Lowry Maher
// Function: This program controls 2 independent Nova 15kW 60V motors -
// and varies their speed output based on Throttle Input and Steering Input

#include <Arduino.h> //Include additional Arduino Libraries
#include <SoftwareSerial.h> //Include software serial library
#include <TM1638.h> //Include the TM1638 library

SoftwareSerial mySerial1(2, 3); //RX, TX, Create a softwareSerial port on pins 2 and 3
SoftwareSerial mySerial2(4, 5); //RX, TX, Create a softwareSerial port on pins 4 and 5

TM1638 module(8, 9, 7); //Define a module on data pin 8, clock pin 9 and strobe pin 7

#define LEFT 0 //Determines left side of LED display
#define RIGHT 1 //Determines left side of LED display

byte displayDigits[] = {63, 6, 91, 79, 102, 109, 124, 7, 127, 103 }; //Array of the numbers from 0 to 9
byte values[] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //An array for the values to be displayed - all zeroes means show nothing.
int theDigits[] = { 0, 0, 0, 0 }; //The digits which will make up a number to be displayed

//Variables for left motor calculation:

int LeftTensCharacter; //Number 1 to 10 at the start of the throttle string

//Variables used to calculate the "TensCharacter"
int LeftFirstTensVariable;
int LeftSecondTensVariable;
int LeftThirdTensVariable;
int LeftFourthTensVariable;
int LeftFifthTensVariable;

//Variables used to calculate the "DecimalCharacters"
float LeftDecimalValue;
float LeftFirstDecimalVariable;
int LeftSecondDecimalVariable;
float LeftThirdDecimalVariable;
float LeftThirdTensVariableDuplicate;
int LeftFourthDecimalVariable;

String LeftOnesCharacters = ""; //String storing the "+" characters for the ones multiplier
String LeftDecimalCharacters = ""; //String storing the "g" characters for the decimal multiplier
String LeftLessThanTenCharacters = ""; //String storing the "-" characters for the ones multiplier when <10%
String LeftFullThrottleString = "m"; //String to store "m" for full throttle, as per the controller manual
String LeftThrottleString = ""; //String that stores the final concated string to be sent to the motor
float LeftOutputPercent;  //Variable to store throttle %

//Variables for right motor calculation:

int RightTensCharacter; //Number 1 to 10 at the start of the throttle string

//Variables used to calculate the "TensCharacter"
int RightFirstTensVariable;
int RightSecondTensVariable;
int RightThirdTensVariable;
int RightFourthTensVariable;
int RightFifthTensVariable;

//Variables used to calculate the "DecimalCharacters"
float RightDecimalValue;
float RightFirstDecimalVariable;
int RightSecondDecimalVariable;
float RightThirdDecimalVariable;
float RightThirdTensVariableDuplicate;
int RightFourthDecimalVariable;

String RightOnesCharacters = ""; //String storing the "+" characters for the ones multiplier
String RightDecimalCharacters = ""; //String storing the "g" characters for the decimal multiplier
String RightLessThanTenCharacters = ""; //String storing the "-" characters for the ones multiplier when <10%
String RightFullThrottleString = "m"; //String to store "m" for full throttle, as per the controller manual
String RightThrottleString = ""; //String that stores the final concated string to be sent to the motor
float RightOutputPercent;  //Variable to store throttle %.

//Variables for inputs and electronic differential
float SteeringInput;
float SteeringPercent;
float ThrottleInput;
float Throttle50Value;
float OutputPercent;
float RightMotorPercent;
float LeftMotorPercent;
float ExcessPercent;
float TurningPercent;
float RightSteeringPercent;
float LeftSteeringPercent;
float WheelSteeringAngle;
float RightWheelSteeringAngle;
float LeftWheelSteeringAngle;
float AverageWheelSteeringAngle;
float WheelTurningRadius;
float LeftWheelTurningRadius;
float RightWheelTurningRadius;
float AverageWheelTurningRadius;
float RadiusDifferencePercent;
float ExcessRadiusDifferencePercent;

float WheelBase = 1.54; //Distance from rear to front wheels is 1.54 metres.
float TrackWidth = 1.15; //Distance from left to right wheels is 1.15 metre.

float NegativePercent;
float ExcessNegativePercent;
float OverHundredVariable;

void setup()
{
  Serial.begin(115200); //Baud rate between computer and Arduino

  mySerial1.begin(115200); //Baud rate between Arduino and Controller
  mySerial1.println("s"); //Initialise the controller to accept serial command strings

  mySerial2.begin(115200); //Baud rate between Arduino and Controller
  mySerial2.println("s"); //Initialise the controller to accept serial command strings

  module.setDisplay(values); //Start with the digital display blank
  module.setupDisplay(true, 7); //Set the display intensity. (1 to 7)
}

void loop() //Runs program continuously
{
  while (millis() < 3000) { //While in the first 3 seconds, send "s" to controller to set to Serial Mode
    mySerial1.println("s"); //Initialise the left controller to accept serial command strings
    mySerial2.println("s"); //Initialise the right controller to accept serial command strings
  }

  ExcessRadiusDifferencePercent = RadiusDifferencePercent;
  ExcessNegativePercent = NegativePercent;

  ThrottleInput = analogRead(A0); //Value from potentiometer placed into "ThrottleInput"

  OutputPercent = (((ThrottleInput - 28.00) / 400.00) * 100.00) / 2.00; //Analog Input value converted to %

  Serial.println(OutputPercent); //Display throttle percent on screen

  SteeringInput = analogRead(A1); //Value from potentiometer placed into "Steering Input"

  if (SteeringInput > 550) { //If approaching full lock left range, set 63 as the zero position
    SteeringInput = 63;
  }
  else if (SteeringInput > 515 && SteeringInput < 550) { //If approaching full lock right, set to 515 as the maximum position
    SteeringInput = 515;
  }

  SteeringPercent = ((SteeringInput - 63) / 452) * 100; //Analog Input value converted to %

  Serial.println(SteeringPercent); //Display steering percent on screen

  TurningPercent = (SteeringPercent - 50) * 2;

  if (OutputPercent < 1) { //If the throttle signal is less than 1%, both motors should be off.
    LeftThrottleString = "0"; //Turn off left motor
    RightThrottleString = "0"; //Turn off right motor
  }

  else if (OutputPercent > 1 || OutputPercent < 20) { //If the throttle signal is between 1% and 20%
    if (SteeringPercent > 45 && SteeringPercent < 55) { //If driving straight, with a 10% deadband incorporated
      LeftMotorPercent = (OutputPercent); //Left motor speed = throttle value
      RightMotorPercent = (OutputPercent); //Right motor speed = throttle value
    }

    //If turning right
    else if (SteeringPercent > 55) { //Steering angle greater than 55%
      RightWheelSteeringAngle = ((TurningPercent / 100) * 28); //Steering angle of right wheel is between 0-28 degrees
      LeftWheelSteeringAngle = ((TurningPercent / 100) * 23); //Steering angle of left wheel is between 0-23 degrees
      AverageWheelSteeringAngle = ((LeftWheelSteeringAngle + RightWheelSteeringAngle) / 2); //Average steering angle
      AverageWheelTurningRadius = (WheelBase / (tan(AverageWheelSteeringAngle * (3.14159 / 180)))); //Turn radius = (w/tan(Beta)) and convert rads to degrees
      LeftWheelTurningRadius = (AverageWheelTurningRadius + (TrackWidth / 2)) ; //Outer radius = average radius + half the width of the car
      RightWheelTurningRadius = (AverageWheelTurningRadius - (TrackWidth / 2)) ; //Inner radius = average radius - half the width of the car
      RadiusDifferencePercent = (((LeftWheelTurningRadius - RightWheelTurningRadius) / LeftWheelTurningRadius) * 100); //Outer - inner / outer
      ExcessPercent = (OutputPercent + (RadiusDifferencePercent / 2) - 100); //Store the amount that goes past 100%
      NegativePercent = ((OutputPercent - (RadiusDifferencePercent / 2)) * (-1)); //Store the amount that goes below 0%
      OverHundredVariable = (OutputPercent + ExcessPercent); //Value past 100% e.g. 112%

      //The following executes when the steering is in full-lock or close to full-lock.
      if (OverHundredVariable > 100) { //If the % to one of the motors is larger than 100%
        LeftMotorPercent = 100.00; //Make the outer motor 100%
        RightMotorPercent = (OutputPercent - ExcessPercent - (ExcessRadiusDifferencePercent / 2)); //Subtract the excess differential % from the inner wheel
      }
      else if ((OutputPercent - (ExcessRadiusDifferencePercent / 2)) < 0) { //If signal to motor is less than 0%
        RightMotorPercent = 0; //Inner motor set to 0%
        LeftMotorPercent = OutputPercent; //Outer motor runs as fast as driver inputs
      }

      //Else if the output % is smaller than the % differential and the car is not at or near full-lock:
      else if (OutputPercent < ExcessRadiusDifferencePercent && ((OutputPercent - (ExcessRadiusDifferencePercent / 2)) > 0)) {
        RightMotorPercent = 0; //Inner motor set to 0%
        LeftMotorPercent = OutputPercent; //Outer motor set to output signal
      }
      else { //Else we are in a straight line: (neither signal is greater than 100 or less than 0)
        LeftMotorPercent = (OutputPercent); //Outer speed = output signal
        RightMotorPercent = (OutputPercent - (ExcessRadiusDifferencePercent)); //Inner speed = throttle - differential
      }
    }

    //If turning left - calculations are repeated with motors reversed
    else if (SteeringPercent < 45) { //Steering less greater than 45%
      RightWheelSteeringAngle = (((TurningPercent / 100) * 23) * (-1)); //Steering angle of right wheel is between 0-23 degrees
      LeftWheelSteeringAngle = (((TurningPercent / 100) * 28) * (-1)); //Steering angle of left wheel is between 0-28 degrees
      AverageWheelSteeringAngle = ((LeftWheelSteeringAngle + RightWheelSteeringAngle) / 2); //Average steering angle
      AverageWheelTurningRadius = (WheelBase / (tan(AverageWheelSteeringAngle * (3.14159 / 180)))); //Turn radius = (w/tan(Beta)) and convert rads to degrees
      LeftWheelTurningRadius = (AverageWheelTurningRadius - (TrackWidth / 2)) ; //Inner radius = average radius + half the width of the car
      RightWheelTurningRadius = (AverageWheelTurningRadius + (TrackWidth / 2)) ; //Outer radius = average radius - half the width of the car
      RadiusDifferencePercent = (((RightWheelTurningRadius - LeftWheelTurningRadius) / RightWheelTurningRadius) * 100); //Outer - inner / outer
      ExcessPercent = (OutputPercent + (RadiusDifferencePercent / 2) - 100); //Store the amount that goes past 100%
      NegativePercent = ((OutputPercent - (RadiusDifferencePercent / 2)) * (-1)); //Store the amount that goes below 0%
      OverHundredVariable = (OutputPercent + ExcessPercent); //Value past 100% e.g. 112%

      //The following executes when the steering is in full-lock or close to full-lock
      if (OverHundredVariable > 100) { //If the % to one of the motors is larger than 100%
        RightMotorPercent = 100.00; //Make the outer motor 100%
        LeftMotorPercent = ((OutputPercent - ExcessPercent) - (ExcessRadiusDifferencePercent / 2)); //Subtract the excess differential % from the inner wheel
      }
      else if ((OutputPercent - (ExcessRadiusDifferencePercent / 2)) < 0) { //If signal to motor is less than 0%
        LeftMotorPercent = 0; //Inner motor set to 0%
        RightMotorPercent = OutputPercent; //Outer motor runs as fast as driver inputs
      }

      //Else if the output % is smaller than the % differential and the car is not at or near full-lock:
      else if (OutputPercent < ExcessRadiusDifferencePercent && ((OutputPercent - (ExcessRadiusDifferencePercent / 2)) > 0)) {
        LeftMotorPercent = 0; //Inner motor set to 0%
        RightMotorPercent = OutputPercent; //Outer motor set to output signal.
      }
      else {//Else we are in a straight line: (neither signal is greater than 100 or less than 0)
        RightMotorPercent = (OutputPercent); //Outer speed = output signal
        LeftMotorPercent = (OutputPercent - (ExcessRadiusDifferencePercent)); //Inner speed = throttle - differential
      }
    }

    //Convert left throttle value into string to be sent to controller
    LeftFirstTensVariable = LeftMotorPercent; //Store the 0-100 value read from the potentiometer e.g. 58.67%
    LeftTensCharacter = (LeftFirstTensVariable / 10); //Store the tens multiplier e.g. 5

    LeftSecondTensVariable = (LeftTensCharacter * 10); //Store the previous value times ten e.g. 50
    LeftThirdTensVariable = (LeftFirstTensVariable - LeftSecondTensVariable); //Store the remaining units e.g 58-50 = 8

    LeftThirdTensVariableDuplicate = LeftThirdTensVariable; //Store the same value in a second variable for seperate calculations simultaneously

    //DecimalValue:
    LeftFirstDecimalVariable = LeftMotorPercent; //Store the 0-100 value read from the potentiometer e.g. 58.67%
    LeftSecondDecimalVariable = LeftMotorPercent; //Store the throttle value in an INT data type e.g. 58
    LeftThirdDecimalVariable = (LeftMotorPercent - LeftSecondDecimalVariable); //Store just the decimal value e.g. 58.67 - 58 = .67
    LeftFourthDecimalVariable = (LeftThirdDecimalVariable * 10); //Store the decimal multiplier e.g. .67 * 10 = 6

    LeftOnesCharacters = ""; //Clear the string
    while (LeftThirdTensVariable >= 1) { //While the units value is >= 1
      LeftThirdTensVariable -= 1; //Decrease the units value by 1
      LeftOnesCharacters += "+"; //Increase the OnesCharacters string by "+"
    }

    LeftDecimalCharacters = ""; //Clear the string
    while (LeftFourthDecimalVariable >= 1) { //While the decimal units value is >= 1
      LeftFourthDecimalVariable -= 1; //Decrease the units value by 1
      LeftDecimalCharacters += "g"; //Increase the DecimalCharacters string by "+"
    }

    LeftLessThanTenCharacters = ""; //Clear the string
    while (LeftThirdTensVariableDuplicate < 10) { //While the units value is >= 1
      LeftThirdTensVariableDuplicate += 1; //increase the units value by 1
      LeftLessThanTenCharacters += "-"; //Increase the DecimalCharacters string by "+"
    }

    if (LeftMotorPercent < 10.00 && 0 < LeftMotorPercent) {
      LeftThrottleString = (1 + LeftLessThanTenCharacters + "f"); //e.g. 3.42% = "1-------ggggf"
    }
    else {
      LeftThrottleString = (LeftTensCharacter + LeftOnesCharacters + "f"); //e.g. 23.42% = "2+++ggggf"
    }

    //Convert right throttle value into string to be sent to controller, motor moving in reverse direction
    RightFirstTensVariable = RightMotorPercent; //Store the 0-100 value read from the potentiometer e.g. 58.67%
    RightTensCharacter = (RightFirstTensVariable / 10); //Store the tens multiplier e.g. 5

    RightSecondTensVariable = (RightTensCharacter * 10); //Store the previous value times ten e.g. 50
    RightThirdTensVariable = (RightFirstTensVariable - RightSecondTensVariable); //Store the remaining units e.g 58-50 = 8

    RightThirdTensVariableDuplicate = RightThirdTensVariable; // Store the same value in a second variable for seperate calculations simultaneously

    //DecimalValue:
    RightFirstDecimalVariable = RightMotorPercent; //Store the 0-100 value read from the potentiometer e.g. 58.67%
    RightSecondDecimalVariable = RightMotorPercent; //Store the throttle value in an INT data type e.g. 58
    RightThirdDecimalVariable = (RightMotorPercent - RightSecondDecimalVariable); //Store just the decimal value e.g. 58.67 - 58 = .67
    RightFourthDecimalVariable = (RightThirdDecimalVariable * 10); //Store the decimal multiplier e.g. .67 * 10 = 6

    RightOnesCharacters = ""; //Clear the string
    while (RightThirdTensVariable >= 1) { //While the units value is >= 1
      RightThirdTensVariable -= 1; //Decrease the units value by 1
      RightOnesCharacters += "+"; //Increase the OnesCharacters string by "+"
    }

    RightDecimalCharacters = ""; //Clear the string
    while (RightFourthDecimalVariable >= 1) { //While the decimal units value is >= 1
      RightFourthDecimalVariable -= 1; //Decrease the units value by 1
      RightDecimalCharacters += "g"; //Increase the DecimalCharacters string by "+"
    }

    RightLessThanTenCharacters = ""; //Clear the string
    while (RightThirdTensVariableDuplicate < 10) { //While the units value is >= 1
      RightThirdTensVariableDuplicate += 1; //increase the units value by 1
      RightLessThanTenCharacters += "-"; //Increase the DecimalCharacters string by "+"
    }

    if (RightMotorPercent < 10.00 && 0 < RightMotorPercent) {
      RightThrottleString = (1 + RightLessThanTenCharacters + "r"); //e.g. 3.42% = "1-------ggggr"
    }
    else {
      RightThrottleString = (RightTensCharacter + RightOnesCharacters + "r"); //e.g. 23.42% = "2+++ggggr"
    }
  }

  else if (OutputPercent > 20) { //If the throttle signal is greater than 20%, set both motors to 20%.
    LeftThrottleString = "2f"; //Left motor speed = "20%"
    RightThrottleString = "2r"; //Right motor speed = "20%"
  }

  mySerial1.println(LeftThrottleString); //Send the left signal % as strings over this port
  mySerial2.println(RightThrottleString); //Send the right signal % as strings over this port

  Serial.println(LeftThrottleString); //Display left throttle string on screen
  Serial.println(RightThrottleString); //Display right throttle string on screen

  float theNumberToDisplay = LeftMotorPercent; //Display the % sent to the motor
  int positionToDisplayIt = LEFT; //Display Number on left or right
  int numberOfDecimalPlacesToShow = 2; //How many decimal places to show - must be 1 or 2
  displayNumber(theNumberToDisplay, positionToDisplayIt, numberOfDecimalPlacesToShow);

  float theNumberToDisplay2 = RightMotorPercent;
  int positionToDisplayIt2 = RIGHT;
  int numberOfDecimalPlacesToShow2 = 2;
  displayNumber2(theNumberToDisplay2, positionToDisplayIt2, numberOfDecimalPlacesToShow2);

  delay(50); //Delay for program stability, reduce delay to increase program iterations and response time
}

//Display numbers on left of digital display
void displayNumber(float numberToSplit, int whichSide, int numOfDPs) {

  if (numOfDPs > 2 or numOfDPs < 1) {
    numOfDPs = 1;
  }

  numberToSplit = (int)(100 * numberToSplit);

  if (numberToSplit > 9999) {
    numberToSplit = 100;
  }
  else {
    theDigits[0] = (int)(numberToSplit / 1000);
    theDigits[1] = (int)((numberToSplit - (1000 * theDigits[0])) / 100);
    theDigits[2] = (int)((numberToSplit - (1000 * theDigits[0]) - (100 * theDigits[1])) / 10);
    theDigits[3] = (int)(numberToSplit - (1000 * theDigits[0]) - (100 * theDigits[1]) - (10 * theDigits[2]));
  }

  int dispDig[4]; //Declaring an array with 4 elements
  if (theDigits[0] == 0) {
    dispDig[0] = 0; //Hide a leading zero if there is one
  }
  else {
    dispDig[0] = displayDigits[theDigits[0]];
    dispDig[1] = displayDigits[theDigits[1]] + 128; //Apend the dp onto the second digit
    dispDig[2] = displayDigits[theDigits[2]];
    dispDig[3] = displayDigits[theDigits[3]];
  }

  if (numOfDPs == 1) {
    dispDig[3] = 0;
  }

  if (numberToSplit / 100 < 0 or numberToSplit / 100 > 100) {
    for (int i = 0; i < 4; i++)
      dispDig[i] = 64;
  }

  if (numberToSplit == 100) {
    dispDig[0] = 6;
    dispDig[1] = 63;
    dispDig[2] = 63;
    dispDig[3] = 0;
  }

  int offset = 0; //Left by default

  values[0 + offset] = dispDig[0];
  values[1 + offset] = dispDig[1];
  values[2 + offset] = dispDig[2];
  values[3 + offset] = dispDig[3];

  module.setDisplay(values);
}

//Display numbers on right of digital display
void displayNumber2(float numberToSplit2, int whichSide2, int numOfDPs2) {

  if (numOfDPs2 > 2 or numOfDPs2 < 1) {
    numOfDPs2 = 1;
  }

  numberToSplit2 = (int)(100 * numberToSplit2);

  if (numberToSplit2 > 9999) {
    numberToSplit2 = 100;
  }
  else {
    theDigits[0] = (int)(numberToSplit2 / 1000);
    theDigits[1] = (int)((numberToSplit2 - (1000 * theDigits[0])) / 100);
    theDigits[2] = (int)((numberToSplit2 - (1000 * theDigits[0]) - (100 * theDigits[1])) / 10);
    theDigits[3] = (int)(numberToSplit2 - (1000 * theDigits[0]) - (100 * theDigits[1]) - (10 * theDigits[2]));
  }

  int dispDig[4]; //Declaring an array with 4 elements
  if (theDigits[0] == 0) {
    dispDig[0] = 0; //Hide a leading zero if there is one
  }
  else {
    dispDig[0] = displayDigits[theDigits[0]];
    dispDig[1] = displayDigits[theDigits[1]] + 128; //Apend the dp onto the second digit
    dispDig[2] = displayDigits[theDigits[2]];
    dispDig[3] = displayDigits[theDigits[3]];
  }

  if (numOfDPs2 == 1) {
    dispDig[3] = 0;
  }

  if (numberToSplit2 / 100 < 0 or numberToSplit2 / 100 >= 100) {
    for (int i = 0; i < 4; i++)
      dispDig[i] = 64;
  }

  if (numberToSplit2 == 100) {
    dispDig[0] = 6;
    dispDig[1] = 63;
    dispDig[2] = 63;
    dispDig[3] = 0;
  }

  int offset = 0; //Left by default
  if (whichSide2 == RIGHT) { //Set to right
    offset = 4;
  }

  values[0 + offset] = dispDig[0];
  values[1 + offset] = dispDig[1];
  values[2 + offset] = dispDig[2];
  values[3 + offset] = dispDig[3];

  //Update the display display with new values
  module.setDisplay(values);
}
