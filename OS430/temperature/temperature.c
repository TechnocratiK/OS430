/*
//
//  temp.c
//
//  Created by: Andrew Doyle on 29 November 2010
//  Last modified by: Andrew Doyle on 29 November 2010
//
//  Provide an API to read from the temperature sensor located in the ADC12 
//  module of the MSP430
//
*/

#include <msp430x14x.h>
#include "config.h"
#include "temperature/temperature.h"
#include "scheduler/scheduler.h"
#include "mutex/mutex.h"
#include "counter/counter.h"

void initializeTemperature(void) {
	// Bits 15-12:	Sample and hold time = 0010 (16)	
	// Bits 11-8:	Sample and hold time = 0010 (16)
	// Bit 7:	Multiple sample and conversion = 0 (disabled)
	// Bit 6:	Reference generator voltage = 0 (1.5V)
	// Bit 5:	Reference generator on = 1 (on)
	// Bit 4:	ADC12 on = 1 (on)
	// Bit 3:	ADCOVIE overflow-interrupt = 0 (disabled)
	// Bit 2:	ADC12TOVIE conversion-time-overflow interrupt enable = 0 (disabled)
	// Bit 1:	ENC Enable conversion = 1 (enabled)
	// Bit 0:	ADC128C start conversion = 0 (don't start)
        ADC12CTL0 = SHT0_8 + REFON + ADC12ON;

	// Bits 15-12:	CSTART ADDx Conversion start address = 0 (0x00 to 0xfh)
	// Bits 11-10:  SHSx Sample-and-hold source select = 00  (ADC12SC bit) ??????
	// Bit 9:	SHP sample+hold pulse mode select = 1 (timer)
	// Bit 8:	Invert signal sample-and-hold = 0 (no)
	// Bit 7-5:	ADC12DIVx clock divider = 111 (8)
	// Bits 4-3:	ADC12SSELx Clock source select = 11 (SMCLK)
	// Bits 2-1:	CONSEQx conversion sequence mode select = 00 (single-channel, single-conversion)
	// Bit 0:	ADC12busy = 0 (no)
        ADC12CTL1 = SHP;
	
	// Bit 7: 	EOS = 1 (end of sequence)
	// Bits 6-4:	SREFx Reference Voltage = 001 (V+ = Vcc, V- = AVss)
	// Bits 3-0:	INCHx Input = 1010 (Temperature Sensor)
        ADC12MCTL0 = SREF_1 + INCH_10;
  
        // enable sample timer
        ADC12CTL0 |= ENC;
}

int readTemperature(void) {
    int result; 
    
    // start the sampling
    ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start

    // wait for the sampling to complete
    sleepThread(25);

    // read the temperature
    result = ((int)ADC12MEM0 - TEMPERATURE_ZERO_OFFSET) *
        TEMPERATURE_SLOPE_NUMERATOR / TEMPERATURE_SLOPE_DENOMINATOR;

    return result;
}

void displayTemperature(void) {
    long int startTime;
    char temp;

    acquireMutex(MUTEX_SSD);
    isDisplayingTemperature = true;
    releaseMutex(MUTEX_SSD);
 
    // display the temperature for some time
    startTime = schedulerTime();

    while (schedulerTime() - startTime < TEMPERATURE_DISPLAY_TIME) {
      temp = readTemperature();
      COUNTER_DATA_OUT = ((temp / 10) << 4) | (temp % 10);
      sleepThread(200);
    }

    acquireMutex(MUTEX_SSD);
    isDisplayingTemperature = false;
    releaseMutex(MUTEX_SSD);

    stopThread();
}
