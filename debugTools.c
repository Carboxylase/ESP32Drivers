#include "include/debugTools.h"
#include <inttypes.h>

void printMemory(__uint32_t *startAddress,
                    __uint32_t numBytes)
{

    __uint32_t numColumns = 8;
    __uint32_t numRows = numBytes / numColumns;

    for (int row = 0; row < numRows; row++)
    {
        printf("%p ", (startAddress + (row * numColumns)));
        for (int col = 0; col < numColumns; col++)
        {
            printf("%lu ",*(startAddress + row*numColumns + col));
        }
    }

}