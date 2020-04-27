/*******************************************************************************
- SD Card Data Logging Programme (Version Includes: All electric vehicle variables)
- Version 1.1
- Modified by Jerico Magtaan
- Last Edited on 27/04/20
*******************************************************************************/
/*
 * Four DS18B20 sensors are used to measure temperature for the BMS temperature,
 * The wiring for each sensor are connected as followed:
- Pin 1 (GND) of DS18B20 sensors are connected to common ground
- Pin 2 (DQ) of DS18B20 sensors can be connected to any digital pin of Arduino
- Pin 3 (Vdd) of DS18B20 sensors can be connected to the 5V pin of the Arduino
- 4.7k pull-up resistor connected between DQ and Vdd of DS18B20 sensors

 * Arduino Uno mounted with an Ethernet Shield 2 for the SD card feature using SPI:
- Pin D4 of Arduino as CS of SD card
- Pin D11 of Arduino for SPI SI pin
- Pin D12 of Arduino for SPI SO pin
- Pin D13 of Arduino for SPI SCK pin
- GND pin of Arduino to common ground
*******************************************************************************/




/************* Import all needed libraries needed by the Arduino **************/
#include <OneWire.h>            // Library used for OneWire device communication.
#include <DallasTemperature.h>  // Library used for DS18B20 temperature sensors.
#include <SPI.h>                // Library used for SPI communication for the SD card.
#include <SD.h>                 // Library used for the SD card.


/*************** Declare pins that will be used on the Arduino ***************/
#define ONE_WIRE_BUS_PIN 2    // Digital pin 2 used for temperature measurements using one-wire bus protocol.
const int chipSelect = 4;     // Digital pin 4 as chip select (CS) pin for SPI communication for the SD card.


/********************* Temperature sensor configurations *********************/
OneWire oneWire(ONE_WIRE_BUS_PIN);    // OneWire instance to communicate with OneWire device (DS18B20 sensors).
DallasTemperature sensors(&oneWire);  // DallasTemperature library setup to work with OneWire devices and library.

// Each DS18B20 has its own unique 64-bit address which was obtained from a previous code.
// Sensor addresses must be configured using "DeviceAddress" and storing it into a variable i.e. Sensor01, etc...
DeviceAddress Sensor01 = { 0x28, 0x50, 0x87, 0x3B, 0x0A, 0x00, 0x00, 0x1C }; 
DeviceAddress Sensor02 = { 0x28, 0x6E, 0x75, 0x3B, 0x0A, 0x00, 0x00, 0xC7 };
DeviceAddress Sensor03 = { 0x28, 0x22, 0x70, 0x3B, 0x0A, 0x00, 0x00, 0x1D };
DeviceAddress Sensor04 = { 0x28, 0xF7, 0x87, 0x3B, 0x0A, 0x00, 0x00, 0xC5 };


/*************************** SD card configurations **************************/
File myFile;
char filename[] = "EV_Data.CSV";       // Create Excel file with filename "EV_Data".

// Variables for all EV signals that will be stored in the SD card:
float tempC, Temp01, Temp02, Temp03, Temp04;  // BMS Individual Sensor Variables.
float BMSTemp, BMSVoltage, BMSCurrent; // BMS Temp., BMS Current and BMS Voltage Variables.
float MotorVoltageFL, MotorCurrentFL;  // Front Left Current Transducer Current, Voltage.
float MotorVoltageFR, MotorCurrentFR;  // Front Right Current Transducer Current, Voltage.
float TSPower;      // Tractive System Total Power.
float CarSpeed;     // Average CarSpeed of the EV (From Sensors).




