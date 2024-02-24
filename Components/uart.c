
#include "common.h"
#include "uart.h"
#include "timer.h"

int uart_port = USE_UART0;
int uart_status = FALSE;
char uart_rx_buffer = 0x0;


// Comment in H file
void uart_switch(int type) {
  uart_port = type;
}

void uart_config_io(void) {
  if(uart_port & USE_UART0) {
    if(uart_port & USE_SPI) {
      P3SEL |= 0x0E;	// All use Prt
      P3DIR |= 0x0B;	// P3.0,1,3 OUT
      P3OUT |= 0x01; // STE is high.
    }
    else {
      P3SEL |= 0x30;  // P3.4,5 = USART0 TXD/RXD
      P3DIR |= 0x10;  // P3.4 output direction
    }
  }
  else if(uart_port & USE_UART1) {
    if(uart_port & USE_SPI) {
      P5SEL |= 0x0E;	// Set all to use port and STE to be GPIO
      P5DIR |= 0x0B;	//P5.0,1,3 OUT
      P5OUT |= 0x01;    // STE is high.
    }
    else {
      P3SEL |= 0xC0;                        // P3.6,7 = USART1 option select
      P3DIR |= 0x20;                        // P3.6 = output direction
    }
  }

}

// Comment in H file
void uart_init(void) {
  unsigned int i;
  int ctl = 0, tctl = 0, br0, br1, mctl;

  do {
    IFG1 &= ~OFIFG;                       // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);           // Time for flag to set
  } while ((IFG1 & OFIFG) != 0);         // OSCFault flag still set? 

  if(uart_port & USE_SPI) {
    ctl |= SWRST;	//SPI
    tctl |=  CKPH | STC;
    ctl |= SYNC | MM; // SPI
  }
  
  //Select SMCLK clk
  tctl |= SSEL1|SSEL0;
  
  // 8-bit characters
  ctl |= CHAR;

  // Set baud rate 67.5 kbps @ 8MHz
  // Calculated here: http://www.daycounter.com/Calculators/MSP430-Uart-Calculator.phtml
  /* uart0 8000000Hz 57595bps =~ 8000000/(0x8A.0x00) */
  br0=0x8A; //setting the baudrate divider
  br1=0x00;
  mctl=0xEF; 

  uart_config_io();

  if(uart_port & USE_UART0) {
    U0CTL = ctl;
    U0BR0 = br0;
    U0BR1 = br1;
    U0MCTL = mctl;
    U0TCTL = tctl;
    
    //Enabling the UART0 TX/RX Module
    ME1 |= UTXE0 + URXE0;
    IE1 |= URXIE0;                        // Enable USART0 RX interrupt

    U0CTL &= ~SWRST;

  }
  else if(uart_port & USE_UART1) {
    // 9600 baud
//    br0 = 0x3;
//    br1 = 0x41;
//    mctl = 0x92;
    
    
    U1CTL = ctl;
    U1BR0 = br0;
    U1BR1 = br1;
    U1MCTL = mctl;
    U1TCTL = tctl;
    
    //Enabling the UART1 TX/RX Module
    //ME2 |= UTXE1 + URXE1;
    ME2 |= USPIE1;
    IE2 |= URXIE1;                        // Enable USART1 RX interrupt

    U1CTL &= ~SWRST;
  }

}

// Comment in H file
void uart_put_byte(char b) {
  if (uart_port & USE_UART0) {
    while ((IFG1 & UTXIFG0) == 0);
    TXBUF0 = b;
  }
  else if (uart_port & USE_UART1){
    while ((IFG2 & UTXIFG1) == 0);
    TXBUF1 = b;
  }
  else return;
}

// Comment in H file
char uart_get_byte(void) {
  uart_status = TRUE;
  _BIS_SR(CPUOFF);
  uart_status = FALSE;
  return uart_rx_buffer;
}

// Comment in H file
char uart_spi_byte(char b, int trans) {
  char data = 0x00;
  //only uart1 at the moment
  if (uart_port & USE_UART1){
    if(trans&SPI_START_TRANS) {
      P5OUT &= ~0x01;
    }
    uart_put_byte(b);
    while ((IFG2 & URXIFG1) == 0);
    data = RXBUF1;

    timer_wait(2);

    if(trans & SPI_END_TRANS) {
      P5OUT |= 0x01;
    }
  }

  return data;
}



/**
 * Receive interrupts for UART. store value and wake CPU if it is waiting for a value, else ignore
 */
void uart0_rx(void) __interrupt[UART0RX_VECTOR] {
  if(uart_port & USE_UART0 && uart_status == TRUE) {
    uart_rx_buffer = RXBUF0;
    _BIC_SR_IRQ(CPUOFF);
  }
}

void uart1_rx(void) __interrupt[UART1RX_VECTOR] {
  if(uart_port & USE_UART1 && uart_status == TRUE) {
    uart_rx_buffer = RXBUF1;
    _BIC_SR_IRQ(CPUOFF);
  }
}
