#include <msp430x14x.h>
#include "include/defaults.h"
#include "scheduler/scheduler.h"
#include "spi/spi.h"
#include "cc2500/cc2500.h"
#include "mutex/mutex.h"
#include "chat/chat.h"
#include "chat/message.h"

static char cc2500ReceiveData[64];
static char cc2500TransmitData[64];
int cc2500ReceiveLength, cc2500TransmitLength;

bool cc2500Transmit;
char cc2500Status;

void initializeCC2500(void) {
  int i; 

  // initialize spi
  initializeSpi();

  // take control of the pins
  P5SEL &= 0xf0;
  P5DIR = (P5DIR & 0xf0) | 0x0b;

  // set the clock and STE high
  P5OUT = (P5OUT & 0xf0) | 0x09;

  // set STE low and wait
  P5OUT &= ~0x01;
  pause(2440);

  // set STE high and wait
  P5OUT |= 0x01;
  pause(2440);

  // set STE low
  P5OUT &= ~0x01;

  // wait for SOMI to go low
  while (P5IN & 0x04)
    yieldThread();
  
  // give control back to the UART
  P5SEL |= 0x0e;

  // send the reset strobe
  cc2500StrobeCommand(CC2500_STROBE_SRES);

  // wait
  pause(2440);

  // set the registers
  cc2500InitializeRegisters();

  // initialize cc2500Transmit to false
  acquireMutex(MUTEX_CC2500_TX);
  cc2500Transmit = false;
  cc2500TransmitLength = 0;
  releaseMutex(MUTEX_CC2500_TX);
}

void cc2500InitializeRegisters(void) {
  cc2500WriteRegister(CC2500_REGISTER_MCSM0,    0x18);
  cc2500WriteRegister(CC2500_REGISTER_MCSM1,    0x3C);

  cc2500WriteRegister(CC2500_REGISTER_FSCTRL1,  0x07);
  cc2500WriteRegister(CC2500_REGISTER_FSCTRL0,  0x00);
  cc2500WriteRegister(CC2500_REGISTER_FREQ2,    0x5D);
  cc2500WriteRegister(CC2500_REGISTER_FREQ1,    0x44);
  cc2500WriteRegister(CC2500_REGISTER_FREQ0,    0xEC);
  cc2500WriteRegister(CC2500_REGISTER_MDMCFG4,  0x2D);
  cc2500WriteRegister(CC2500_REGISTER_MDMCFG3,  0x3B);
  cc2500WriteRegister(CC2500_REGISTER_MDMCFG2,  0x73);
  cc2500WriteRegister(CC2500_REGISTER_MDMCFG1,  0x23);
  cc2500WriteRegister(CC2500_REGISTER_MDMCFG0,  0x3B);
  cc2500WriteRegister(CC2500_REGISTER_CHANNR,   CC2500_CHANNEL);
  cc2500WriteRegister(CC2500_REGISTER_DEVIATN,  0x01);
  cc2500WriteRegister(CC2500_REGISTER_FREND1,   0xB6);
  cc2500WriteRegister(CC2500_REGISTER_FREND0,   0x10);
  cc2500WriteRegister(CC2500_REGISTER_MCSM0,    0x18);
  cc2500WriteRegister(CC2500_REGISTER_FOCCFG,   0x1D);
  cc2500WriteRegister(CC2500_REGISTER_BSCFG,    0x1C);
  cc2500WriteRegister(CC2500_REGISTER_AGCCTRL2, 0xC7);
  cc2500WriteRegister(CC2500_REGISTER_AGCCTRL1, 0x00);
  cc2500WriteRegister(CC2500_REGISTER_AGCCTRL0, 0xB0);
  cc2500WriteRegister(CC2500_REGISTER_FSCAL3,   0xEA);
  cc2500WriteRegister(CC2500_REGISTER_FSCAL2,   0x0A);
  cc2500WriteRegister(CC2500_REGISTER_FSCAL1,   0x00);
  cc2500WriteRegister(CC2500_REGISTER_FSCAL0,   0x11);
  cc2500WriteRegister(CC2500_REGISTER_FSTEST,   0x59);
  cc2500WriteRegister(CC2500_REGISTER_TEST2,    0x88);
  cc2500WriteRegister(CC2500_REGISTER_TEST1,    0x31);
  cc2500WriteRegister(CC2500_REGISTER_TEST0,    0x0B);
  cc2500WriteRegister(CC2500_REGISTER_FIFOTHR,  0x0f);
  cc2500WriteRegister(CC2500_REGISTER_IOCFG2,   0x29);
  cc2500WriteRegister(CC2500_REGISTER_IOCFG0,   0x06);
  cc2500WriteRegister(CC2500_REGISTER_PKTCTRL1, 0x04);
  cc2500WriteRegister(CC2500_REGISTER_PKTCTRL0, 0x05);
  cc2500WriteRegister(CC2500_REGISTER_ADDR,     0xFF);
  cc2500WriteRegister(CC2500_REGISTER_PKTLEN,   0xFF);
}

