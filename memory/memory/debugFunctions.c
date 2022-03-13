/** @file debugFunctions.c
 *  @brief Functions implementation for debugging.
 *
 */

#include "debugFunctions.h"
#include <stdio.h>
#include <string.h>
#include "attributes.h"

 /** @brief Print the content of an void array(data)
 *          with size lengthe.
 *  @param data Pointer to void type for the array.
 *  @param length Size of the array. 
 *  @return void.
 */
void printData( void* data, UInt8_t length, UInt8_t dataType)
{    
    printf(" data = ");
    if ( uint8PtrType == dataType )
    {
        printf(" ---------------UInt8_t--------------------- \n");
        UInt8_t* dataLocal = (UInt8_t*)data;
        for (int i = 0; i < length; i++)
        {
            printf(" 0x%.2x , ", dataLocal[i]);
        }
    }
    else if (uint16PtrType == dataType)
    {
        printf(" ---------------UInt16_t--------------------- \n");
        UInt16_t* dataLocal = (UInt16_t*)data;
        for (int i = 0; i < length; i++)
        {
            printf(" 0x%.2x , ", dataLocal[i]);
        }
    }
    else if (uint32PtrType == dataType)
    {
        printf(" ---------------UInt32_t--------------------- \n");
        UInt32_t* dataLocal = (UInt32_t*)data;
        for (int i = 0; i < length; i++)
        {
            printf(" 0x%.2x , ", dataLocal[i]);
        }
    }
    else
    {
        return;
    }    
    printf(" \n");
}
