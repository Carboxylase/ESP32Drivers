#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <stdio.h>

void printMemory(volatile __uint8_t *startAddress,
                    __uint32_t numBytes);

#endif