/*******************************************************************************
- Speed Measurement (RPM) with a Digital Speed Sensor using Interrupt
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 26/04/20
*******************************************************************************/
/*
 * The purpose of this code is to implement an RPM measurement for the speed sensor
 * that will be implemented on the electric car.
 * For this version, this will be done by measuring the period duration between pulses
 * to calculate the RPM.
 
 * Speed sensor and Arduino will be connected as follows:
- Pin D3 of Arduino to the output pin of the speed sensor
- Pin 5V of Arduino to Vsupply pin of the speed sensor
- GND pin of speed sensor to common ground
- GND pin of Arduino to common ground
*******************************************************************************/




/************ Declare variables that will be used on the Arduino *************/
const byte PulsesPerRevolution = 1;         // Variable storing no. of pulses in a revolution (number of teeth on geartooth disc).
volatile unsigned long LastTimeWeMeasured;  // Variable storing last time pulse was measured, used to calculate the period.
volatile unsigned long PeriodBetweenPulses; // Variable storing period between pulses in microseconds.
unsigned long RPM;                          // Variable storing averaged/smoothed RPM reading.




/*********************** Void setup allows to run once ***********************/
void setup()
{

 // Begin serial communication:
 Serial.begin(9600);
 
 // Digital Pin 3 Set As An Interrupt for the Arduino Uno:
 attachInterrupt(digitalPinToInterrupt(3), Pulse_Event, RISING);
 delay(1000);     // 1s delay
 
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  // Calculate and display RPM:
  if(PeriodBetweenPulses > 0)
 {
    RPM = 60*(1000000/(PeriodBetweenPulses*PulsesPerRevolution));     // Multiply by 60 for one minute and 1000000 microsecond for one second.
                                                                      // PeriodBetweenPulses multipled by number of pulses/number of teeth in geartooth disc.
   
    delay(500);               // 0.5s delay. 
    Serial.print("RPM: "); 
    Serial.print(RPM);        // Print RPM to serial monitor.
    Serial.println();
  }
  
}
/**************************** End main loop***********************************/




/********************* Event interrupt for pulse event ***********************/
void Pulse_Event()
{
  
   PeriodBetweenPulses = micros() - LastTimeWeMeasured; // Current "micros" minus the old "micros" when the last pulse happens.
                                                        // This will result with the period (microseconds) between both pulses.
   
   LastTimeWeMeasured = micros();  // Stores the current micros so the next time we have a pulse we would have something to compare with.
   
}
/******************* End event interrupt for pulse event *********************/
