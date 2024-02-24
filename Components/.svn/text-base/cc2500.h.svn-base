
#include <string.h>
#include "uart.h"
#include "timer.h"
#include "hyperterm.h"


// Register address defines to improve code readability
#define CC2500REG_IOCFG2 0x00
#define CC2500REG_IOCFG1 0x01
#define CC2500REG_IOCFG0 0x02
#define CC2500REG_FIFOTHR 0x03
#define CC2500REG_SYNC1 0x04
#define CC2500REG_SYNC0 0x05
#define CC2500REG_PKTLEN 0x06
#define CC2500REG_PKTCTRL1 0x07
#define CC2500REG_PKTCTRL0 0x08
#define CC2500REG_ADDR 0x09
#define CC2500REG_CHANNR 0x0A
#define CC2500REG_FSCTRL1 0x0B
#define CC2500REG_FSCTRL0 0x0C
#define CC2500REG_FREQ2 0x0D
#define CC2500REG_FREQ1 0x0E
#define CC2500REG_FREQ0 0x0F
#define CC2500REG_MDMCFG4 0x10
#define CC2500REG_MDMCFG3 0x11
#define CC2500REG_MDMCFG2 0x12
#define CC2500REG_MDMCFG1 0x13
#define CC2500REG_MDMCFG0 0x14
#define CC2500REG_DEVIATN 0x15
#define CC2500REG_MCSM2 0x16
#define CC2500REG_MCSM1 0x17
#define CC2500REG_MCSM0 0x18
#define CC2500REG_FOCCFG 0x19
#define CC2500REG_BSCFG 0x1A
#define CC2500REG_AGCCTRL2 0x1B
#define CC2500REG_AGCCTRL1 0x1C
#define CC2500REG_AGCCTRL0 0x1D
#define CC2500REG_WOREVT1 0x1E
#define CC2500REG_WOREVT0 0x1F
#define CC2500REG_WORCTRL 0x20
#define CC2500REG_FREND1 0x21
#define CC2500REG_FREND0 0x22
#define CC2500REG_FSCAL3 0x23
#define CC2500REG_FSCAL2 0x24
#define CC2500REG_FSCAL1 0x25
#define CC2500REG_FSCAL0 0x26
#define CC2500REG_RCCTRL1 0x27
#define CC2500REG_RCCTRL0 0x28
#define CC2500REG_FSTEST 0x29
#define CC2500REG_PTEST 0x2A
#define CC2500REG_AGCTEST 0x2B
#define CC2500REG_TEST2 0x2C
#define CC2500REG_TEST1 0x2D
#define CC2500REG_TEST0 0x2E

#define CC2500REG_SRES 0x30 
#define CC2500REG_SFSTXON 0x31 
#define CC2500REG_SXOFF 0x32 
#define CC2500REG_SCAL 0x33 
#define CC2500REG_SRX 0x34 
#define CC2500REG_STX 0x35 
#define CC2500REG_SIDLE 0x36 
#define CC2500REG_SWOR 0x38 
#define CC2500REG_SPWD 0x39 
#define CC2500REG_SFRX 0x3A 
#define CC2500REG_SFTX 0x3B 


#define CC2500REG_PARTNUM (0xC0|0x30)
#define CC2500REG_VERSION (0xC0|0x31)
#define CC2500REG_FREQEST (0xC0|0x32)
#define CC2500REG_LQI     (0xC0|0x33)
#define CC2500REG_RSSI    (0xC0|0x34)
#define CC2500REG_MARCSTATE   (0xC0|0x35)
#define CC2500REG_WORTIME1    (0xC0|0x36)
#define CC2500REG_WORTIME0    (0xC0|0x37)
#define CC2500REG_PKTSTATUS   (0xC0|0x38)
#define CC2500REG_VCO_VC_DAC  (0xC0|0x39)
#define CC2500REG_TXBYTES     (0xC0|0x3A)
#define CC2500REG_RXBYTES     (0xC0|0x3B)
#define CC2500REG_RCCTRL1_STATUS  (0xC0|0x3C)
#define CC2500REG_RCCTRL0_STATUS  (0xC0|0x3D)


#define CC2500REG_DATABUF 0x3F

#define CC2500_UART (USE_UART1|USE_SPI)

#ifndef __INCLUDE_CC2500_H
#define __INCLUDE_CC2500_H

#define CC2500_WRITE 0x00
#define CC2500_READ 0x80
#define CC2500_BURST 0x40

typedef struct {
  char address;
  char* name;
} cc2500_reg;

/**
 * Please see C file for comments on functions
 */

void cc2500_init(void);
char cc2500_status(void);
char cc2500_read_reg(char i);
void cc2500_write_reg(char i, char val);
void cc2500_reset(void);

void cc2500_setradioregisters(void);


char* cc2500_reg_get_name(int addr);
int cc2500_reg_get_address(char* name);

char* cc2500_receive_packet(int* len);
void cc2500_transmit_packet(char* buffer, int length);

#endif

