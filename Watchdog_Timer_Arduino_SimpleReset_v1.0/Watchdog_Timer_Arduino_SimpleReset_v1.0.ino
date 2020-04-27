/*******************************************************************************
- Watchdog Timer for Arduino with Simple Reset
- Version 1.0
- Modified by Jerico Magtaan
- Last Edited on 22/04/20
*******************************************************************************/
/*
 This code can be used on any Arduino, its function is to simply
 show how watchdog timer can be configured where it allows for 
 different watchdog timer timeout between 15ms to 8s:
 WDTO_15MS    15
 WDTO_30MS    30
 WDTO_60MS    60
 WDTO_120MS   120
 WDTO_250MS   250
 WDTO_500MS   500
 WDTO_1S      1000
 WDTO_2S      2000
 WDTO_4S      4000
 WDTO_8S      8000
*******************************************************************************/




/************* Import all needed libraries needed by the Arduino **************/
#include <avr/wdt.h>    // Library used to implement built-in watchdog timer on the Arduino.


/*************** Declare pins that will be used on the Arduino ***************/
const int LEDPin = 13;  // Digital pin 13 is the default LED pin on the Arduino UNO.


/*********************** Void setup allows to run once ***********************/
void setup()
{
  
  // Begin serial communication:
  Serial.begin(9600);
  Serial.println("------------------------------------------");
  Serial.println("Watchdog Timer Example Setup Initializing");

  
  pinMode(LEDPin, OUTPUT);  // Set LEDPin digital pin 13 as an output.

  
  wdt_disable();            // Disable the watchdog timer before setting up watchdog timer and watchdog enable.
  delay(2000);              // 2 second delay is needed to let bootloader check that a new code is uploaded to the
                            // Arduino, this prevents the Arduino from going into an infinite loop this way.
                            
  wdt_enable(WDTO_2S);      // Enable the watchdog with a timeout of 2 seconds, this can be changed to the different
                            // time setting specified at the start of this code. The principle of the watchdog timer
                            // relies on this function as this code can be used for any programme, all that you need is
                            // to know how long the main loop runs so you know what timer value to set for your system.
  Serial.println("Watchdog Timer Enabled (With 2s timeout)");
  
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  Serial.println(">>Entering Main Loop<<");
  
  // The LED is simply used to turn it on and off for a certain period of time:
  // Any code can go in here as long as the main loop includes the "wdt_reset()" function.
  for(int i = 0; i<20; i++)
  {
    digitalWrite(LEDPin, HIGH);
    delay(100);
    digitalWrite(LEDPin, LOW);
    delay(100);
    wdt_reset();    // The watchdog timer needs to be reset after each time the loop runs.
                    // This lets the Arduino know that it is not stuck in an infinite loop.
  }
  Serial.println(">>Main Loop Finished<<");
  

  while(1);     // Infinite loop can be placed to see that the watchdog timer is working.
                // When the infinite loop is present, the loop never gets to reset the watchdog timer.
                // Depending on the time you have set with the "wdt_enable" function, the Arduino will be reset.
                // For this system we have set the timeout to be 2s, thus if the timer has not been reset for more than
                // two seconds, the entire Arduino will be reset to exit out of the infinite loop.

                // This while loop can be commented out again and you will see that the Arduino will operate as normal.
                
}
/**************************** End main loop***********************************/
