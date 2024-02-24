#ifndef _TERMINAL_TERMINAL_H
#define _TERMINAL_TERMINAL_H

#include "config.h"
#include "include/defaults.h"
#include "mutex/mutex.h"
#include "pipes/pipes.h"

#define TERMINAL_COLOR_BLACK '0'
#define TERMINAL_COLOR_RED '1'
#define TERMINAL_COLOR_GREEN '2'
#define TERMINAL_COLOR_YELLOW '3'
#define TERMINAL_COLOR_BLUE '4'
#define TERMINAL_COLOR_MAGENTA '5'
#define TERMINAL_COLOR_CYAN '6'
#define TERMINAL_COLOR_WHITE '7'
#define TERMINAL_COLOR_DEFAULT '9'

#define TERMINAL_ATTRIBUTE_NORMAL '0'
#define TERMINAL_ATTRIBUTE_BOLD '1'
#define TERMINAL_ATTRIBUTE_UNDERLINED '4'
#define TERMINAL_ATTRIBUTE_BLINK '5'
#define TERMINAL_ATTRIBUTE_INVERSE '7'
#define TERMINAL_ATTRIBUTE_INVISIBLE '8'

#define TERMINAL_DIRECTION_UP 'A'
#define TERMINAL_DIRECTION_DOWN 'B'
#define TERMINAL_DIRECTION_LEFT 'C'
#define TERMINAL_DIRECTION_RIGHT 'D'

extern Pipe *terminalReadPipe, *terminalWritePipe;

void initializeTerminal(void);
void terminalReceiveIsr(void);
void terminalTransmitIsr(void);

// terminal controls
#define terminalBurstRead(buffer, length, block) readPipe(terminalReadPipe, buffer, length, block)
unsigned int terminalBurstWrite(const char *buffer, unsigned int length, bool block);
#define terminalReadByte(byte, block) readPipeByte(terminalReadPipe, byte, block)
unsigned int terminalWriteByte(const char *byte, bool block);

void terminalSetForegroundColor(char color);
void terminalSetBackgroundColor(char color);
void terminalSetAttribute(char attribute);
void terminalClearAttribute(char attribute);
void terminalSetCursorPosition(int row, int column);
void terminalSetCursorColumn(int column);
void terminalMoveCursor(int n, char direction);
void terminalClear(void);
void terminalGetTextArea(int *rows, int *columns);
void terminalBackspace(void);
void terminalBell(void);

// terminal lock
#define terminalLock() acquireMutex(MUTEX_TERMINAL)
#define terminalUnlock() releaseMutex(MUTEX_TERMINAL)

// for baud rates
#if (TERMINAL_BAUD_RATE == 1200)
  #define TERMINAL_BR1 0x1a
  #define TERMINAL_BR0 0x0a
  #define TERMINAL_MCTL 0xee
#elif (TERMINAL_BAUD_RATE == 2400)
  #define TERMINAL_BR1 0x0d
  #define TERMINAL_BR0 0x05
  #define TERMINAL_MCTL 0x11
#elif (TERMINAL_BAUD_RATE == 4800)
  #define TERMINAL_BR1 0x06
  #define TERMINAL_BR0 0x82
  #define TERMINAL_MCTL 0xee
#elif (TERMINAL_BAUD_RATE == 9600)
  #define TERMINAL_BR1 0x03
  #define TERMINAL_BR0 0x41
  #define TERMINAL_MCTL 0x11
#elif (TERMINAL_BAUD_RATE == 14400)
  #define TERMINAL_BR1 0x02
  #define TERMINAL_BR0 0x2b
  #define TERMINAL_MCTL 0x55
#elif (TERMINAL_BAUD_RATE == 19200)
  #define TERMINAL_BR1 0x01
  #define TERMINAL_BR0 0xa0
  #define TERMINAL_MCTL 0xee
#elif (TERMINAL_BAUD_RATE == 28800)
  #define TERMINAL_BR1 0x01
  #define TERMINAL_BR0 0x15
  #define TERMINAL_MCTL 0xbb
#elif (TERMINAL_BAUD_RATE == 33600)
  #define TERMINAL_BR1 0x00
  #define TERMINAL_BR0 0xee
  #define TERMINAL_MCTL 0x20
#elif (TERMINAL_BAUD_RATE == 38400)
  #define TERMINAL_BR1 0x00
  #define TERMINAL_BR0 0xd0
  #define TERMINAL_MCTL 0x92
#elif (TERMINAL_BAUD_RATE == 57600)
  #define TERMINAL_BR1 0x00
  #define TERMINAL_BR0 0x8a
  #define TERMINAL_MCTL 0xfd
#elif (TERMINAL_BAUD_RATE == 115200)
  #define TERMINAL_BR1 0x00
  #define TERMINAL_BR0 0x45
  #define TERMINAL_MCTL 0x29
#endif

#endif                                  // _TERMINAL_TERMINAL_H
