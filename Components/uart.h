/**
 * UART H file for send bytes
 *
 * Expects:
 * 1. SMCLK to be running at 8MHz
 * 2. Port 1 and 3 attached to the keypad and P1 interrupt not in use
 * 3. If SPI, Port 3 and 5 not in use elsewhere and ready for use
 * 4. TimerA counting up and interrupt A0 is not in use
 */

#ifndef __INCLUDE_UART_H
#define __INCLUDE_UART_H

#define USE_UART0 0x1
#define USE_UART1 0x2
#define USE_SPI 0x4


#define SPI_CONT_TRANS 0x0
#define SPI_START_TRANS 0x1
#define SPI_END_TRANS 0x2
#define SPI_SINGLE_TRANS (SPI_START_TRANS|SPI_END_TRANS)


/**
 * Select which UART port the next uart method will effect
 */
void uart_init(void);

/**
 * Initialize the UART control registers to a known value and configure for 57600Kbps, no parity, 8-bit, SPI optional
 */
void uart_switch(int type);

/**
 * Put a byte on the UART data lines
 */
void uart_put_byte(char b);

/**
 * Retrieve a value from the uart data lines
 */
char uart_get_byte(void);

/**
 * Send and receive a value via SPI, and optionally pull STE low or high or neither
 */
char uart_spi_byte(char b, int trans);

#endif
