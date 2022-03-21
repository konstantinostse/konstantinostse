/** @file nvmDriver.cpp
 *  @brief NVM read/write driver implementation.
 * 
 */
/* Disable warnings for fopen */
#define _CRT_SECURE_NO_WARNINGS

#include "nvmDriver.h"
#include "nvmDriverError.h"

uint8_t readNvm(void* const buffer, uint32_t nvmAddress, uint32_t byteSize)
{
    FILE* filePtr = NULL;
#if defined(__STDC_VERSION__) && ( __STDC_VERSION__ > 199409L)
    fopen_s(&filePtr, "attDb.bin", "r");
    if (!filePtr)
#else
    if ( NULL == ( filePtr = fopen("attDb.bin", "r")) )
#endif
    {
        printf(" Problem to open file!!!\n");
        return NVM_ACCESS_DENIED_ERROR;
    }    
    if (0 != fseek(filePtr, nvmAddress, SEEK_CUR))
    {
        return NVM_BAD_ADDRESS_ERROR;
    }
#if 1
    fread(buffer, byteSize, 1, filePtr);
#else
    size_t elementsRead = fread(buffer, byteSize, 1, filePtr);
    if (elementsRead != 1)
    {
        return NVM_ACCESS_DENIED_ERROR;
    }
#endif
    if (0 != fclose(filePtr))
    {
        return NVM_UNKNOWN_PROGRAM_ERROR;
    }
    return NVM_NO_ERROR;
}

uint8_t writeNvm( void* const buffer, uint32_t nvmAddress, uint32_t byteSize)
{  
    FILE* filePtr = NULL;
#if defined(__STDC_VERSION__) && ( __STDC_VERSION__ > 199409L)
    fopen_s(&filePtr, "attDb.bin", "r+");
    if (!filePtr)
#else    
    if ( NULL == ( filePtr = fopen("attDb.bin", "r+")) )
#endif
    {
#if defined(__STDC_VERSION__) && ( __STDC_VERSION__ > 199409L)
        fopen_s(&filePtr, "attDb.bin", "w+");
        if (!filePtr)
#else
        if ( NULL == ( filePtr = fopen("attDb.bin", "w+")) )
#endif
        {
            printf(" Problem to open file!!!\n");
            return NVM_ACCESS_DENIED_ERROR;
        }
    }
    if (0 != fseek(filePtr, nvmAddress, SEEK_SET))
    {
        return NVM_ACCESS_DENIED_ERROR;
    }
    size_t elementsWritten = fwrite(buffer, byteSize, 1, filePtr);
    if (elementsWritten != 1)
    {
        return NVM_PROTECTED_BLOCK_ERROR;
    }
    if (0 != fclose(filePtr))
    {
        return NVM_UNKNOWN_PROGRAM_ERROR;
    }
    return NVM_NO_ERROR;
}