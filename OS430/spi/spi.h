#ifndef _SPI_SPI_H
#define _SPI_SPI_H

#define SPI_TRANSMISSION_START 0x01
#define SPI_TRANSMISSION_END 0x02
#define SPI_TRANSMISSION_CONTINUE 0x00
#define SPI_TRANSMISSION_SINGLE 0x03

void initializeSpi(void);

#define spiStartCommunication() P5OUT &= ~0x01
#define spiStopCommunication() P5OUT |= 0x01
void spiTxContinueWait(void);
void spiTxEndWait(void);
char spiByte(char output, unsigned int option);

#endif                                  // _SPI_SPI_H
