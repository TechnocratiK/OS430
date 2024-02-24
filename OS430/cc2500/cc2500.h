#ifndef _CC2500_CC2500_H
#define _CC2500_CC2500_H

#include "config.h"
#include "chat/message.h"

#define CC2500_REGISTER_IOCFG2 0x00
#define CC2500_REGISTER_IOCFG1 0x01
#define CC2500_REGISTER_IOCFG0 0x02
#define CC2500_REGISTER_FIFOTHR 0x03
#define CC2500_REGISTER_SYNC1 0x04
#define CC2500_REGISTER_SYNC0 0x05
#define CC2500_REGISTER_PKTLEN 0x06
#define CC2500_REGISTER_PKTCTRL1 0x07
#define CC2500_REGISTER_PKTCTRL0 0x08
#define CC2500_REGISTER_ADDR 0x09
#define CC2500_REGISTER_CHANNR 0x0a
#define CC2500_REGISTER_FSCTRL1 0x0b
#define CC2500_REGISTER_FSCTRL0 0x0c
#define CC2500_REGISTER_FREQ2 0x0d
#define CC2500_REGISTER_FREQ1 0x0e
#define CC2500_REGISTER_FREQ0 0x0f
#define CC2500_REGISTER_MDMCFG4 0x10
#define CC2500_REGISTER_MDMCFG3 0x11
#define CC2500_REGISTER_MDMCFG2 0x12
#define CC2500_REGISTER_MDMCFG1 0x13
#define CC2500_REGISTER_MDMCFG0 0x14
#define CC2500_REGISTER_DEVIATN 0x15
#define CC2500_REGISTER_MCSM2 0x16
#define CC2500_REGISTER_MCSM1 0x17
#define CC2500_REGISTER_MCSM0 0x18
#define CC2500_REGISTER_FOCCFG 0x19
#define CC2500_REGISTER_BSCFG 0x1a
#define CC2500_REGISTER_AGCCTRL2 0x1b
#define CC2500_REGISTER_AGCCTRL1 0x1c
#define CC2500_REGISTER_AGCCTRL0 0x1d
#define CC2500_REGISTER_WOREVT1 0x1e
#define CC2500_REGISTER_WOREVT0 0x1f
#define CC2500_REGISTER_WORCTRL 0x20
#define CC2500_REGISTER_FREND1 0x21
#define CC2500_REGISTER_FREND0 0x22
#define CC2500_REGISTER_FSCAL3 0x23
#define CC2500_REGISTER_FSCAL2 0x24
#define CC2500_REGISTER_FSCAL1 0x25
#define CC2500_REGISTER_FSCAL0 0x26
#define CC2500_REGISTER_RCCTRL1 0x27
#define CC2500_REGISTER_RCCTRL0 0x28
#define CC2500_REGISTER_FSTEST 0x29
#define CC2500_REGISTER_PTEST 0x2a
#define CC2500_REGISTER_AGCTEST 0x2b
#define CC2500_REGISTER_TEST2 0x2c
#define CC2500_REGISTER_TEST1 0x2d
#define CC2500_REGISTER_TEST0 0x2e
#define CC2500_REGISTER_PARTNUM 0xf0
#define CC2500_REGISTER_VERSION 0xf1
#define CC2500_REGISTER_FREQEST 0xf2
#define CC2500_REGISTER_LQI 0xf3
#define CC2500_REGISTER_RSSI 0xf4
#define CC2500_REGISTER_MARCSTATE 0xf5
#define CC2500_REGISTER_WORTIME1 0xf6
#define CC2500_REGISTER_WORTIME0 0xf7
#define CC2500_REGISTER_PKTSTATUS 0xf8
#define CC2500_REGISTER_VCO_VC_DAC 0xf9
#define CC2500_REGISTER_TXBYTES 0xfa
#define CC2500_REGISTER_RXBYTES 0xfb
#define CC2500_REGISTER_RCCTRL1_STATUS 0xfc
#define CC2500_REGISTER_RCCTRL0_STATUS 0xfd

#define CC2500_STROBE_SRES 0x30
#define CC2500_STROBE_SFSTXON 0x31
#define CC2500_STROBE_SXOFF 0x32
#define CC2500_STROBE_SCAL 0x33
#define CC2500_STROBE_SRX 0x34
#define CC2500_STROBE_STX 0x35
#define CC2500_STROBE_SIDLE 0x36
#define CC2500_STROBE_SWOR 0x38
#define CC2500_STROBE_SPWD 0x39
#define CC2500_STROBE_SFRX 0x3a
#define CC2500_STROBE_SFTX 0x3b
#define CC2500_STROBE_SWORRST 0x3c
#define CC2500_STROBE_SNOP 0x3d

#define CC2500_ADDRESS_READ 0x80
#define CC2500_ADDRESS_WRITE 0x00
#define CC2500_ADDRESS_BURST 0x40

#define CC2500_DATA_BUFFER 0x3f

#define CC2500_PACKET_MESSAGE 0x00
//#define CC2500_PACKET_

void initializeCC2500(void);
void cc2500InitializeRegisters(void);

char cc2500ReadRegister(char address);
void cc2500WriteRegister(char address, char value);
void cc2500StrobeCommand(char strobe);
void cc2500ReceivePacket(void);
void cc2500TransmitPacket(void);
void cc2500ProcessPacket(void);

void cc2500SendMessage(Message *message, int length);

void cc2500Thread(void);

#endif                                  // _CC2500_CC2500_H
