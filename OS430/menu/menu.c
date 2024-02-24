#include "include/defaults.h"
#include "scheduler/scheduler.h"
#include "terminal/terminal.h"
#include "menu/menu.h"
#include "rtc/rtc.h"
#include "counter/counter.h"
#include "chat/chat.h"
#include "temperature/temperature.h"
#include "lcd/lcd.h"

static const char userPrompt[] = "Enter counter start value: ";
static const char Temperature[] = "Temparature is: ";
static const char invalidInput[] = "Invalid Input";
static const char space = ' ';
static const char menuText[] =
"Choose one of the following options:\r\n\
------------------------------------\r\n\
1. Display Real Time Clock\r\n\
2. Counter Menu\r\n\
3. Display Temparature\r\n\
4. Chat\r\n\
\r\n\
> ";

static const char menuTextCount [] = 
"\r\nChoose one of the following options:\r\n\
------------------------------------\r\n\
1. Start Timer from Zero\r\n\
2. Start Timer from user value\r\n\
3. Stop Timer\r\n\
4. Resume Timer\r\n\
5. Main Menu\r\n\
\r\n\
> ";

static const char pressEscapeToReturn[] = "Press Escape to return...";
static const char crlf[] = "\r\n";
char realtimeClockReadout[] = "20  /  /     :  :  ";


//static const char *weekdays[] = {"Monday", "Tuesday", "Wednesday"

void menuThread(void) {
  while (true) {
    // display the menu
    menuDisplay(menuText, sizeof(menuText));

    switch (menuGetInteger()) {
      case 1:
        menuRealtimeClock();
        break;
      case 2:
        menuCounter();
        break;
      case 3:
        menuTemperature();
        break;
      case 4: 
        chat();
        break;
    }
  }
}

void menuDisplay(const char *menuString, int length) {
  int pos = 0;
  
  while (pos < length - 31) {
    terminalBurstWrite(&(menuString[pos]), 32, true);
    pos += 32;
  }

  if (pos < length)
    terminalBurstWrite(&(menuString[pos]), length - pos, true);
}

int menuGetInteger(void) {
  int result = 0;
  int keysTyped = 0;
  char key;

  while (true) {
    terminalReadByte(&key, true);

    if (key == 0x1b) {
      return -2;
    } else if (key == 0x0d) {
      terminalBurstWrite(crlf, 2, true);
      return (keysTyped > 0) ? result : -1;
    } else if (key == 0x7f) {
      if (keysTyped == 0) {
        terminalBell();
      } else {
        result /= 10;
        keysTyped--;
        terminalBackspace();
      }
    } else if (key >= '0' && key <= '9') {
      if (result > 3276 || (result == 3276 && key > '7')) {
        terminalBell();
      } else {
        result = result * 10 + (key - '0');
        keysTyped++;
        terminalWriteByte(&key, true);
      }
    } else
      terminalBell();
  }
}

void menuRealtimeClock(void) {
  char data, key;
  RTCTime time;
  int i;


  // display the real time clock text
  terminalBurstWrite(crlf, 2, true);
  terminalBurstWrite(crlf, 2, true);
  terminalBurstWrite(pressEscapeToReturn, sizeof(pressEscapeToReturn), true);
  terminalBurstWrite(crlf, 2, true);
  terminalBurstWrite(realtimeClockReadout, sizeof(realtimeClockReadout), true);
      
  while (true) {
    // display the real time clock
    rtcReadTime(&time);
    for (i = 0; i < 3; i++) {
      terminalSetCursorColumn(i * 3 + 3);
      data = (((char *)(&time))[i] / 10) + '0';
      terminalWriteByte(&data, true);
      data = (((char *)(&time))[i] % 10) + '0';
      terminalWriteByte(&data, true);
    }

    terminalSetCursorColumn(13);
    for (i = 0; i < 3; i++) {
      terminalSetCursorColumn(i * 3 + 12);
      data = (((char *)(&time))[i + 4] / 10) + '0';
      terminalWriteByte(&data, true);
      data = (((char *)(&time))[i + 4] % 10) + '0';
      terminalWriteByte(&data, true);
    }

    // check if a key is available
    if (terminalReadByte(&key, false))
      if (key == 0x1b) {
        terminalBurstWrite(crlf, 2, true);
        terminalBurstWrite(crlf, 2, true);
        return;
      }
    
    // sleep for a second
    sleepThread(1000);

    // DEBUG: Increment the clock
    /*time.second = (time.second == 59) ? 0 : time.second + 1;
    time.minute = (time.minute == 59) ? 0 : time.minute + 
        ((time.second == 0) ? 1 : 0);
    time.hour = (time.hour == 23) ? 0 : time.hour +
        ((time.minute == 0 && time.second == 0) ? 1 : 0);*/
  }
}

void menuCounter(void) {
  int userValue = 0;
  menuDisplay(menuTextCount, sizeof(menuTextCount));
  switch (menuGetInteger()) {
      case 1:
        setCounter(0);
        startCounter();
        break;
      case 2:
        terminalBurstWrite(userPrompt,sizeof(userPrompt),true);
        userValue = menuGetInteger();
        if(userValue < 0) {
          terminalBurstWrite(invalidInput, sizeof(invalidInput), true);          
        } else {
          userValue = userValue % 60; 
          setCounter(userValue);
          startCounter();
        }
        break;
      case 3:
        stopCounter();
        break;
      case 4:
        startCounter();
        break;
      case 5: 
        break;
      default:
        terminalBurstWrite(invalidInput, sizeof(invalidInput), true);
        break;
  }

  terminalBurstWrite(crlf, 2, true);
}

void menuTemperature(void){
  int temp;
  char tempChars[2];
  terminalBurstWrite(crlf, 2, true);
  terminalBurstWrite(Temperature, sizeof(Temperature), true);
  terminalWriteByte(&space,true);
  terminalWriteByte(&space,true);

  temp = readTemperature();
  tempChars[0] = (temp / 10) + '0';
  tempChars[1] = (temp % 10) + '0';

  if (tempChars[0] == '0')
    terminalWriteByte(&(tempChars[1]), true);
  else
    terminalBurstWrite(tempChars, 2, true);

  // display the temperature on the SSD for some time
  startThread(displayTemperature, 32);

  terminalBurstWrite(crlf, 2, true);
  terminalBurstWrite(crlf, 2, true);
}
