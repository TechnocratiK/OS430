#ifndef _COUNTER_COUNTER_H
#define _COUNTER_COUNTER_H

#include "include/defaults.h"

#define COUNTER_DATA_DIR P2DIR
#define COUNTER_DATA_SEL P2SEL
#define COUNTER_DATA_OUT P2OUT
#define COUNTER_RESET_DIR P1DIR
#define COUNTER_RESET_SEL P1SEL
#define COUNTER_RESET_OUT P1OUT
#define COUNTER_RESET_PIN 0x01

#define COUNTER_RUNNING 1
#define COUNTER_STOPPED 0

extern bool isDisplayingTemperature;

void counterThread(void);

void setCounter(char newCountValue);
void startCounter(void);
void stopCounter(void);

#endif                                  // _COUNTER_COUNTER_H
