/*******************************************************************************
// Transmitter Code for CAN bus
// Version 1.0
// Programme by Jerico Magtaan
// Last Edited on 24/04/20
*******************************************************************************/
/*
 * Arduino and MCP2515 module connections via SPI and 5V supply pin:
- Pin D10 of Arduino to CS of MCP2515 module
- Pin D11 of Arduino to SI of MCP2515 module
- Pin D12 of Arduino to SO of MCP2515 module
- Pin D13 of Arduino to SCK of MCP2515 module
- Pin 5V of Arduino to VCC of MCP2515 module
- All Arduino and MCP2515 modules connected to a common ground
- 120 Ohm termination resistor at both ends of CAN bus
*******************************************************************************/




/************* Import all needed libraries needed by the Arduino **************/
#include <SPI.h>                // Library used for SPI communication for the CAN module.
#include <mcp_can.h>            // Library used for CAN bus communication.


/*************** Declare pins that will be used on the Arduino ***************/
MCP_CAN CAN(10);            // Digital pin 10 used as default chip select (CS) pin for SPI communication.


/************ Declare variables that will be used on the Arduino *************/
unsigned char DataSend[8] = {'t', 'e', 's', 't', 'i', 'n', 'g', '!'};   // Variable storing DataSend/characters that will be sent over CAN bus.
                                                                        // This is a simple way of testing if the CAN bus works.




/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  // Begin serial communication:
  Serial.begin(9600);

  // Begin CAN bus communication using the CAN bus shield at 100KBPS:
  START_INIT:

    if(CAN_OK == CAN.begin(CAN_100KBPS))
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);   // 0.1 second delay.
        goto START_INIT;
    }
    
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{ 
  
    // Use the CAN bus library to send the value stored in "DataSend" over CAN bus with an address
    // of 0xF1, this address can be changed i.e. 0x3, 0xD1, etc...
    Serial.println("entered communication loop");
    CAN.sendMsgBuf(0xF1, 0, 8, DataSend);
    delay(2000);     // 2 second delay.
                    
}
/**************************** End main loop***********************************/
