/*******************************************************************************
- Arduino to Arduino Digital Pin Signal Check (Master Code)
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 22/04/20
*******************************************************************************/
/*
 * The purpose of this code is to investigate a simple digital pin signal
 * check that can be used between the LVCSS and Lowry's Arduino.
 * Two Arduinos are connected to each other with the following pins:
- Pin D6 (Output) of Master Arduino to Pin D5 (Input) of Slave Arduino
- 4.7kOhm protectiveresistor in digital pin connection of both Arduinos
- GND pin of both Arduinos connected to a common ground
*******************************************************************************/




/*************** Declare pins that will be used on the Arduino ***************/
const int OutputSignalPin = 6;  // Digital pin 6 on Master Arduino as output.





/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  pinMode(OutputSignalPin, OUTPUT);   // Set digital pin 6 as an output pin.
                                      // this pin will be used to transmit signals to the slave/receiving Arduino.

  // Begin serial communication:
  Serial.begin(9600);
  Serial.println("| Master Arduino Setup |");
  Serial.println("-------------------------------------------");
  
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  digitalWrite(OutputSignalPin, HIGH);  // Set digital pin 6 output pin high.
  Serial.println("High");               // Print to serial monitor it is high.
  delay(250);      // 250 millisecond delay.
  
}
/**************************** End main loop***********************************/
