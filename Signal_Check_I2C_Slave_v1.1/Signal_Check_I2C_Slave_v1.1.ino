/*******************************************************************************
- Arduino to Arduino I2C Serial Communication Test (I2C Slave Code)
  (This version includes a time condition, Master sends ReceivedValue value of 0 to
  1 every 0.5 seconds over the I2C bus, if either 0 or 1 is held for more
  than four seconds, that means the Master Arduino is not operating)
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
int ReceivedValue = 0;                                  // Variable that will be constantly received from the Master Arduino.
unsigned long ReceivedValue1LastTimeConditionWasFalse;  // Condition when received variable ReceivedValue is 1.
unsigned long ReceivedValue0LastTimeConditionWasFalse;  // Condition when received variable ReceivedValue is 0.




/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  // Begin serial communication:
  Serial.begin(9600);
  Serial.println("| I2C Slave Initiated |");

  
  // Set up I2C communication/I2C bus:
  Wire.begin(5);                // Configure this Arduino with a slave address of 5.
  Wire.onReceive(receiveEvent); // Carry out event for receiving signals on the I2C bus.
  Serial.println("------------------------------------------");
  
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  Serial.println(ReceivedValue);  // print the received variable stored in ReceivedValue.
  delay(500);                     // 0.5 second delay.


  // Count the duration that the value in variable ReceivedValue not equal to 1:
  if (ReceivedValue != 1)
  {
    ReceivedValue1LastTimeConditionWasFalse = millis();
  }
  // Condition to show that SignalValue equal to 1 for more than four seconds:
  // When the Master Arduino is disconnected, it continuously receives the last sent value instead.
  // The principle of a signal check is used with a timing condition, where the value 4000(4s) can be
  // adjusted to suit the system of the user, it can be configured to detect faster by reducing the time "4000".
  if (millis() - ReceivedValue1LastTimeConditionWasFalse >= 4000)
  {
    Serial.println("Master Arduino not operating, turn off system (1)");
    // Code for Low Voltage System to use relay to turn off system here...
  }

  // Count the duration that the value in variable ReceivedValue not equal to 0:  
  if (ReceivedValue != 0)
  {
    ReceivedValue0LastTimeConditionWasFalse = millis();
  }
  // Condition to show that SignalValue equal to 0 for more than four seconds.
  // When the Master Arduino is disconnected, it continuously receives the last sent value instead.
  // The principle of a signal check is used with a timing condition, where the value 4000(4s) can be
  // adjusted to suit the system of the user, it can be configured to detect faster by reducing the time "4000".
  if (millis() - ReceivedValue0LastTimeConditionWasFalse >= 4000)
  {
    Serial.println("Master Arduino not operating, turn off system (0)");
    // Code for Low Voltage System to use relay to turn off system here...
  }
  
}
/**************************** End main loop***********************************/


/**************** Event for receive from Master via I2C bus ******************/
void receiveEvent(int bytes)
{
  ReceivedValue = Wire.read();    // Store received bytes into variable ReceivedValue.
}
/*****************************************************************************/
