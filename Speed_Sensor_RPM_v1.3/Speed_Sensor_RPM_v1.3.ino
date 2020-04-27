/*******************************************************************************
- Speed Measurement (RPM) with a Digital Speed Sensor using Interrupt
- Version 1.3
- Modified by Jerico Magtaan
- Last Edited on 27/04/20
*******************************************************************************/
/*
 * The purpose of this code is to implement an RPM measurement for the speed sensor
 * that will be implemented on the electric car.
 * For this version, this will be done by measuring the period duration between pulses
 * to calculate the RPM, where the number of readings will vary depending on the RPM, where
 * fewer readings will be taken at lower RPM and more readings at higher RPM for accuracy.
 
 * Speed sensor and Arduino will be connected as follows:
- Pin D3 of Arduino to the output pin of the speed sensor
- Pin 5V of Arduino to Vsupply pin of the speed sensor
- GND pin of speed sensor to common ground
- GND pin of Arduino to common ground
*******************************************************************************/




/************ Declare variables that will be used on the Arduino *************/
              // Calbration variables (adjustable to suit the system)
              
const byte PulsesPerRevolution = 1;  // Set how many pulses there are on each revolution.

// If the period between pulses is too high or pulses have stopped, we would get stuck showing the last value instead of 0.
// As a solution we will set a maximum limit for the period, where if the period is above this value, the RPM will be 0.
// The higher the set value, the longer lag there will be to sense that pulses stopped, but this allows more readings at low RPM.
// Setting a low value is going to allow the detection of stop situations faster, but it will prevent having low RPM readings.
const unsigned long ZeroTimeout = 100000;  // Variable storing period limit, which is used to show that RPM is 0.
                                           // For high response time i.e. Set to 100000 (microseconds).
                                           // For reading very low RPM i.e. Set to 300000 (microseconds).
                                           
// Calibration for smoothing RPM:
const byte numReadings = 2;  // Variable storing number of samples for smoothing. The higher, the more smoothing, but it will react slower to changes.



/******************************************************************************/
              // Fixed variables needed for calculations, etc...

// These variables have big values since it is measured in microseconds, thus we need to add 1000
// so these variables don't start at 0 because if it is at 0, it will be interpreted as a high frequency.
volatile unsigned long PeriodBetweenPulses = ZeroTimeout + 1000;  // Variable storing the period between pulses.
volatile unsigned long PeriodAverage = ZeroTimeout + 1000;        // Variable storing averaged period between pulses from multiple pulses.
                                                            
volatile unsigned long LastTimeWeMeasured;  // Variable storing the last time we measured a pulse so we can calculate the period.  
                                                          
unsigned long FrequencyRaw;    // Variable storing the calculated frequency, based on the period. This has a lot of extra decimals without the decimal point.
unsigned long FrequencyReal;   // Variable storing the calculated frequency without decimals.
unsigned long RPM;             // Variable storing the RPM without any processing.
unsigned int PulseCounter = 1; // Variable storing counts of the amount of pulse readings we took so we can average multiple pulses before calculating the period.
unsigned long PeriodSum;       // Variable storing the summation of all the periods to do the average.

unsigned int AmountOfReadings = 1;  // Variable storing the number of readings we will be taking depending on how low or high the calculated RPM is.
                                    // AmountOfReadings=1 would be the minimum reading, which gives us a reading per pulse.
                                    // Low AmountofReadings will be responsive but less accurate at high speeds, while high AmountofReadings will give
                                    // accurate readings at high speeds but readings at low speeds would be too far apart due to longer periods between pulses.
                                    // This variable will be automatically adjusted based on how low/high the RPM.
                                    
unsigned int ZeroDebouncingExtra;   // Variable storing the extra value added to the ZeroTimeout to debounce it.
                                    // The ZeroTimeout needs debouncing so when the value is close to the threshold it doesn't
                                    // jump from 0 to the value. This extra value changes the threshold a little when we show a 0.

// Variables that can change mid-cycle:
// Variables unused by the interrupt are needed because we are going to use maths and functions for these variables, thus we store the value from
// "LastTimeWeMeasured" and micros()" in these variables, so we can use them for calculations in the main loop:
unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;  // Variable storing the last time we measured a pulse in that cycle.                                                     
unsigned long CurrentMicros = micros();                   // Variable storing the micros in that cycle.
                                 
// Variables for RPM measurement for data smoothing:
unsigned long readings[numReadings];  // Variable storing the input.
unsigned long readIndex;              // The index of the current reading.
unsigned long total;                  // The running total.
unsigned long average;                // The RPM value after applying the smoothing.




/*********************** Void setup allows to run once ***********************/
void setup()
{

  // Begin serial communication:
  Serial.begin(9600);

  // Digital Pin 3 Set As An Interrupt for the Arduino Uno:
  attachInterrupt(digitalPinToInterrupt(3), Pulse_Event, RISING);

  delay(1000);  // 1 second delay.
                // This delay allows the micros() to be high enough for the first few cycles
                // which prevents us from getting negative readings.
                
}
/********************************* End setup *********************************/




