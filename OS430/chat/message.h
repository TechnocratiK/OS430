#ifndef __INCLUDE_MESSAGE_H
#define __INCLUDE_MESSAGE_H

typedef struct {
  char sender;
  char message[49];
} Message;

void initializeFlash(void);
Message *flashGetMessage(int age);
void flashStoreMessage(Message * msg);


#endif
