#ifndef __INCLUDE_GAME_H
#define __INCLUDE_GAME_H


// Common states
#define PRINT_INIT 5
#define HOST_OR_JOIN 8

// Host states
#define SEND_HOST_BEACON 10
#define DO_YOU_ACCEPT 11
#define SET_UP_HEARTBEATS 12

// Client states
#define LISTEN_FOR_BEACON 20
#define ACCEPT_BEACON 21
#define WAIT_HOST_HB 22

// Game states
#define INIT_BOARD 31
#define SET_NUMBER 32
#define SEND_DATA 33
#define GRAB_INPUT 34



struct packet{
       int length;
       int dest_addy; 
       int src_addy;
       int type;
       char* data;
};


extern int KEYPRESSED;
extern int KEY[5];
extern int NUMBER[4];
extern int RECEIVED[4];
extern int NEWPACKET;





void resetKeyBuf(void);
void readRx(void);
void playGame(void);

#endif
