#include <msp430x14x.h>
#include "include/defaults.h"
//#include "lcd/lcd.h"
#include "cc2500/cc2500.h"
#include "rtc/rtc.h"
#include "scheduler/scheduler.h"
#include "terminal/terminal.h"
#include "testing/testing.h"
#include "memory/memory.h"
#include "counter/counter.h"

void tempTestingThread(void) {
  
  //displayTemperature();
}

void counterTestingThread(void) {
  startCounter();
}

void writeHexByte(char byte) {
  char data[4] = "0x00";

  data[2] = (byte >> 4) & 0x0f;
  data[3] = byte & 0x0f;
  data[2] += (data[2] < 10) ? '0' : 'a' - 10;
  data[3] += (data[3] < 10) ? '0' : 'a' - 10;
  terminalBurstWrite(data, 4, true);
}

/*void lcdTest() {
  char *data;
  data = allocateMemory(24 * sizeof(char));
  lcd_init();
  initializeTerminal();
  lcd_printf("abcdefgh");
  lcd_goto(LCD_LINE2);
  lcd_printf("ooooo");
  lcd_goto(LCD_LINE1);
  lcd_printf("Line one");
  while(TRUE);
}*/
