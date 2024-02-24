#include <msp430x14x.h>
#include "include/defaults.h"
#include "scheduler/scheduler.h"
#include "counter/counter.h"
#include "mutex/mutex.h"

int counterState;
char countValue;
bool isDisplayingTemperature;

void counterThread(void) {
  countValue = 0;
  counterState = COUNTER_RUNNING;

  // setup the counter ports
  COUNTER_DATA_SEL = 0;
  COUNTER_DATA_DIR |= 0xff;
  COUNTER_RESET_SEL &= ~(COUNTER_RESET_PIN);
  COUNTER_RESET_DIR |= COUNTER_RESET_PIN;

  // reset the counter
  COUNTER_RESET_OUT |= COUNTER_RESET_PIN;
  COUNTER_RESET_OUT &= ~(COUNTER_RESET_PIN);

  // initialize 'isDisplayingTemperature' to false
  isDisplayingTemperature = false;

  while (true) {
      acquireMutex(MUTEX_SSD);
        if (!isDisplayingTemperature)
          COUNTER_DATA_OUT = ((countValue / 10) << 4) | (countValue % 10);
      releaseMutex(MUTEX_SSD);

    if (counterState == COUNTER_RUNNING)
      countValue = (countValue == 59) ? 0 : countValue + 1;

    sleepThread(1000);
  }
}

void setCounter(char newCountValue) {
    // stop the counter
    counterState = COUNTER_STOPPED;
    sleepThread(1000);
    countValue = newCountValue;

    counterState = COUNTER_RUNNING;
}

void startCounter(void) {
  counterState = COUNTER_RUNNING;
}

void stopCounter(void) {
  counterState = COUNTER_STOPPED;
}
