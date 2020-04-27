/*******************************************************************************
- Printing Internal Addresses of DS18B20 One-Wire Sensor
- Programme from http://henrysbench.capnfatz.com/henrys-bench/arduino-temperature-measurements/ds18b20-arduino-user-manual-introduction-and-contents/ds18b20-user-manual-part-2-getting-the-device-address/
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 22/04/20
*******************************************************************************/
/*
 * A DS18B20 sensor's 64-bit address can be printed out using this code
 * along with the wiring connections specified below:
- Pin 1 (GND) of DS18B20 sensors are connected to GND pin of Arduino
- Pin 2 (DQ) of DS18B20 sensors can be connected to any digital pin of Arduino
- Pin 3 (Vdd) of DS18B20 sensors can be connected to either 5V or 3.3V pin of Arduino
- 4.7k pull-up resistor connected between DQ and Vdd of DS18B20 sensors
*******************************************************************************/




/************* Import all needed libraries needed by the Arduino **************/
#include <OneWire.h>            // Library used for OneWire device communication.


/*************** Declare pins that will be used on the Arduino ***************/
#define SENSOR_PIN 2            // Digital pin 2 used for temperature sensor.

/********************* Temperature sensor configurations *********************/
OneWire Bus(SENSOR_PIN);        // OneWire instance to set up pin 2 as OneWire bus.




/*********************** Void setup allows to run once ***********************/
void setup()
{
  // Begin serial communication:
  Serial.begin(9600);


  // Check for connected one-wire devices to the Arduino:
  discoverOneWireDevices();
  
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  // Loop not needed for this programme, address printed once to serial monitor.
}
/**************************** End main loop***********************************/




/******************** Event DS18B20 sensor address print *********************/
void discoverOneWireDevices(void) 
{
  
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];   // Number of bytes in address array.

  Serial.print("Getting the device address...\n\r");

  /* Print if a device has been found and print the device address */
  while(Bus.search(addr)) 
  {
    // Read each byte in the address array:
    Serial.print("\n\r\n\r\'One-Wire\' Device has been found with Serial Number:\n\r");
    for( i = 0; i < 8; i++)
    {
      Serial.print("0x");
      if (addr[i] < 16) 
      {
        Serial.print('0');
      }
      
      // Print each byte in the address array in hex format:
      Serial.print(addr[i], HEX);
      if (i < 7) 
      {
        Serial.print(", ");
      }
    }

    
    // Check to make sure that what we read is correct:
    if ( OneWire::crc8( addr, 7) != addr[7]) 
    {
      Serial.print("CRC is not valid!\n\r");
      return;
    }
  }

  
  Serial.println();
  Serial.print("Finished");
  Bus.reset_search();   // Reset search for next sensor to be used.
  return;
}
/***************** End event DS18B20 sensor address print ******************/
