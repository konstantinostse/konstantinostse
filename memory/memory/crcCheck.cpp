/** @file crcCheck.c
* @brief crc calculation functions implementation 
*        for UInt8_t ,UInt16_t and UInt32_t types.
*
*/
#include "debugFunctions.h"
#include "crcCheck.h"

UInt8_t crcCheck(UInt8_t* data, size_t sz)
{
	UInt8_t crc = 0;

	while (sz-- != 0)
	{
		crc ^= (*data);
		data = data + 1;
		//printf("CRC : crc = %d   data = %d \n", crc, *data);
	}

	return crc;
}

UInt16_t crcCheck(UInt16_t* data, size_t sz)
{
	UInt16_t crc = 0;

	while (sz-- != 0)
	{
		crc ^= (*data);
		data = data + 1;
		//printf("CRC : crc = %d   data = %d \n", crc, *data);
	}

	return crc;
}

UInt32_t crcCheck(UInt32_t* data, size_t sz)
{
	UInt32_t crc = 0x00000000;

	while (sz-- != 0)
	{
		crc ^= (*data);
		data = data + 1;
		//printf("CRC : crc =  0x%.8x   data =  0x%.8x \n", crc, *data);
	}

	return crc;
}
