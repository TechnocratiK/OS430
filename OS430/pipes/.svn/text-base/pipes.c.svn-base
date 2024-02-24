#include <msp430x14x.h>

#include "include/defaults.h"
#include "memory/memory.h"
#include "scheduler/scheduler.h"
#include "utility/utility.h"
#include "pipes/pipes.h"

Pipe *createPipe(unsigned int size) {
  Pipe *result;

  // attempt to allocate memory for the pipe
  if (result = (Pipe *)allocateMemory(sizeof(Pipe) + size * sizeof(char))) {
    // the allocation was successful, setup the pipe
    result->readIndex = 0;
    result->writeIndex = 0;
    result->readSize = 0;
    result->writeSize = size;
    result->size = size;
  } 
  
  return result;
}

void destroyPipe(Pipe *pipe) {
  freeMemory(pipe, pipe->size + sizeof(Pipe));
}

unsigned int readPipe(Pipe *pipe, char *buffer, unsigned int length, bool block) {
  unsigned int bytesToCopy, statusRegister;

  // check for stupid input
  if (length > pipe->size)
    return 0;
  
  // make sure the read operation is feasible
  if (block) {
    // reduce the pipe's read size
    storeAndDisableInterrupts(&statusRegister);
    pipe->readSize -= length;

    // loop, blocking until the pipe has sufficient free space
    while (true) {
      if (pipe->readSize < 0) {
        restoreInterrupts(&statusRegister);
        yieldThread();
        storeAndDisableInterrupts(&statusRegister);
       } else {
        restoreInterrupts(&statusRegister);
        break;
      }
    }
  } else {
    // disable interrupts to check the length of the pipe's data
    storeAndDisableInterrupts(&statusRegister);
    if (pipe->readSize < length) {
      // a read isn't possible, return 0
      restoreInterrupts(&statusRegister);
      return 0;
    } else {
      // a read is possible
      pipe->readSize -= length;
      restoreInterrupts(&statusRegister);
    }
  }

  // read the data into the buffer, copying up to the end of the pipe
  bytesToCopy = length;
  while (pipe->readIndex < pipe->size && bytesToCopy > 0) {
    *buffer++ = ((char *)pipe + sizeof(Pipe))[pipe->readIndex++];
    bytesToCopy--;
  }

  // adjust the read index as needed
  if (!(pipe->readIndex < pipe->size))
    pipe->readIndex = 0;

  // read the data into the buffer, wrapping around with anything that remains
  while (bytesToCopy > 0) {
    *buffer++ = ((char *)pipe + sizeof(Pipe))[pipe->readIndex++];
    bytesToCopy--;
  }

  // update the write size
  storeAndDisableInterrupts(&statusRegister);
  pipe->writeSize += length;
  restoreInterrupts(&statusRegister);

  return length;
}

unsigned int writePipe(Pipe *pipe, const char *buffer, unsigned int length, bool block) {
  unsigned int bytesToCopy, statusRegister;

  // check for stupid input
  if (length > pipe->size)
    return 0;
  
  // make sure the write operation is feasible
  if (block) {
    // reduce the pipe's write size
    storeAndDisableInterrupts(&statusRegister);
    pipe->writeSize -= length;

    // loop, blocking until the pipe has sufficient free space
    while (true) {
      if (pipe->writeSize < 0) {
        restoreInterrupts(&statusRegister);
        yieldThread();
        storeAndDisableInterrupts(&statusRegister);
      } else {
        restoreInterrupts(&statusRegister);
        break;
      }
    }
  } else {
    // disable interrupts to check the space available in the pipe
    storeAndDisableInterrupts(&statusRegister);
    if (pipe->writeSize < length) {
      // a write isn't possible, return 0
      restoreInterrupts(&statusRegister);
      return 0;
    } else {
      // a write is possible
      pipe->writeSize -= length;
      restoreInterrupts(&statusRegister);
    }
  }

  // read the data out of the buffer, copying up to the end of the pipe
  bytesToCopy = length;
  while (pipe->writeIndex < pipe->size && bytesToCopy > 0) {
    ((char *)pipe + sizeof(Pipe))[pipe->writeIndex++] = *buffer++;
    bytesToCopy--;
  }

  // adjust the write index as needed
  if (!(pipe->writeIndex < pipe->size))
    pipe->writeIndex = 0;

  // read the data out of the buffer, wrapping around with anything that remains
  while (bytesToCopy > 0) {
    ((char *)pipe + sizeof(Pipe))[pipe->writeIndex++] = *buffer++;
    bytesToCopy--;
  }

  // update the read size
  storeAndDisableInterrupts(&statusRegister);
  pipe->readSize += length;
  restoreInterrupts(&statusRegister);

  return length;
}

