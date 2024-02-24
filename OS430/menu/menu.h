#ifndef _MENU_MENU_H
#define _MENU_MENU_H

void menuThread(void);
void menuDisplay(const char *menuString, int length);
int menuGetInteger(void);
void menuRealtimeClock(void);
void menuCounter(void);
void menuTemperature(void);
void menuChat(void);

#endif                                  // _MENU_MENU_H
