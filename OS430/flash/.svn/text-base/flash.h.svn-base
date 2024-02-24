#ifndef _FLASH_FLASH_H
#define _FLASH_FLASH_H

#include "config.h"

/* Set Flash Timing Generator
  - Set to MCLK/2 for Flash Timing Generator
*/
void  writeToFlashMemory(char value, int offset);
void  eraseFlashMemory(void);
void  flashFlushCache(void);

#define readFromFlashMemory(offset) (*((char *)((FLASH_BASE_ADDRESS) + offset)))

#endif                                  // _FLASH_FLASH_H