unsigned int readPipeByte(Pipe *pipe, char *byte, bool block) {
  unsigned int statusRegister;

  // disable interrupts
  storeAndDisableInterrupts(&statusRegister);

  if (block) {
    // decrement the read size
    pipe->readSize--;

    // wait for data to become availeble
    while (pipe->readSize < 0) {
      restoreInterrupts(&statusRegister);
      yieldThread();
      storeAndDisableInterrupts(&statusRegister);
    }
  } else if (pipe->readSize > 0) {
    // decrement the read size
    pipe->readSize--;
  } else {
    // restore interrupts and return a failure
    restoreInterrupts(&statusRegister);
    return 0;
  }

  // read the byte
  *byte = *((char *)pipe + sizeof(Pipe) + pipe->readIndex);

  // adjust the pipe parameters
  pipe->writeSize++;
  if (++(pipe->readIndex) == pipe->size)
    pipe->readIndex = 0;
  
  // restore interrupts and return a success
  restoreInterrupts(&statusRegister);
  return 1;
}

unsigned int writePipeByte(Pipe *pipe, const char *byte, bool block) {
  unsigned int statusRegister;

  // disable interrupts
  storeAndDisableInterrupts(&statusRegister);

  if (block) {
    // decrement the write size
    pipe->writeSize--;

    // wait for space to become availeble
    while (pipe->writeSize < 0) {
      restoreInterrupts(&statusRegister);
      yieldThread();
      storeAndDisableInterrupts(&statusRegister);
    }
  } else if (pipe->writeSize > 0) {
    // decrement the write size
    pipe->writeSize--;
  } else {
    // restore interrupts and return a failure
    restoreInterrupts(&statusRegister);
    return 0;
  }

  // write the byte
  *((char *)pipe + sizeof(Pipe) + pipe->writeIndex) = *byte;

  // adjust the pipe parameters
  pipe->readSize++;
  if (++(pipe->writeIndex) == pipe->size)
    pipe->writeIndex = 0;
  
  // restore interrupts and return a success
  restoreInterrupts(&statusRegister);
  return 1;
}

unsigned int readPipeByteIsr(Pipe *pipe, char *byte) {
  if (pipe->readSize > 0) {
    // read the byte
    *byte = *((char *)pipe + sizeof(Pipe) + pipe->readIndex);

    // adjust the read index
    if (++(pipe->readIndex) == pipe->size)
      pipe->readIndex = 0;

    // adjust the sizes
    pipe->readSize--;
    pipe->writeSize++;

    // return a success
    return 1;
  } else
    // return a failure
    return 0;
}

unsigned int writePipeByteIsr(Pipe *pipe, const char *byte) {
  if (pipe->writeSize > 0) {
    // write the byte
    *((char *)pipe + sizeof(Pipe) + pipe->writeIndex) = *byte;

    // adjust the write index
    if (++(pipe->writeIndex) == pipe->size)
      pipe->writeIndex = 0;

    // adjust the sizes
    pipe->readSize++;
    pipe->writeSize--;

    // return a success
    return 1;
  } else
    // return a failure
    return 0;
}

int getPipeReadSize(Pipe *pipe) {
  unsigned int statusRegister;
  int result;

  // disable interrupts
  storeAndDisableInterrupts(&statusRegister);

  // read the result
  result = pipe->readSize;

  // restore interrupts and return
  restoreInterrupts(&statusRegister);

  return result;
}

int getPipeWriteSize(Pipe *pipe) {
  unsigned int statusRegister;
  int result;

  // disable interrupts
  storeAndDisableInterrupts(&statusRegister);

  // read the result
  result = pipe->writeSize;

  // restore interrupts and return
  restoreInterrupts(&statusRegister);

  return result;
}
