/** @file debugFunctions.c
 *  @brief Functions implementation for debugging.
 *
 */

#include "debugFunctions.h"
#include <stdio.h>
#include <string.h>

 /** @brief Print the content of an UInt8_t array(data)
 *          with size lengthe.
 *  @param data Pointer to UInt8_t type for the array.
 *  @param length Size of the array. 
 */
void printData(UInt8_t* data, UInt8_t length)
{
	printf(" ---------------UInt8_t--------------------- \n");
	printf(" data = ");
	for (int i = 0; i < length; i++)
	{
        printf(" 0x%.2x , ", data[i] );
	}
	printf(" \n");
}
/** @brief Print the content of an UInt16_t array(data)
 *          with size lengthe.
 *  @param data Pointer to UInt8_t type for the array.
 *  @param length Size of the array.
 */
void printData(UInt16_t* data, UInt8_t length)
{
	printf(" ----------------UInt16_t-------------------- \n");
	printf(" data = ");
	for (int i = 0; i < length; i++)
	{
		printf(" 0x%.4x , ", data[i]);
	}
	printf(" \n");

}
/** @brief Print the content of an UInt32_t array(data)
 *          with size lengthe.
 *  @param data Pointer to UInt8_t type for the array.
 *  @param length Size of the array.
 */
void printData(UInt32_t* data, UInt8_t length)
{
	printf(" ----------------UInt32_t-------------------- \n");
	printf(" data = ");
	for (int i = 0; i < length; i++)
	{
		printf(" 0x%.8x , ", data[i]);
	}
	printf(" \n");

}
