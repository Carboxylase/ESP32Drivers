#include "include/debugTools.h"

void printMemory(volatile __uint8_t *startAddress,
                    __uint32_t numBytes)
{

    printf("Printing memory...\n");
    __uint32_t numColumns = 4;
    __uint32_t numRows = numBytes / numColumns;
    if (numBytes%numColumns > 0)
    {
        numRows++;
    }

    volatile __uint8_t *byteToPrint = startAddress; // try using this instead

    for (int row = 0; row < numRows; row++)
    {
        // printf("%5p", (void*)(startAddress + (row * numColumns)));
        printf("%5p", (void*)byteToPrint);
        for (int col = 0; col < numColumns; col++)
        {
            // printf("%5lX ",*(__uint32_t*)(startAddress + row*numColumns + col));
            printf("%5hhX", *byteToPrint);
            if (row == (numRows-1) && col == ((numBytes%numColumns)-1))
            {
                printf("\n");
                return;
            }
            byteToPrint ++;
        }

        printf("\n");
    }

}