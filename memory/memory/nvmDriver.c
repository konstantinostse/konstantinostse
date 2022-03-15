/** @file nvmDriver.cpp
 *  @brief NVM read/write driver implementation.
 * 
 */

#include "nvmDriver.h"
#include "nvmDriverError.h"

uint8_t readNvm(void* const buffer, uint32_t nvmAddress, uint32_t byteSize)
{    
    FILE* filePtr = NULL;
    if (0 != fopen_s(&filePtr, "attDb.bin", "r"))
    {
        printf(" Problem to open file!!!\n");
        return NVM_ACCESS_DENIED_ERROR;
    }
    if (!filePtr)
    {        
        printf(" Problem to open file!!!\n");
        return NVM_ACCESS_DENIED_ERROR;
    }
    if (0 != fseek(filePtr, nvmAddress, SEEK_CUR))
    {
        return NVM_BAD_ADDRESS_ERROR;
    }
    if (0 != fread(buffer, byteSize, 1, filePtr))
    {
        return NVM_ACCESS_DENIED_ERROR;
    }
    if (0 != fclose(filePtr))
    {
        return NVM_UNKNOWN_PROGRAM_ERROR;
    }
    return NVM_NO_ERROR;
}

uint8_t writeNvm( void* const buffer, uint32_t nvmAddress, uint32_t byteSize)
{  
    FILE* filePtr = NULL;
    fopen_s(&filePtr, "attDb.bin", "r+");
    if (!filePtr)
    {
        fopen_s(&filePtr, "attDb.bin", "w+");
        if (!filePtr)
        {
            printf(" Problem to open file!!!\n");
            return NVM_ACCESS_DENIED_ERROR;
        }
    }
    if (0 != fseek(filePtr, nvmAddress, SEEK_SET))
    {
        return NVM_ACCESS_DENIED_ERROR;
    }
    if (0 != fwrite(buffer, byteSize, 1, filePtr))
    {
        return NVM_PROTECTED_BLOCK_ERROR;
    }
    if (0 != fclose(filePtr))
    {
        return NVM_UNKNOWN_PROGRAM_ERROR;
    }
    return NVM_NO_ERROR;
}