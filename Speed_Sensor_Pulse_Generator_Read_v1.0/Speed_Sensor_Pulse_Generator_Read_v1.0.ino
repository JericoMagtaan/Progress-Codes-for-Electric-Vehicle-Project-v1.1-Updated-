/*******************************************************************************
- Arduino Program for Measuring from the Pulse Generator
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 29/04/20
*******************************************************************************/
/*
 * The purpose of this code is to read the pulse signal value for the Pulse Generator
 * code that will be used for the Speed Sensor code.
 
 * Receiving Arduino and Pulse Generator Arduino connections as follows:
- Pin D3 of Receive/Read Arduino to output Pin D4 of the Pulse Generator Arduino
- 4.7kOhm protective resistor between digital pin connection of both Arduinos
- GND pin of both Arduinos to common ground
*******************************************************************************/




/*************** Declare pins that will be used on the Arduino ***************/
const int InputSignalPin = 3;   // Digital pin 3 on receiving Arduino as input.

/************ Declare variables that will be used on the Arduino *************/
int SignalValue;                // Variable storing received signal value at digital pin 3.




/*********************** Void setup allows to run once ***********************/
void setup()
{

  pinMode(InputSignalPin, INPUT);   // Set digital pin 3 as an input pin.
                                    // This pin will be used to receive the output signal from the transmitting Arduino.

  // Begin serial communication:
  Serial.begin(9600);
 
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  SignalValue = digitalRead(InputSignalPin);  // Read received digital value from input signal pin 5.
  Serial.println(SignalValue);                // Print received value.
  delay(1000);      // 1 second delay.
  
}
/**************************** End main loop***********************************/
