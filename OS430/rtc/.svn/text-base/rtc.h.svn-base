#ifndef _RTC_RTC_H
#define _RTC_RTC_H

#define RTC_PIN_CLK 0x08
#define RTC_PIN_DATA 0x04
#define RTC_PIN_IO 0x02
#define RTC_PIN_CE 0x01

#define RTC_PORT_SEL P3SEL
#define RTC_PORT_DIR P3DIR
#define RTC_PORT_IN P3IN
#define RTC_PORT_OUT P3OUT

typedef struct {
  char year;
  char month;
  char day;
  char weekday;
  char hour;
  char minute;
  char second;
} RTCTime;

void rtcInit(void);
void rtcRxMode(void);
void rtcTxMode(void);

void rtcReadTime(RTCTime *systemTime);
void rtcWriteTime(RTCTime *systemTime);

char rtcReadByte(void);
void rtcWriteByte(char byte);
char rtcReadNibble(void);
void rtcWriteNibble(char nibble);

char charToBcd(char x);
char bcdToChar(char x);

#endif                                  // _RTC_RTC_H
