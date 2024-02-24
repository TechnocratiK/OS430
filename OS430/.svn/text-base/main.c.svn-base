#include <msp430x14x.h>
#include "cc2500/cc2500.h"
#include "counter/counter.h"
#include "lcd/lcd.h"
#include "menu/menu.h"
#include "rtc/rtc.h"
#include "scheduler/scheduler.h"
#include "temperature/temperature.h"
#include "terminal/terminal.h"

void main(void) {
  // initialize the temperature measurement
  initializeTemperature();

  // initialize the RTC
  rtcInit();

  // initialize the LCD
  lcd_init();

  // initialize the terminal
  initializeTerminal();

  // clear the terminal and move the cursor to the top-left corner
  terminalClear();
  terminalSetCursorPosition(1, 1);

  // start the cc2500 thread
  startThread(cc2500Thread, 128);

  // start the counter thread
  startThread(counterThread, 32);

  // start the menu thread
  startThread(menuThread, 256);

  // stop this thread
  stopThread();
}
