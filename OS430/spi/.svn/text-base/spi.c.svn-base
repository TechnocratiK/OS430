#include <msp430x14x.h>
#include "scheduler/scheduler.h"
#include "spi/spi.h"

char spiByte(char output, unsigned int option) {
  char result;

  if (option & SPI_TRANSMISSION_START)
    P5OUT &= ~0x01;
  
  // wait for the transmission buffer to be empty
  while ((IFG2 & UTXIFG1) == 0)
    yieldThread();

  // write the byte to enable reading
  U1TXBUF = output;

  // wait for a byte to be received
  while ((IFG2 & URXIFG1) == 0)
    yieldThread();

  // read the byte and wait a bit
  result = U1RXBUF;
  pause(488);

  if (option & SPI_TRANSMISSION_END)
    P5OUT |= 0x01;

  return result;
}

void initializeSpi(void) {
  unsigned int i;
  char CTL = 0, TCTL = 0;
  char BR1, BR0, MCTL;

  // clear the OSCFault flag and wait for it to stay unset
  do {
    IFG1 &= ~OFIFG;
    for (i = 0xff; i > 0; i--);
  } while ((IFG1 & OFIFG) != 0);

  // set the USART control register (U1CTL)
  /* Bit    Name  Description
     7-6          Unused
       5     I2C  I2C Mode Enabled
       4    CHAR  Character length (0 = 7, 1 = 8)
       3  LISTEN  Listen enable (=1)
       2    SYNC  Synchronous mode enable (=1)
       1      MM  Multiprocessor mode select (0 = Idle-line, 1 = Address-bit)
       0   SWRST  Software reset enable (=1)
  */
  // set SWRST
  CTL |= SWRST;

  // set U1CTL for SPI
  TCTL |= CKPH | STC;
  CTL |= SYNC | MM;

  // set the baud rate registers (U1BR1:U1BR0)
  BR1 = 0x00;
  BR0 = 0x8A;

  // set the modulation register
  MCTL = 0x00;

  // set the BRCLK source (see below for details)
  TCTL |= SSEL1 | SSEL0;

  // use 8-bit characters
  CTL |= CHAR;

  // load the registers
  U1CTL = CTL;
  U1BR1 = BR1;
  U1BR0 = BR0;
  U1MCTL = MCTL;
  U1TCTL = TCTL;

  // enable the UART1 transceiver
  ME2 |= USPIE1;
  //IE2 |= URXIE1;
  U1CTL &= ~SWRST;

  // setup the SPI port
  P5SEL = (P5SEL & 0xf0) | 0x0e;
  P5DIR = (P5DIR & 0xf0) | 0x0b;
  P5OUT |= 0x01;
}
