#include <msp430x14x.h>
#include "flash/flash.h"
#include "utility/utility.h"

//#define FLASH_BASE_ADDRESS 0x1000

// write one byte to flash memory
void writeToFlashMemory(char value, int offset) {
  unsigned int statusRegister;
  char *flashPointer;                   // Flash pointer

  // disable interrupts
  storeAndDisableInterrupts(&statusRegister);

  flashPointer = (char *)(FLASH_BASE_ADDRESS + offset);

  // unlock the flash for writing
  FCTL3 = FWKEY;
  FCTL1 = FWKEY + ERASE;
  // set WRT bit for write operation
  FCTL1 = FWKEY + WRT; // Set WRT bit for write operation
  *flashPointer = value;
  // clear the WRT bit and lock the flash
  FCTL1 = FWKEY;
  FCTL3 = FWKEY + LOCK;

  // restore interrupts
  restoreInterrupts(&statusRegister);
}
