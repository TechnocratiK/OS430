
#include "common.h"
#include "keyboard.h"
#include "hyperterm.h"
#include "timer.h"
#include "cc2500.h"


void main(void) {
  char temp;
  int data[3];

  // Stop watchdog.
  WDTCTL = WDTPW + WDTHOLD;
  _EINT();
  // Turning 8MHZ Oscillator ON
  BCSCTL1 &= ~XT2OFF;
  // Route XT2 into SMCLK and MCLK
  BCSCTL2 |= SELM1|SELS;
  // ACLK (32.768KHz), clear TAR
  TACTL = TASSEL_1 + TACLR + MC1;

  timer_init();
  hyperterm_init();
  cc2500_init();
  lcd_init();
  keyboard_init();



  P5OUT &= ~0x40;

  while(TRUE) {
    keyboard_getchar();
  }
  
}

