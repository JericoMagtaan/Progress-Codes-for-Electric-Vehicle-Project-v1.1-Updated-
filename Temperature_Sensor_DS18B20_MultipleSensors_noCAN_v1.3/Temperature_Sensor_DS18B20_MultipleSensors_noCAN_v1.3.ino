/*******************************************************************************
- Temperature Measurements with Multiple DS18B20 One-Wire Sensor (4x)
  (This version includes implementation of CAN bus)
- Version 1.3
- Modified by Jerico Magtaan
- Last Edited on 27/04/20
*******************************************************************************/
/*
 * Four DS18B20 sensors are used to measure temperature
 * The wiring for each sensor are connected as followed:
- Pin 1 (GND) of DS18B20 sensors are connected to common ground
- Pin 2 (DQ) of DS18B20 sensors can be connected to any digital pin of Arduino
- Pin 3 (Vdd) of DS18B20 sensors can be connected to the 5V pin of the Arduino
- 4.7k pull-up resistor connected between DQ and Vdd of DS18B20 sensors

 * Arduino connected to MCP2515 module via SPI and 5V for CAN bus:
- Pin D10 of Arduino to CS of MCP2515 module
- Pin D11 of Arduino to SI of MCP2515 module
- Pin D12 of Arduino to SO of MCP2515 module
- Pin D13 of Arduino to SCK of MCP2515 module
- GND pin of Arduino to common ground
 * MCP2515 module connections:
- Pin Vcc of MCP2515 can be connected 5V from Arduino
*******************************************************************************/




/************* Import all needed libraries needed by the Arduino **************/
#include <OneWire.h>            // Library used for OneWire device communication.
#include <DallasTemperature.h>  // Library used for DS18B20 temperature sensors.

/*************** Declare pins that will be used on the Arduino ***************/
#define ONE_WIRE_BUS_PIN 2  // Digital pin 2 used for temperature measurements using one-wire bus protocol.

/********************* Temperature sensor configurations *********************/
OneWire oneWire(ONE_WIRE_BUS_PIN);    // OneWire instance to communicate with OneWire device (DS18B20 sensors).
DallasTemperature sensors(&oneWire);  // DallasTemperature library setup to work with OneWire devices and library.

// Each DS18B20 has its own unique 64-bit address which was obtained from a previous code.
// Sensor addresses must be configured using "DeviceAddress" and storing it into a variable i.e. Sensor01, etc...
DeviceAddress Sensor01 = { 0x28, 0x50, 0x87, 0x3B, 0x0A, 0x00, 0x00, 0x1C }; 
DeviceAddress Sensor02 = { 0x28, 0x6E, 0x75, 0x3B, 0x0A, 0x00, 0x00, 0xC7 };
DeviceAddress Sensor03 = { 0x28, 0x22, 0x70, 0x3B, 0x0A, 0x00, 0x00, 0x1D };
DeviceAddress Sensor04 = { 0x28, 0xF7, 0x87, 0x3B, 0x0A, 0x00, 0x00, 0xC5 };




/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  // Begin serial communication:
  Serial.begin(9600);

  // DS18B20 sensor resolution can be configured using "setResolution", we are using four sensors at
  // 10 bit resolution giving us a total 187.5x4=750ms total conversion time.
  // It is best to set the resolution based on number of sensors we plan to use for faster converison time:
  // 9 bit resolution, 93.75 ms conversion time
  // 10 bit resolution, 187.5 ms conversion time
  // 11 bit resolution, 375 ms conversion time
  // 12 bit resolution, 750 ms conversion time
  Serial.print("Starting up Temperature Control Library ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.println("-------------------------------------");
  sensors.begin();
  sensors.setResolution(Sensor01, 10);
  sensors.setResolution(Sensor02, 10);
  sensors.setResolution(Sensor03, 10);
  sensors.setResolution(Sensor04, 10);
  
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  // Count the number of sensors connected on the one-wire bus at digital pin 2:
  // This can be done using the "getDeviceCount", this is a good way to check that all sensors
  // are connected and there are no faulty connections in the one-wire bus.
  delay(1000);      // 1 second delay.
  Serial.println();
  Serial.print("Number of connected Devices on bus: ");  
  Serial.println(sensors.getDeviceCount());   // Display number of connected devices on bus.
  Serial.print("Measuring temperatures from all connected sensors... ");  
  Serial.println();
  sensors.requestTemperatures();     // Start temperature measurements.

  Serial.print("Sensor01 temperature:   ");
  printTemperature(Sensor01);       // Print Sensor01 measured temperature to serial monitor.
  Serial.println();

  Serial.print("Sensor02 temperature:   ");
  printTemperature(Sensor02);       // Print Sensor02 measured temperature to serial monitor.
  Serial.println();
 
  Serial.print("Sensor03 temperature:   ");
  printTemperature(Sensor03);       // Print Sensor03 measured temperature to serial monitor.
  Serial.println();
   
  Serial.print("Sensor04 temperature:   ");
  printTemperature(Sensor04);       // Print Sensor04 measured temperature to serial monitor.
  Serial.println();   

  // Take average readings of all temperature sensors to get the averaged BMS temperature:
  float Temp1 = sensors.getTempC(Sensor01);
  float Temp2 = sensors.getTempC(Sensor02);
  float Temp3 = sensors.getTempC(Sensor03);
  float Temp4 = sensors.getTempC(Sensor04);
  float BMSTemp = (Temp1 + Temp2 + Temp3 + Temp4)/4; 
  Serial.print(" Average temperature:   ");
  Serial.print(BMSTemp);       // Print average measured temperature (BMS) to serial monitor.
  Serial.print("  (degC)");
  Serial.println();   
  delay(1000);    // 1 second delay.
  
}
/**************************** End main loop***********************************/




/*************** Event DS18B20 sensors temperature measurement ****************/
void printTemperature(DeviceAddress deviceAddress)
{

// getTempC is used to measure temperature in degrees Celsius, "deviceAddress" is
// needed to measure temperature from the different sensors with different addresses.
float tempC = sensors.getTempC(deviceAddress);

   if (tempC == -127.00) 
   {
   Serial.print("Error getting temperature  ");
   } 
   else
   {
   Serial.print(tempC);
   Serial.print("  (degC)");
   } 
   
}
/************* End event DS18B20 sensors temperature measurement ***************/
