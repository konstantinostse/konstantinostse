/** @file crcCheck.c
* @brief crc calculation functions implementation 
*        for UInt8_t ,UInt16_t and UInt32_t types.
*
*/
#include "debugFunctions.h"
#include "crcCheck.h"

int crcCheck( void* data, void* crc, size_t sz, UInt8_t dataType )
{
	
	if (uint8PtrType == dataType)
	{
		UInt8_t* crcPtr = (UInt8_t*)crc;
		*crcPtr = 0;
		UInt8_t* dataPtr = (UInt8_t*)data;
		while (sz-- != 0)
		{
			*crcPtr ^= (*dataPtr);
			dataPtr = dataPtr + 1;
			//printf("CRC : crc = %d   data = %d \n", crc, *data);
		}

	}
	else if (uint16PtrType == dataType)
	{
		UInt16_t* crcPtr = (UInt16_t*)crc;
		*crcPtr = 0;
		UInt16_t* dataPtr = (UInt16_t*)data;
		while (sz-- != 0)
		{
			*crcPtr ^= (*dataPtr);
			dataPtr = dataPtr + 1;
			//printf("CRC : crc = %d   data = %d \n", crc, *data);
		}
	}
	else if (uint32PtrType == dataType)
	{
		UInt32_t* crcPtr = (UInt32_t*)crc;
		*crcPtr = 0;
		UInt32_t* dataPtr = (UInt32_t*)data;
		while (sz-- != 0)
		{
			*crcPtr ^= (*dataPtr);
			dataPtr = dataPtr + 1;
			//printf("CRC : crc = %d   data = %d \n", crc, *data);
		}
	}
	else
	{
		/*  Unknown Data type .*/
		return -1;
	}
	/* Succeed to estimate crc.*/
	return 0;
}
