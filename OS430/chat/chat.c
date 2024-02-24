#include "include/defaults.h"
#include "cc2500/cc2500.h"
#include "chat/chat.h"
#include "chat/message.h"
#include "terminal/terminal.h"

bool chatRedraw;
Message typingMessage;
static const char chatPrompt[] = "New message here: ";
static const char sentByYou[] = "You say: ";
static const char sentByThem[] = "Your friend says: ";
static const char crlf[] = "\r\n";
unsigned int typingPos = 0;

void chat(void) {
  char newChar, bell;
  bool redrawNeeded;
  
  // initialze
  typingMessage.sender = 0;
  acquireMutex(MUTEX_CHAT_REDRAW);
  chatRedraw = true;
  releaseMutex(MUTEX_CHAT_REDRAW);
  bell = 0x07;

  while(true) {
    acquireMutex(MUTEX_CHAT_REDRAW);
    redrawNeeded = chatRedraw;
    chatRedraw = false;
    releaseMutex(MUTEX_CHAT_REDRAW);

    if(redrawNeeded)
      chatDoRedraw();

    if (terminalReadByte(&newChar, false)) {
      if (newChar == 0x0d) {
        typingMessage.message[typingPos] = NULL;
        cc2500SendMessage(&typingMessage, typingPos);
        typingPos = 0;
      } else if (newChar == 0x1b) {
        terminalBurstWrite(crlf, 2, true);
        terminalBurstWrite(crlf, 2, true);
        return;
      } else if (newChar == 0x7f) {
        if (typingPos == 0) {
          terminalBell();
        } else {
          typingMessage.message[--typingPos] = NULL;
          terminalBackspace();
        }
      } else if (typingPos == 48){
        typingMessage.message[typingPos] = NULL;
        terminalBell();
      } else {
        typingMessage.message[typingPos++] = newChar;
        terminalWriteByte(&newChar, true);
      }
    }
  }
}

void chatDoRedraw(void) {
  Message *currentMessage;
  int i, len;
  terminalClear();
  terminalSetCursorPosition(1, 1);

  acquireMutex(MUTEX_FLASH);
  for (i = 4; i >= 0; i--) {
    currentMessage = flashGetMessage(i);
    if (currentMessage->sender) {
      terminalSetForegroundColor(TERMINAL_COLOR_RED);
      terminalBurstWrite(sentByYou, sizeof(sentByYou), true);
    } else {
      terminalSetForegroundColor(TERMINAL_COLOR_BLUE);
      terminalBurstWrite(sentByThem, sizeof(sentByThem), true);
    }

    // determine the length of the message
    len = -1;
    while (currentMessage->message[++len]);

    // write the message to the terminal
    terminalBurstWrite(currentMessage->message, len, true);
    terminalBurstWrite(crlf, 2, true);

    // if the message is the latest, write it to the LCD
    if (i == 0) {
      lcd_clearscreen();
      lcd_printf(currentMessage->message);
    }
  }
  releaseMutex(MUTEX_FLASH);

  // draw the prompt
  terminalSetForegroundColor(TERMINAL_COLOR_WHITE);
  terminalSetCursorPosition(6, 1);
  terminalBurstWrite(chatPrompt, 18, true);

  // draw the current message
  terminalBurstWrite(typingMessage.message, typingPos, true);
}