/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  // Begin serial communication:
  Serial.begin(9600);
  Serial.println("Starting up EV Data Logging System");
  Serial.println("-------------------------------------");
  
  // Always make pin 10 an output even when not using:
  // pinMode(10, OUTPUT); // Might need this, but this code works without setting it as an output.

  // SD card configuration using SPI Chip Select on pin 4:
  if (!SD.begin(chipSelect)) 
  {
    delay (1000);
    return;
  }
  
  // We delete any file currently on the SD card with the EV_Data.CSV name to prevent multiple file copies:
  if (SD.exists("EV_Data.CSV")) 
  {
    SD.remove("EV_Data.CSV");
  } 

  // Setup the file by filling out the first row of the Excel spreadsheet and filling out column titles accordingly - Starts from left to right:
  myFile = SD.open("EV_Data.CSV", FILE_WRITE);
  myFile.println("Sensor01(degC),Sensor02(degC),Sensor03(degC),Sensor04(degC),BMS Temperature(degC),BMS Voltage(V),BMS Current(A),FL Motor Voltage(V),FL Motor Current(A),FR Motor Voltage(V),FR Motor Current(A),TS Power(kW)");      
  myFile.close();

  
  // DS18B20 sensor resolution can be configured using "setResolution", we are using four sensors at
  // 10 bit resolution giving us a total 187.5x4=750ms total conversion time.
  // It is best to set the resolution based on number of sensors we plan to use for faster converison time:
  // 9 bit resolution, 93.75 ms conversion time
  // 10 bit resolution, 187.5 ms conversion time
  // 11 bit resolution, 375 ms conversion time
  // 12 bit resolution, 750 ms conversion time
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
  
  /********************* BMS Temperature Data Log *********************/
  sensors.requestTemperatures();  // Call for DS18B20 temp. sensor measurements.
  myFile = SD.open("EV_Data.CSV", FILE_WRITE);
  delay(500);   // 0.5 second delay
  
  // Sensor01 temperature data logging:
  Temp01 = sensors.getTempC(Sensor01);
  myFile.print(Temp01);   // Store Sensor01 temperature into Excel file.
  myFile.print(",");
  
  // Sensor02 temperature data logging:
  Temp02 = sensors.getTempC(Sensor02);
  myFile.print(Temp02);   // Store Sensor02 temperature into Excel file.
  myFile.print(",");
  
  // Sensor03 temperature data logging:
  Temp03 = sensors.getTempC(Sensor03);
  myFile.print(Temp03);   // Store Sensor03 temperature into Excel file.
  myFile.print(",");  
  
  // Sensor04 temperature data logging:
  Temp04 = sensors.getTempC(Sensor04);
  myFile.print(Temp04);   // Store Sensor04 temperature into Excel file.
  myFile.print(",");  
  
  // BMS average temperature data logging:
  BMSTemp = (Temp01 + Temp02 + Temp03 + Temp04)/4;   
  myFile.print(BMSTemp);  // Store averaged BMS temperature into Excel file.
  myFile.print(",");

  /*********************** BMS Voltage Data Log ***********************/
  BMSVoltage++;           // BMS voltage dummy variable increment.
  delay (100);            // 0.1 second delay.
  if (BMSVoltage>100)     // If BMS voltage reaches max range of 100V, reset back to 0V.
  {
    BMSVoltage = 0;
  }
  myFile.print(BMSVoltage);   // Store dummy BMS voltage into Excel file.
  myFile.print(",");


  /*********************** BMS Current Data Log ***********************/
  BMSCurrent++;            // BMS current dummy variable increment.
  delay (100);             // 0.1 second delay.
  if (BMSCurrent>650)      // If BMS current reaches max range of 650A, reset back to 0A.
  {
    BMSCurrent = 0;
  }
  myFile.print(BMSCurrent);   // Store dummy BMS current into Excel file.
  myFile.print(",");


  /***************** Front Left Motor Voltage Data Log *****************/
  MotorVoltageFL++;         // Front left motor voltage dummy variable increment.
  delay (100);              // 0.1 second delay.
  if (MotorVoltageFL>140)   // If front left motor voltage reaches max range of 140V, reset back to 0V.
  {
    MotorVoltageFL = 0;
  }
  myFile.print(MotorVoltageFL);   // Store dummy front left motor voltage into Excel file.
  myFile.print(",");


  /***************** Front Left Motor Current Data Log *****************/
  MotorCurrentFL++;         // Front left motor current dummy variable increment.
  delay (100);              // 0.1 second delay.
  if (MotorCurrentFL>300)   // If front left motor current reaches max range of 300A, reset back to 0A.
  {
    MotorCurrentFL = 0;
  }
  myFile.print(MotorCurrentFL);   // Store dummy front left motor current into Excel file.
  myFile.print(",");


  /***************** Front Right Motor Voltage Data Log *****************/
  MotorVoltageFR++;         // Front right motor voltage dummy variable increment.
  delay (100);              // 0.1 second delay.
  if (MotorVoltageFR>140)   // If front right motor voltage reaches max range of 140V, reset back to 0V.
  {
    MotorVoltageFR = 0;
  }
  myFile.print(MotorVoltageFR);   // Store dummy front right motor voltage into Excel file.
  myFile.print(",");


  /***************** Front Right Motor Current Data Log *****************/
  MotorCurrentFR++;        // Front right motor current dummy variable increment.
  delay (100);             // 0.1 second delay.
  if (MotorCurrentFR>300)  // If front right motor current reaches max range of 300A, reset back to 0A.
  {
    MotorCurrentFR = 0;
  }
  myFile.print(MotorCurrentFR);   // Store dummy front right motor current into Excel file.
  myFile.print(",");


  /***************** Tractive System Power Data Log ******************/
  TSPower++;              // Tractive system power dummy variable increment.
  delay (100);            // 0.1 second delay.
  if (MotorCurrentFR>80)  // If tractive system power reaches max range of 80kW, reset back to 0kW.
  {
    TSPower = 0;
  }
  myFile.print(TSPower);  // Store dummy tractive system power into Excel file.
  myFile.print(",");

  
  /********************* Average Car Speed Data Log *********************/
  CarSpeed++;           // EV CarSpeed dummy variable increment.
  delay (100);          // 0.1 second delay.
  if (CarSpeed>600)     // If CarSpeed reaches max range of 600RPM, reset back to 0RPM.
  {
    CarSpeed = 0;
  }
  myFile.print(CarSpeed);   // Store dummy car speed into Excel file.
  myFile.print(",");
  myFile.println();
  myFile.close();   // Close the file after all measurements have been logged.
  Serial.println(" Data Logged ");  // Let user know that data has been logged.
  
}
/**************************** End main loop ***********************************/




/*************** Event DS18B20 sensors temperature measurement ****************/
void printTemperature(DeviceAddress deviceAddress)
{
  
// getTempC is used to measure temperature in degrees Celsius, "deviceAddress" is
// needed to measure temperature from the different sensors with different addresses.
tempC = sensors.getTempC(deviceAddress);
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
/************** End event DS18B20 sensors temperature measurement ***************/
