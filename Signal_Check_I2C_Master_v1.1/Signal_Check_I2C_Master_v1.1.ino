/*******************************************************************************
- Arduino to Arduino I2C Serial Communication Test (I2C Master Code)
- Version 1.1
- Modified by Jerico Magtaan
- Last Edited on 22/04/20
*******************************************************************************/
/*
 * The purpose of this code is to investigate if I2C can be used as a
 * handshake protocol for signal checking between two Arduinos
 * Two Arduinos are connected to each other using built-in I2C pins:
- Pin A4 (SDA) of Slave Arduino to Pin A4 (SDA) of Master Arduino
- Pin A5 (SCL) of Slave Arduino to Pin A5 (SCL) of Master Arduino
- GND pin of both Arduinos connected to a common ground
*******************************************************************************/




/************* Import all needed libraries needed by the Arduino **************/
#include <Wire.h>     // Library used for I2C communication.

/************ Declare variables that will be used on the Arduino *************/
int TransmittedValue = 0;   // Variable that will be constantly sent to the Slave Arduino.




/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  // Begin serial communication:
  Serial.begin(9600);
  Serial.println("| I2C Master Initiated |");

  // Set up I2C communication/I2C bus:
  Wire.begin();   // An address in () can be placed i.e. 5 is address 5, but since this Arduino is
                  // set up as the Master Arduino, no value is placed in () to configure it as a Master.
  Serial.println("-------------------------------------------");
  
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{

  Wire.beginTransmission(5);      // Transmit to device with address 5 using I2C bus.
  Wire.write(TransmittedValue);   // Send value stored in variable TransmittedValue.
  Wire.endTransmission();         // Stop transmission on the I2C bus.
  Serial.println(">>Variable Transmitted to Address 5<<");

  TransmittedValue++;             // Variable TransmittedValue incremented by 1 each time.
  delay (500);                    // 0.5 second delay.
  if (TransmittedValue > 1)       // If variable TransmittedValue greater than 1, make TransmittedValue = 0.
  {
    TransmittedValue = 0;
  }
  
}
/**************************** End main loop***********************************/