char cc2500ReadRegister(char address) {
  cc2500Status = spiByte(CC2500_ADDRESS_READ | address, SPI_TRANSMISSION_START);
  return spiByte(0x00, SPI_TRANSMISSION_END);
}

void cc2500WriteRegister(char address, char value) {
  cc2500Status = spiByte(CC2500_ADDRESS_WRITE | address, SPI_TRANSMISSION_START);
  spiByte(value, SPI_TRANSMISSION_END);
}

void cc2500StrobeCommand(char strobe) {
  cc2500Status = spiByte(CC2500_ADDRESS_WRITE | strobe, SPI_TRANSMISSION_SINGLE);
}

/*void cc2500ReceivePacket(void) {
  long int startTime;

  cc2500ReceiveLength = 0;

  // latch the current time to have a timeout
  startTime = schedulerTime();

  // receive as much of the packet as possible within the timeframe
  while (schedulerTime() - startTime < CC2500_RECEIVE_TIMEOUT) {
    // yield if no bytes are available (avoids reading RXBYTES too much)
    if ((cc2500ReadRegister(CC2500_REGISTER_RXBYTES) & 0x7f) == 0)
      yieldThread();
    else {
      cc2500ReceiveData[cc2500ReceiveLength++] = cc2500ReadRegister(CC2500_DATA_BUFFER);
    }
  }
}*/
void cc2500ReceivePacket(void) {
  long int lastReceivedTime;

  cc2500ReceiveLength = 0;

  // latch the current time to have a timeout
  lastReceivedTime = schedulerTime();

  // receive as much of the packet as possible
  while (schedulerTime() - lastReceivedTime < CC2500_RECEIVE_TIMEOUT) {
    // yield if no bytes are available (avoids reading RXBYTES too much)
    if ((cc2500ReadRegister(CC2500_REGISTER_RXBYTES) & 0x7f) == 0)
      yieldThread();
    else {
      cc2500ReceiveData[cc2500ReceiveLength++] = cc2500ReadRegister(CC2500_DATA_BUFFER);
      lastReceivedTime = schedulerTime();
    }
  }
}

void cc2500TransmitPacket(void) {
  int i;

  // switch to idle mode
  cc2500StrobeCommand(CC2500_STROBE_SIDLE);

  // write the packet length
  cc2500WriteRegister(CC2500_DATA_BUFFER, cc2500TransmitLength + 1);

  // write the packet address
  cc2500WriteRegister(CC2500_DATA_BUFFER, CC2500_DESTINATION_ADDRESS);

  // write the packet
  for (i = 0; i < cc2500TransmitLength; i++)
    cc2500WriteRegister(CC2500_DATA_BUFFER, cc2500TransmitData[i]);
  
  // put the cc2500 in transmit mode
  cc2500StrobeCommand(CC2500_STROBE_STX);

  // wait for the packet to send
  while ((cc2500ReadRegister(CC2500_REGISTER_MARCSTATE) & 0x1f) != 1)
    yieldThread();
  
  // flush the transmit buffer
  cc2500StrobeCommand(CC2500_STROBE_SFTX);

  // switch to receive mode
  cc2500StrobeCommand(CC2500_STROBE_SRX);
}

