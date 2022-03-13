/**
 * @file debugFunctions.h
 *
 * @brief
 */

#ifndef  __DEBUG_FUNCTIONS__
#define __DEBUG_FUNCTIONS__

#include <stdint.h>
#include <stdio.h>
#include "attributes.h"

 /** @brief Print the data for a UInt8_t ,UInt16_t or
  *         UInt32_t array(data) with size length and return the crc value.
  *  @param[in]    data      Void pointer to array with the Data.
  *  @param[in]    length    Size of the array.
  *  @param[in]    dataType  Data type of the array.
  *                          uint8PtrType for pointer to UInt8_t.
  *                          uint16PtrType for pointer to UInt16_t.
  *                          uint32PtrType for pointer to UInt32_t.
  *  @return                 crc calculation Status. 0 for success and -1 for failure.
  */
void printData(void* data, UInt8_t length, UInt8_t dataType);

#endif /* __DEBUG_FUNCTIONS__ */

