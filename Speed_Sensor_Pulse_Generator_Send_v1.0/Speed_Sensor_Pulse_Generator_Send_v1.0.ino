/*******************************************************************************
- Arduino Program to act as a Pulse Generator for Speed (RPM) Program
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 26/04/20
*******************************************************************************/
/*
 * The purpose of this code is to send pulses to the Speed Sensor code to
 * measure RPM. This code will be used to test if the Speed Sensor code is
 * working and is able to detect pulses and calculate the RPM.
 
 * Speed sensor and Arduino will be connected as follows:
- Pin D4 of Pulse Generating Arduino to input Pin D3 of the Measuring Arduino
- 4.7kOhm protective resistor between digital pin connection of both Arduinos
- GND pin of both Arduinos to common ground
*******************************************************************************/




/*************** Declare pins that will be used on the Arduino ***************/
const int OutputSignalPin = 4;  // Digital pin 4 used for the output signal pulse.

/************ Declare variables that will be used on the Arduino *************/
int State = 0;                  // Variable storing current state in the loop.
                                // This variable is needed to produce the signal pulse.




/*********************** Void setup allows to run once ***********************/
void setup()
{

  pinMode(OutputSignalPin, OUTPUT);   // Set digital pin 4 as an output pin.
                                      // this pin will be used to send the signal pulse to the receiving Arduino.
 
 // Begin serial communication:
 Serial.begin(9600);
 
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{

  // State change to alternate between low and high at digital pin 4 to simulate a square-wave pulse output:
  while(1)
  {
    if(State == 0)
    {
      digitalWrite(OutputSignalPin, LOW);
      State = 1;
    }
    
    else
    {
      digitalWrite(OutputSignalPin, HIGH);
      State = 0;
    }
    
    delay(30);    // Delay is used to set the pulse duration per cycle.
                  // Since we are using this code for the Speed Sensor code, we adjust the delay to
                  // change the duration of period between pulses since it dictates how fast the pin alternates between Low and high,
                  // where shorter delay would simulate shorter pulse duration, meaning faster RPM,
                  // whereas longer delay would simulate longer pulse duration, meaning slower RPM.                                      
  }
  
}
/**************************** End main loop***********************************/
