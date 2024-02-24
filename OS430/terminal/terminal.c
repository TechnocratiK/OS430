#include <msp430x14x.h>
#include "pipes/pipes.h"
#include "scheduler/scheduler.h"
#include "utility/utility.h"
#include "terminal/terminal.h"

Pipe *terminalReadPipe, *terminalWritePipe;

static char foregroundColorControlSequence[] = "\x01b[39m";
static char backgroundColorControlSequence[] = "\x01b[49m";
static char setAttributeControlSequence[] = "\x01b[0m";
static char clearAttributeControlSequence[] = "\x01b[0m";
static char setCursorPositionControlSequence[] = "\x01b[000;000H";
static char setCursorColumnControlSequence[] = "\x01b[000`";
static char moveCursorControlSequence[] = "\x01b[000A";
static const char clearControlSequence[] = "\x01b[2J";
static const char getTextAreaControlSequence[] = "\x01b[18t";
static char getTextAreaResponse[12];

static bool isWriting = false;

void initializeTerminal(void) {
  // allocate two 64-byte pipes for the terminal
  terminalReadPipe = createPipe(64);
  terminalWritePipe = createPipe(64);

  // set the USART control register (U0CTL)
  /* Bit    Name  Description
       7    PENA  Parity enable (=1)
       6     PEV  Parity select (Odd = 0)
       5     SPB  Stop bit select (0 = One, 1 = Two)
       4    CHAR  Character length (0 = 7, 1 = 8)
       3  LISTEN  Listen enable (=1)
       2    SYNC  Synchronous mode enable (=1)
       1      MM  Multiprocessor mode select (0 = Idle-line, 1 = Address-bit)
       0   SWRST  Software reset enable (=1)
  */
  // set SWRST
  U0CTL |= SWRST;

  // set U0CTL for 8N1
  U0CTL &= ~(PENA | PEV | SPB | LISTEN | SYNC | MM);
  U0CTL |= CHAR;

  // set the baud rate registers (U0BR1:U0BR0)
  U0BR1 = TERMINAL_BR1;
  U0BR0 = TERMINAL_BR0;

  // set the modulation register
  U0MCTL = TERMINAL_MCTL;

  // set the BRCLK source (see below for details)
  U0TCTL |= SSEL1;

  // set the USART receive control register (U0RCTL)
  /* Bit    Name  Description
       7      FE  Framing error flag (0 = no error, 1 = low stop bit)
       6      PE  Parity error flag (0 = no error, 1 = parity error)
       5      OE  Overrun error flag (0 = no error, 1 = overrun occurred)
       4     BRK  Break flag (0 = no break condition, 1 = break condition)
       3  URXEIE  Receive erroneous-character interrupt-enable (0 = discard errors, 1 = ignore errors)
       2  URXWIE  Receive wake-up interrupt-enable (0 = all characters, 1 = only addresses)
       1  RXWAKE  Receive wake-up flag (0 = received character is data, 1 = received character is an address)
       0   RXERR  Receive error flag (0 = no errors, 1 = receive error detected)
  */
  U0RCTL = 0x00;

  // set the USART transmit control register (U0TCTL)
  /* Bit    Name  Description
       7          Unused
       6    CKPL  Clock polarity select (0 = UCLKI is non-inverted UCLK, 1 = UCLKI is inverted UCLK)
     5-4   SSELx  Source select (00 = UCLKI, 01 = ACLK, 1x = SMCLK)
       3   URXSE  UART Receive Start Edge (0 = disabled, 1 = enabled)
       2  TXWAKE  Transmitter Wake (0 = next character is data, 1 = next character is an address)
       1          Unused
       0   TXEPT  Transmitter empty (0 = transmission in progress, 1 = transmission complete)
  */
  U0TCTL &= ~(TXWAKE);

  // enable transmission and reception on USART0
  ME1 |= URXE0 | UTXE0;

  // reinitialize the USART
  U0CTL &= ~SWRST;

  // enable transmission and reception interrupts on USART0
  IE1 |= URXIE0 | UTXIE0;

  // setup the UART port
  P3SEL |= 0x30;
}

void terminalReceiveIsr(void) __interrupt[USART0RX_VECTOR] {
  char data = U0RXBUF;

  // write a byte into the read pipe, ignoring the outcome
  writePipeByteIsr(terminalReadPipe, &data);
}

void terminalTransmitIsr(void) __interrupt[USART0TX_VECTOR] {
  char data;

  // read a byte from the write pipe, if available
  if (readPipeByteIsr(terminalWritePipe, &data)) {
    isWriting = true;
    U0TXBUF = data;
  } else
    isWriting = false;
}

