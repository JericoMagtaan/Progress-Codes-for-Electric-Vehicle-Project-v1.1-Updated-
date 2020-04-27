/*******************************************************************************
- LVCSS Code for Brake Light and 5V Relay
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 24/04/20
*******************************************************************************/
/*
 * This code is used in the LVCSS Arduino, its main purpose is to control the
 * brake light and 5V relay currently installed in the LVCSS system.
 
 * For future purposes, the signal check code for the electric vehicle between
 * this Arduino and the Motor Control Arduino, to ensure that if the Arduino
 * controlling the motor ever goes off, the LVCSS Arduino can detect this and
 * trip the 5V relay to shut the system down for safety reasons.
*******************************************************************************/




/*************** Declare pins that will be used on the Arduino ***************/
const int BrakeLight =  10;      // Digital pin 10 used for the brake light.
const int SafetyRelay =  9;      // Digital pin 9 used for the 5v safety relay.




/*********************** Void setup allows to run once ***********************/
void setup() 
{
  
  // Begin serial communication:
  Serial.begin(9600);

  
  pinMode(BrakeLight, OUTPUT);  // Set digital pin 10 Brakelight pin as output.
  pinMode(SafetyRelay, OUTPUT); // Set digital pin 9 Relay pin as output.
  
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop() 
{
  
  // Brake input at analog pin A0 measured to check position of the brake:
  int brake = analogRead(A0);
  if (brake > 10)
  {
    analogWrite(BrakeLight, 0);   // Brake input = 0 means it is in default position.
    Serial.println("test");
  }
  else
  {
    analogWrite(BrakeLight, 255); // Brake input = 255 means it is in switched position.
  }
  analogWrite(SafetyRelay, 255);
  Serial.println(brake);
  delay(250);        // 250 millisecond delay.
  
}
/**************************** End main loop***********************************/
