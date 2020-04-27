/*******************************************************************************
- Temperature Measurements with a Single DS18B20 One-Wire Sensor
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 22/04/20
*******************************************************************************/
/*
 * One DS18B20 sensors is used to measure temperature
 * The wiring for the sensor is connected as followed:
- Pin 1 (GND) of DS18B20 sensors are connected to GND pin of Arduino
- Pin 2 (DQ) of DS18B20 sensors can be connected to any digital pin of Arduino
- Pin 3 (Vdd) of DS18B20 sensors can be connected to either 5V or 3.3V pin of Arduino
- 4.7k pull-up resistor connected between DQ and Vdd of DS18B20 sensors
*******************************************************************************/




/************* Import all needed libraries needed by the Arduino **************/
#include <OneWire.h>            // Library used for OneWire device communication
#include <DallasTemperature.h>  // Library used for DS18B20 temperature sensors


/*************** Declare pins that will be used on the Arduino ***************/
#define ONE_WIRE_BUS_PIN 2  // Digital pin 2 used for temperature measurements using one-wire bus protocol


/********************* Temperature sensor configurations *********************/
OneWire oneWire(ONE_WIRE_BUS_PIN);    // OneWire instance to communicate with OneWire device (DS18B20 sensors)
DallasTemperature sensors(&oneWire);  // DallasTemperature library setup to work with OneWire devices and library




/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  // Begin serial communication
  Serial.begin(9600);
  Serial.println("Temperature Measurements with DS18B20");
  Serial.println("Temperature Sensor: DS18B20");
  delay(1000);  // 1 second delay

  // Set up temperature sensor to start measuring using the DallasTemperature library
  sensors.begin();

}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop() 
{

  // Start temperature measurements using the "requestTemperatures" function
  Serial.println("Requesting temperature...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  // The temperature is  printed to the screen by using "ByIndex", this allows us to
  // display measurements all the sensors connected on the onewire bus at digital pin 2.
  Serial.print("Device 1 (index 0) = ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" Degrees C");
  delay(1000);  // 1 second delay

}
/**************************** End main loop***********************************/
