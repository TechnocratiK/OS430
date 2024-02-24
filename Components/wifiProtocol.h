#ifndef __INCLUDE_WIFIPROTOCOL_H
#define __INCLUDE_WIFIPROTOCOL_H


extern int RECEIVED[4];
extern int NEWPACKET;



void sendBeacon(void);
int readRxBuf(void);

#endif