unsigned int terminalBurstWrite(const char *buffer, unsigned int length, bool block) {
  unsigned int statusRegister;

  // write to the pipe
  writePipe(terminalWritePipe, buffer, length, block);

  // only flag for the interrupt if the USART isn't writing
  storeAndDisableInterrupts(&statusRegister);
  if (!isWriting)
    IFG1 |= UTXIFG0;
  restoreInterrupts(&statusRegister);
}

unsigned int terminalWriteByte(const char *byte, bool block) {
  unsigned int statusRegister;

  // write to the pipe
  writePipeByte(terminalWritePipe, byte, block);

  // only flag for the interrupt if the USART isn't writing
  storeAndDisableInterrupts(&statusRegister);
  if (!isWriting)
    IFG1 |= UTXIFG0;
  restoreInterrupts(&statusRegister);
}

void terminalSetForegroundColor(char color) {
  foregroundColorControlSequence[3] = color;
  terminalBurstWrite(foregroundColorControlSequence, 5, true);
}

void terminalSetBackgroundColor(char color) {
  backgroundColorControlSequence[3] = color;
  terminalBurstWrite(backgroundColorControlSequence, 5, true);
}

void terminalSetAttribute(char attribute) {
  setAttributeControlSequence[2] = attribute;
  terminalBurstWrite(setAttributeControlSequence, 4, true);
}

void terminalClearAttribute(char attribute) {
  if (attribute == TERMINAL_ATTRIBUTE_NORMAL || attribute == TERMINAL_ATTRIBUTE_BOLD)
    attribute = 2;

  clearAttributeControlSequence[3] = attribute;
  terminalBurstWrite(clearAttributeControlSequence, 5, true);
}

void terminalSetCursorPosition(int row, int column) {
  if (row < 1 || column < 1)
    return;
  
  setCursorPositionControlSequence[2] = (char)(row / 100) + '0';
  setCursorPositionControlSequence[3] = (char)((row / 10) % 10) + '0';
  setCursorPositionControlSequence[4] = (char)(row % 10) + '0';
  setCursorPositionControlSequence[6] = (char)(column / 100) + '0';
  setCursorPositionControlSequence[7] = (char)((column / 10) % 10) + '0';
  setCursorPositionControlSequence[8] = (char)(column % 10) + '0';
  terminalBurstWrite(setCursorPositionControlSequence, 10, true);
}

void terminalSetCursorColumn(int column) {
  if (column < 1)
    return;
  
  setCursorColumnControlSequence[2] = (char)(column / 100) + '0';
  setCursorColumnControlSequence[3] = (char)((column / 10) % 10) + '0';
  setCursorColumnControlSequence[4] = (char)(column % 10) + '0';
  terminalBurstWrite(setCursorColumnControlSequence, 6, true);
}

void terminalMoveCursor(int n, char direction) {
  moveCursorControlSequence[2] = (char)(n / 100) + '0';
  moveCursorControlSequence[3] = (char)((n / 10) % 10) + '0';
  moveCursorControlSequence[4] = (char)(n % 10) + '0';
  moveCursorControlSequence[5] = direction;
  terminalBurstWrite(moveCursorControlSequence, 6, true);
}

void terminalClear(void) {
  terminalBurstWrite(clearControlSequence, 4, true);
}

void terminalGetTextArea(int *rows, int *columns) {
  int i;

  // send the command to request the text area size
  terminalBurstWrite(getTextAreaControlSequence, 5, true);

  // expect an escape sequence back
  for (i = 0; i < 12; i++) {
    terminalReadByte(&(getTextAreaResponse[i]), true);
    if (getTextAreaResponse[i] == 't')
      break;
  }

  // parse the string
  *rows = 0;
  for (i = 4; getTextAreaResponse[i] >= '0' && getTextAreaResponse[i] <= '9'; i++)
    *rows = *rows * 10 + (int)(getTextAreaResponse[i] - '0');
  
  *columns = 0; i++;
  for (; getTextAreaResponse[i] >= '0' && getTextAreaResponse[i] <= '9' && i < 12; i++)
    *columns = *columns * 10 + (int)(getTextAreaResponse[i] - '0');
}

void terminalBackspace(void) {
  char bs = 0x7f;

  terminalWriteByte(&bs, true);
}

void terminalBell(void) {
  char bel = 0x07;

  terminalWriteByte(&bel, true);
}
