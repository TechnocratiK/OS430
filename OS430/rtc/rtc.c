/*
//
//  rtc.c
//
//  Created by: Andrew Doyle on 25 November 2010
//  Last modified by: Andrew Doyle on 28 November 2010
//
//  Interface to the RTC chip.  rtcReadTime() and rtcWriteTime() methods read or
//  write the time from the RTC chip itself.  The systemTime struct holds the
//  following, stored as chars:
//		-year
//		-month
//		-day
//		-weekday
//		-hour
//		-minute
//		-second
//
//  The RTC chip is wired via the H2 header to the MSP430 with the following
//  connections:
//		-H2 pin 35 :: CE :: P3 bit 0
//		-H2 pin 36 :: IO ::  P3 bit 1
//		-H2 pin 37 :: DATA :: P3 bit 2
//		-H2 pin 38 :: CLK :: P3 bit 3
//
*/

#include <msp430x14x.h>
#include "rtc.h"
#include "scheduler/scheduler.h"

#define 	clkHigh()	RTC_PORT_OUT |= (RTC_PIN_CLK)
#define 	clkLow()	RTC_PORT_OUT &= ~(RTC_PIN_CLK)

#define 	startComm()	RTC_PORT_OUT |= RTC_PIN_CE
#define 	endComm()	RTC_PORT_OUT &= ~(RTC_PIN_CE)

void rtcInit(void) {
  RTCTime time;
  
  RTC_PORT_SEL &= ~(RTC_PIN_CE | RTC_PIN_CLK | RTC_PIN_DATA | RTC_PIN_IO);
  
  // CE, IO and CLK are output
  RTC_PORT_DIR |= (RTC_PIN_CE | RTC_PIN_IO | RTC_PIN_CLK);

  // DATA switches from input to output
  RTC_PORT_DIR &= ~(RTC_PIN_DATA);

  // initialize the RTC with a time
  time.year = 10;
  time.month = 12;
  time.day = 02;
  time.hour = 12;
  time.minute = 30;
  time.second = 00;
  rtcWriteTime(&time);
}


// set the direction of the DATA pin to in and set "receive" flag 
void rtcRxMode(void) {
  // DATA as input
  RTC_PORT_DIR &= ~(RTC_PIN_DATA);

  // Write IO flag
  RTC_PORT_OUT &= ~(RTC_PIN_IO);
}

// set the direction of the DATA pin to out and set "write" flag
void rtcTxMode(void) {
  // DATA as output
  RTC_PORT_DIR |= (RTC_PIN_DATA);

  // Write IO flag
  RTC_PORT_OUT |= (RTC_PIN_IO);
}

void rtcReadTime(RTCTime *systemTime) {
  int i;
  long long time;

  clkHigh();
  rtcRxMode();
  clkLow();

  startComm();
	
  systemTime->year = bcdToChar(rtcReadByte());
  systemTime->month = bcdToChar(rtcReadByte());
  systemTime->day = bcdToChar(rtcReadByte());
  systemTime->weekday = bcdToChar(rtcReadNibble());
  systemTime->hour = bcdToChar(rtcReadByte());
  systemTime->minute = bcdToChar(rtcReadByte());
  systemTime->second = bcdToChar(rtcReadByte());

  endComm();
  clkHigh();
  clkLow();
}

void rtcWriteTime(RTCTime *systemTime) {
  int i;

  clkHigh();
  rtcTxMode();
  clkLow();

  startComm();
	
  rtcWriteByte(charToBcd(systemTime->year));
  rtcWriteByte(charToBcd(systemTime->month));
  rtcWriteByte(charToBcd(systemTime->day));
  rtcWriteNibble(charToBcd(systemTime->weekday));
  rtcWriteByte(charToBcd(systemTime->hour));
  rtcWriteByte(charToBcd(systemTime->minute));

  clkHigh();
  endComm();
  clkLow();
}

char rtcReadByte(void) {
  int i;
  char result = 0;

  for (i = 0; i < 8; i++) {
    clkHigh();
		
    if (RTC_PORT_IN & RTC_PIN_DATA)
      result = (result >> 1) | 0x80;
    else
      result = (result >> 1) & 0x7f;
	
    clkLow();
  }

  return result;
}

void rtcWriteByte(char byte) {
  int i;

  for (i = 0; i < 8; i++) {
    clkLow();
		
    if (byte & 0x01)
      RTC_PORT_OUT |= RTC_PIN_DATA;
    else
      RTC_PORT_OUT &= ~(RTC_PIN_DATA);

    clkHigh();

    byte = (byte >> 1) & 0x7f;
  }
}

char rtcReadNibble(void) {
  int i;
  char result = 0;

  for (i = 0; i < 4; i++) {
    clkHigh();
		
    if (RTC_PORT_IN & RTC_PIN_DATA)
      result = (result >> 1) | 0x08;
    else
      result = (result >> 1) & 0x07;
	
    clkLow();
  }

  return result;
}

void rtcWriteNibble(char nibble) {
  int i;

  for (i = 0; i < 4; i++) {
    clkLow();
		
    if (nibble & 0x01)
      RTC_PORT_OUT |= RTC_PIN_DATA;
    else
      RTC_PORT_OUT &= ~(RTC_PIN_DATA);

    clkHigh();

    nibble = (nibble >> 1) & 0x7f;
  }
}

char charToBcd(char x) {
  return ((x / 10) << 4) | (x % 10);
}

char bcdToChar(char x) {
  return ((x & 0xf0) >> 4) * 10 + (x & 0x0f);
}
