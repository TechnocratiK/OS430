#include "include/defaults.h"
#include "message.h"
#include "flash/flash.h"
#include "memory/memory.h"
#include "config.h"

char flashMemoryCache[256];

void initializeFlash(void) {
  int i;

  for (i = 0; i < 256; i++)
    flashMemoryCache[i] = readFromFlashMemory(i);
}

Message *flashGetMessage(int age) {
  int newestMessage;

  // ensure that age is between 0 and MESSAGE_MAX_AGE
  if (age < 0 || age > MESSAGE_MAX_AGE)
    return null;
   
  // get the newest message from flash memory
  newestMessage = (((int)readFromFlashMemory(1)) << 8) | (((int)readFromFlashMemory(0)) & 0x00ff);

  if (newestMessage - age < 0)
    newestMessage += MESSAGE_MAX_AGE + 1;
  
  return (Message *)(((char *)FLASH_BASE_ADDRESS) + (newestMessage - age) * FLASH_BLOCK_SIZE + 2);
}

void flashStoreMessage(Message * msg) {
  int newestMessage;
  int flashMessageOffset, i;

  // get the newest message from flash memory
  newestMessage = (((int)readFromFlashMemory(1)) << 8) | (((int)readFromFlashMemory(0)) & 0x00ff);

  // update the message pointer
  newestMessage = (newestMessage >= 4) ? 0 : newestMessage + 1;

  // write the message pointer
  flashMemoryCache[0] = (char)(newestMessage & 0x00ff);
  flashMemoryCache[1] = (char)((newestMessage >> 8) & 0x00ff);

  // write the message
  flashMessageOffset = newestMessage * FLASH_BLOCK_SIZE + 2;
  for (i = 0; i < FLASH_BLOCK_SIZE; i++)
    flashMemoryCache[flashMessageOffset + i] = ((char *)msg)[i];

  // update the flash memory
  flashFlushCache();
}

void flashFlushCache(void) {
  int i;

  // erase both flash segments
  eraseFlashMemory();

  // perform the writes
  for (i = 0; i < 256; i++)
    writeToFlashMemory(flashMemoryCache[i], i);
}