/******************* Void loop allows to run continuously ********************/
void loop()
{
  
  // Two variables that can change in the middle of the cycle:
  // Since the variables "LastTimeWeMeasured" and "micros()" are used in the interrupt, we need to store the values in them
  // in another variable so we can use their values in the main loop, if we were to use these variables as is, glitches would occur.
  LastTimeCycleMeasure = LastTimeWeMeasured;  // Store the LastTimeWeMeasured value in another variable called "LastTimeCycleMeasure".
  CurrentMicros = micros();                   // Store the micros() value in another variable called "CurrentMicros".


  // CurrentMicros should always be higher than LastTimeWeMeasured, to ensure this is the case, compare both
  // variables and if CurrentMicros is lower than LastTimeCycleMeasure, let LastTimeCycleMeasure = CurrentMicros.
  // This is needed because later this variable is needed to see if pulses have stopped.
  if(CurrentMicros < LastTimeCycleMeasure)
  {
    LastTimeCycleMeasure = CurrentMicros;
  }


  // Calculate the frequency using the averaged period between pulses using equation 1/T:
  // (Note that 1000000 microseconds = 1 second, we will get rid of the extra zeroes from this calculation when calculating "FrequencyReal")
  FrequencyRaw = 10000000000 / PeriodAverage;


  // Detect if pulses stopped or frequency is too low, so we can dusplay that Frequency = 0.
  // If the pulses are too far apart and has reached the set ZeroTimeout limit, make Frequency = 0:
  if(PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra)
  {
    FrequencyRaw = 0;            // Let the frequency = 0.
    ZeroDebouncingExtra = 2000;  // Change the threshold a little so it doesn't bounce.
  }
  else
  {
    ZeroDebouncingExtra = 0;     // Reset the threshold to the normal value so it doesn't bounce.
  }

  FrequencyReal = FrequencyRaw / 10000;   // Divide by 10000 to calculate frequency without decimal places.

  // Calculate the RPM:
  RPM = FrequencyRaw / PulsesPerRevolution * 60;  // Frequency divided by number of pulses per revolution multiply by 60s to get minutes.
  RPM = RPM / 10000;                              // Divide by 10000 to calculate RPM without decimal places.

  // Print information on the serial monitor:
  Serial.print("Period: ");
  Serial.print(PeriodBetweenPulses);    // Print the period between pulses to serial monitor (microseconds).
  Serial.print("\tReadings: ");
  Serial.print(AmountOfReadings);       // Print the number of readings to serial monitor (unitless).
  Serial.print("\tFrequency: ");
  Serial.print(FrequencyReal);          // Print the calculated frequency to serial monitor (Hertz).
  Serial.print("\tRPM: ");
  Serial.print(RPM);                    // Print the calculated speed to serial monitor (RPM).
  Serial.println();
  delay(500);                           // 0.5s delay.
                                        // We can get rid of this delay, but for testing purposes, I placed this delay so I can investigate
                                        // the displayed period, number of readings, frequency and RPM to see if it works as expected.
  
}
/**************************** End main loop***********************************/




/********************* Event interrupt for pulse event ***********************/
void Pulse_Event()
{

  PeriodBetweenPulses = micros() - LastTimeWeMeasured;  // Current "micros" minus the old "micros" when the last pulse happens.
                                                        // This will result with the period (microseconds) between both pulses.
                                                        // The way is made, the overflow of the "micros" is not going to cause any issue.
  LastTimeWeMeasured = micros();  // Stores the current micros so the next time we have a pulse we would have something to compare with.


  // If the counter PulseCounter reaches the set limit of AmountofReadings:
  if(PulseCounter >= AmountOfReadings)
  {
    PeriodAverage = PeriodSum / AmountOfReadings;  // Calculate the average period by dividing the sum of all readings by no. of readings.
    PulseCounter = 1;                 // Reset the counter to start over. The reset value is 1 because its the minimum setting allowed (1 reading).
    PeriodSum = PeriodBetweenPulses;  // Reset PeriodSum to start a new averaging calculation.


    // Change the amount of readings depending on the period between pulses.
    // To be very responsive, ideally we should read every pulse. The problem is that at higher speeds the period gets
    // too low decreasing the accuracy. To get more accurate readings at higher speeds we should get multiple pulses and
    // average the period, but if we do that at lower speeds then we would have readings too far apart (laggy or sluggish).
    // To have both advantages at different speeds, we will change the amount of readings depending on the period between pulses using map functions:
    int RemappedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);
    // This map function is used as follows: "map(Value#1, Value#2, Value#3, Value#4, Value#5);", where:
    // Value#1 is what are we going to remap. In this case is the PeriodBetweenPulses.
    // Value#2 is the period value when we are going to have only 1 reading. The higher it is, the lower RPM has to be to reach 1 reading.
    // Value#3 is the period value when we are going to have 10 readings. The higher it is, the lower RPM has to be to reach 10 readings.
    // Value#4 is the number of reading at lower RPM (higher period between pulses) as specified in Value#2.
    // Value#5 is the number of reading at higher RPM (lower period between pulses) as specified in Value#3.
    RemappedAmountOfReadings = constrain(RemappedAmountOfReadings, 1, 10);  // Constrain the value so it doesn't go below or above the limits.
    AmountOfReadings = RemappedAmountOfReadings;  // Set amount of readings as the remaped value.
  }
  else
  {
    PulseCounter++;   // Increase the counter for amount of readings by 1.
    PeriodSum = PeriodSum + PeriodBetweenPulses;  // Add the periods so later we can average.
  }

}
/******************* End event interrupt for pulse event *********************/