void cc2500ProcessPacket(void) {
  int i;

  // decide what to do based on the packet type
  switch (cc2500ReceiveData[2]) {
    case 0:
      // the packet is destined for the messenger, pad it with zeros
      for (i = cc2500ReceiveLength - 1; i < 64; i++)
        cc2500ReceiveData[i] = 0x00;

      // write the message to flash memory after setting its sender byte
      cc2500ReceiveData[3] = 0;
      acquireMutex(MUTEX_FLASH);
      flashStoreMessage((Message *)(&(cc2500ReceiveData[3])));
      releaseMutex(MUTEX_FLASH);

      // inform the chat program to redraw
      acquireMutex(MUTEX_CHAT_REDRAW);
      chatRedraw = true;
      releaseMutex(MUTEX_CHAT_REDRAW);
      break;
  }
}

void cc2500SendMessage(Message *message, int length) {
  int i;

  // yield until it is possible to transmit
  while (true) {
    acquireMutex(MUTEX_CC2500_TX);
    if (cc2500Transmit)
      releaseMutex(MUTEX_CC2500_TX);
    else {
      // set the packet type in the transmit buffer 
      cc2500TransmitData[0] = CC2500_PACKET_MESSAGE;

      // copy the message into the transmit buffer after setting its sender byte
      ((char *)message)[0] = true;
      for (i = 0; i < length + 1; i++)
        cc2500TransmitData[i + 1] = ((char *)message)[i];
      
      // pad a bit
      cc2500TransmitData[length + 2] = 0x00;
      cc2500TransmitData[length + 3] = 0x00;
      
      // update the transmit length
      cc2500TransmitLength = length + 4;

      // set the transmit flag to true
      cc2500Transmit = true;
      releaseMutex(MUTEX_CC2500_TX);

      // write the message to flash memory
      acquireMutex(MUTEX_FLASH);
      flashStoreMessage(message);
      releaseMutex(MUTEX_FLASH);

      // inform the chat program to redraw
      acquireMutex(MUTEX_CHAT_REDRAW);
      chatRedraw = true;
      releaseMutex(MUTEX_CHAT_REDRAW);
      return;
    }
  }
}

void cc2500Thread(void) {
  char rxBytes;

  // initialize the cc2500
  initializeCC2500();

  // switch to receive mode
  cc2500StrobeCommand(CC2500_STROBE_SRX);

  while (true) {
    // read the RXBYTES register
    rxBytes = cc2500ReadRegister(CC2500_REGISTER_RXBYTES);

    // determine the action based on rxBytes
    if (rxBytes & 0x80) {
      // a receive overflow has occurred, flush the receive buffer
      cc2500StrobeCommand(CC2500_STROBE_SIDLE);
      cc2500StrobeCommand(CC2500_STROBE_SFRX);
      cc2500StrobeCommand(CC2500_STROBE_SRX);
    } else if (rxBytes & 0x7f) {
      // bytes have been received, receive the packet
      cc2500ReceivePacket();

      // perform an action based on the received packet
      cc2500ProcessPacket();
    } else {
      // check if a packet needs to be sent and send it while disallowing other
      // thread from writing a new packet to the outgoing buffer
      acquireMutex(MUTEX_CC2500_TX);
      if (cc2500Transmit)
        cc2500TransmitPacket();

      cc2500Transmit = false;
      releaseMutex(MUTEX_CC2500_TX);
    }

    // pause for a while
    sleepThread(100);
  }
}
