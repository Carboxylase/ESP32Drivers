#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

#define MAX_1_BIT 1
#define MAX_2_BIT 3
#define MAX_3_BIT 7
#define MAX_4_BIT 15
#define MAX_5_BIT 31
#define MAX_6_BIT 63
#define MAX_7_BIT 127
#define MAX_8_BIT 255
#define MAX_9_BIT 511
#define MAX_10_BIT 1023
#define MAX_11_BIT 2047
#define MAX_12_BIT 4095
#define MAX_13_BIT 8191
#define MAX_14_BIT 16383
#define MAX_15_BIT 32767
#define MAX_16_BIT 65535

typedef enum
{
    FALSE,
    TRUE
}bool_t;

void clearAndSetBits(volatile __uint32_t *reg,
                        __uint8_t bitShift,
                        __uint32_t val);

void clearBits(volatile __uint32_t *reg,
                __uint8_t bitShift);

void setBits(volatile __uint32_t *reg,
            __uint8_t bitShift);

#endif //UTILITY_H