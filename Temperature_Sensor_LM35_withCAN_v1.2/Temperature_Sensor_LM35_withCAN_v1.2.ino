/*******************************************************************************
- Temperature Measurements with Multiple (4) Analog LM35 Sensors
- Version 1.2
- Programme by Jerico Magtaan
- Last Edited on 27/04/20
*******************************************************************************/
/*
 * Four analog LM35 sensors are used to measure temperature, this version
 * contains code that sends temp. values over CAN bus.
 
 * The wiring for the LM35 sensors are connected as followed:
- Pin 1 (Vs) of LM35 sensors can be supplied 4 to 20V (5V from Arduino)
- Pin 2 (Vout) of LM35 sensors can be connected to any analog pin of Arduino
- Pin 3 (GND) of LM35 sensors can be connected to a common ground

 * Arduino connected to MCP2515 module via SPI and 5V for CAN bus:
- Pin D10 of Arduino to CS of MCP2515 module
- Pin D11 of Arduino to SI of MCP2515 module
- Pin D12 of Arduino to SO of MCP2515 module
- Pin D13 of Arduino to SCK of MCP2515 module
- GND pin of Arduino to common ground

 * MCP2515 module connections:
- Pin Vcc of MCP2515 can be supplied 5V from Arduino
- GND pin of MCP2515 to common ground
*******************************************************************************/




/************* Import all needed libraries needed by the Arduino **************/
#include <SPI.h>            // Library used for SPI communication for the CAN module.
#include <mcp_can.h>        // Library used for CAN bus communication.

/*************** Declare pins that will be used on the Arduino ***************/
MCP_CAN CAN(10);            // Digital pin 10 used as default chip select (CS) pin for SPI communication.
int tempPin1 = A0;          // Analog pin 0 connected to LM35 temperature sensor.
int tempPin2 = A1;          // Analog pin 1 connected to LM35 temperature sensor.
int tempPin3 = A2;          // Analog pin 2 connected to LM35 temperature sensor.
int tempPin4 = A3;          // Analog pin 3 connected to LM35 temperature sensor.

/************ Declare variables that will be used on the Arduino *************/
float tempC1;       // Variable storing analog measurement from sensor at pin A0.
float tempC2;       // Variable storing analog measurement from sensor at pin A1.
float tempC3;       // Variable storing analog measurement from sensor at pin A2.
float tempC4;       // Variable storing analog measurement from sensor at pin A3.
float tempdegC1;    // Variable storing digital measurement from sensor at pin A0.
float tempdegC2;    // Variable storing digital measurement from sensor at pin A1.
float tempdegC3;    // Variable storing digital measurement from sensor at pin A2.
float tempdegC4;    // Variable storing digital measurement from sensor at pin A3.
float BMSTemp;      // Variable storing averaged digital measurement from all sensors.




/*********************** Void setup allows to run once ***********************/
void setup ()
{
  
  // Begin serial communication:
  Serial.begin(9600);
  Serial.print("| LM35 Sensor Measurements Started |");
  
  // Begin CAN bus communication using the CAN bus shield at 100KBPS:
  START_INIT:

    if(CAN_OK == CAN.begin(CAN_100KBPS))
    {
        Serial.println("CAN BUS Shield init ok!");
        Serial.println("-------------------------------------");
    }
    
    else
    {
        Serial.println("CAN BUS Shield failed to start");
        Serial.println("Initialize CAN BUS Shield again");
        Serial.println("-------------------------------------");
        goto START_INIT;        
        delay(1000);
    }
    
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  tempC1 = analogRead(tempPin1);                // Read from A0 (analog value).
  tempdegC1 = (5.0 * tempC1 * 100.0) / 1024.0;  // A0 temp. conversion to digital.
  tempC2 = analogRead(tempPin2);                // Read from A1 (analog value).
  tempdegC2 = (5.0 * tempC2 * 100.0) / 1024.0;  // A1 temp. conversion to digital.
  tempC3 = analogRead(tempPin3);                // Read from A2 (analog value).
  tempdegC3 = (5.0 * tempC3 * 100.0) / 1024.0;  // A2 temp. conversion to digital.
  tempC4 = analogRead(tempPin4);                // Read from A3 (analog value).
  tempdegC4 = (5.0 * tempC4 * 100.0) / 1024.0;  // A3 temp. conversion to digital.

  BMSTemp = (tempdegC1 + tempdegC2 + tempdegC3 + tempdegC4)/4;  // Averaged temp. calculation stored in BMSTemp.

  // Print averaged temperature to serial monitor:
  Serial.print("Average temperature:   ");
  Serial.print(BMSTemp);
  Serial.print("  (degC)");
  Serial.println(); 

  // Use the CAN bus library to send the value stored in BMSTemp over CAN bus with an address
  // of 0x2, this address can be changed i.e. 0x3, 0xD1, etc...
  CAN.sendMsgBuf(0x2, 0, 2, (const byte *) &BMSTemp);
  Serial.println("Temp. CAN ID Sent");
  Serial.println("-------------------------------------");
  delay(1000);      // 1 second delay.
  
}
/**************************** End main loop***********************************/
