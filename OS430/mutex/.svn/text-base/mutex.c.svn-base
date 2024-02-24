#include <msp430x14x.h>
#include "include/defaults.h"
#include "scheduler/scheduler.h"
#include "utility/utility.h"
#include "mutex/mutex.h"

char mutexes[(MUTEX_COUNT + 7) / 8];

void initializeMutexes(void) {
  int i;
  
  // erase the mutexes array
  for (i = 0; i < (MUTEX_COUNT + 7) / 8; i++)
    mutexes[i] = 0x00;
}

void acquireMutex(unsigned int mutex) {
  unsigned int statusRegister;
  unsigned int mutexesIndex;
  char mutexBitMask;
  
  // determine which lock to sense
  mutexesIndex = (mutex >> 3) & 0x1fff;
  mutexBitMask = 1 << (mutex & 0x07);
  
  // block while waiting for the lock to become available
  while (true) {
    // disable interrupts
    storeAndDisableInterrupts(&statusRegister);

    // check if the lock is available
    if (!(mutexes[mutexesIndex] & mutexBitMask)) {
      // lock it and return
      mutexes[mutexesIndex] |= mutexBitMask;
      restoreInterrupts(&statusRegister);
      return;
    } else {
      // restore interrupts and block
      restoreInterrupts(&statusRegister);
      yieldThread();
    }
  }
}

void releaseMutex(unsigned int mutex) {
  unsigned int statusRegister;
  unsigned int mutexesIndex;
  char mutexBitMask;
  
  // determine which lock to sense
  mutexesIndex = (mutex >> 3) & 0x7fff;
  mutexBitMask = ~(1 << (mutex & 0x07));
  
  // disable interrupts
  storeAndDisableInterrupts(&statusRegister);
  
  // unlock the mutex
  mutexes[mutexesIndex] &= mutexBitMask;
  
  // restore interrupts
  restoreInterrupts(&statusRegister);

  yieldThread();
}
