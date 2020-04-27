/*******************************************************************************
// Receiver Code for CAN bus
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
unsigned char len = 0;    // Variable storing length of data.
unsigned char buf[8];     // Variable storing data buffer from CAN message.
unsigned int canID;       // Variable storing CAN ID of transmitting device in the CAN bus.




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
        delay(100);
        goto START_INIT;
    }
    
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
    // We can use the CAN library to receive the data from a transmitting device.
    // For this example, we only have one other transmitting Arduino connected to the CAN bus.
    // This program will store the address and CAN data and print it to the serial monitor, as many
    // transmitting Arduinos can be connected and you will see that the Arduino with this program
    // will be able to print the different CAN addresses and the data they are sending accordingly.
    if(CAN_MSGAVAIL == CAN.checkReceive())    // Check if data is coming
    {
        CAN.readMsgBuf(&len, buf);            // Read data, len: data length, buf: data buffer
        canID = CAN.getCanId();               // Get the ID of incoming message

        Serial.print("ID is: ");
        Serial.print(canID, HEX);             // Print the ID in its standard form (HEX)

        Serial.print("    Length is: ");
        Serial.println(len);
        
        for(int i = 0; i<len; i++)            // Loop for incoming data to print on screen
        {
            Serial.write(buf[i]);             // Print characters received
        }
        Serial.println("\n\t*********************************************\n");
    }
    
}
/**************************** End main loop***********************************/
