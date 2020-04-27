/*******************************************************************************
- Arduino to Arduino Digital Pin Signal Check (Slave Code)
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 26/04/20
*******************************************************************************/
/*
 * The purpose of this code is to investigate a simple digital pin signal
 * check that can be used between the LVCSS and Lowry's Arduino.
 * Two Arduinos are connected to each other with the following pins:
- Pin D6 (Output) of Master Arduino to Pin D5 (Input) of Slave Arduino
- 4.7kOhm protective resistor between digital pin connection of both Arduinos
- GND pin of both Arduinos connected to a common ground
*******************************************************************************/




/*************** Declare pins that will be used on the Arduino ***************/
const int InputSignalPin = 5;   // Digital pin 5 on Slave Arduino as input.

/************ Declare variables that will be used on the Arduino *************/
int SignalValue;                          // Variable storing signal value 1 or 0.
unsigned long LastTimeConditionWasFalse;  // Condition when Signal is 0 (low).




/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  pinMode(InputSignalPin, INPUT);   // Set digital pin 5 as an input pin.
                                    // this pin will be used to receive signals from the master/transmitting Arduino.

  // Begin serial communication:
  Serial.begin(9600);
  Serial.println("| Slave Arduino Setup |");
  Serial.println("-------------------------------------------");
  
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  SignalValue = digitalRead(InputSignalPin);  // Read received digital value from input signal pin 5.
  Serial.println(SignalValue);                // Print received value.
  delay(250);      // 250 millisecond delay.


  // Count the duration that the SignalValue not equal to 0 (low):
  if (SignalValue != 0)
  {
    LastTimeConditionWasFalse = millis();
  }
  
  
  // Condition to show that SignalValue equal to 0 for more than four seconds:
  // When the Master Arduino is disconnected, no signal will be received, thus only sending 0's.
  // The principle of a signal check is used with a timing condition, where the value 4000(4s) can be
  // adjusted to suit the system of the user, it can be configured to detect faster by reducing the time "4000".
  if (millis() - LastTimeConditionWasFalse >= 4000)
  {
    Serial.println("Master Arduino not operating, turn off system");
    // Code for Low Voltage System to use relay to turn off system here...
  }
  
}
/**************************** End main loop***********************************/
