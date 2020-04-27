/*******************************************************************************
- Temperature Measurements with Multiple (4) Analog LM35 Sensors
- Version 1.1
- Programme by Jerico Magtaan
- Last Edited on 27/04/20
*******************************************************************************/
/*
 * Four analog LM35 sensors are used to measure temperature, this version
 * contains code that sends temp. values over CAN bus.
 
 * The wiring for the LM35 sensors and Arduino connection as followed:
- Pin 1 (Vs) of LM35 sensors can be supplied 4 to 20V (5V from Arduino)
- Pin 2 (Vout) of LM35 sensors can be connected to any analog pin of Arduino
- Pin 3 (GND) of LM35 sensors can be connected to a common ground
- GND pin of Arduino to a common ground
*******************************************************************************/




/*************** Declare pins that will be used on the Arduino ***************/
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
  delay(1000);      // 1 second delay.
  
}
/**************************** End main loop***********************************/
