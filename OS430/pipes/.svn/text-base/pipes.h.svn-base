#ifndef _PIPES_PIPES_H
#define _PIPES_PIPES_H

#include "include/defaults.h"

typedef struct {
  unsigned int readIndex, writeIndex;
  int readSize, writeSize;
  unsigned int size;
} Pipe;

Pipe *createPipe(unsigned int size);
void destroyPipe(Pipe *pipe);
unsigned int readPipe(Pipe *pipe, char *buffer, unsigned int length, bool block);
unsigned int writePipe(Pipe *pipe, const char *buffer, unsigned int length, bool block);
unsigned int readPipeByte(Pipe *pipe, char *byte, bool block);
unsigned int writePipeByte(Pipe *pipe, const char *byte, bool block);
unsigned int readPipeByteIsr(Pipe *pipe, char *byte);
unsigned int writePipeByteIsr(Pipe *pipe, const char *byte);
int getPipeReadSize(Pipe *pipe);
int getPipeWriteSize(Pipe *pipe);

#endif
