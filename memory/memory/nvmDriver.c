/** @file nvmDriver.cpp
 *  @brief NVM read/write driver implementation.
 * 
 */

#include "nvmDriver.h"


int readNvm(void* const buffer, uint32_t nvmAddress, uint32_t byteSize)
{
    FILE* filePtr = NULL;
    fopen_s(&filePtr, "attDb.bin", "r");

    if (!filePtr)
    {        
        printf(" Problem to open file!!!");
        return -1;        
    }
    fseek(filePtr, nvmAddress, SEEK_CUR);
    fread(buffer, byteSize, 1, filePtr);

    fclose(filePtr);
    return 0;
}

int writeNvm( void* const buffer, uint32_t nvmAddress, uint32_t byteSize)
{
    FILE* filePtr = NULL;
    fopen_s(&filePtr, "attDb.bin", "r+");

    if (!filePtr)
    {
        fopen_s(&filePtr, "attDb.bin", "w+");
        if (!filePtr)
        {
            printf(" Problem to open file!!!");
            return -1;
        }
    }
    fseek(filePtr, nvmAddress,SEEK_SET);
    fwrite(buffer, byteSize, 1, filePtr);

    fclose(filePtr);
    return 0;
}