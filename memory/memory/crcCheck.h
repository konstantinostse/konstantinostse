/**
 * @file crcCheck.h
 *
 * @brief CRC calculation functions prototypes
 */

#ifndef __CRC_CHECK_H__
#define __CRC_CHECK_H__

#include <stdint.h>
#include "attributes.h"

/** @brief Calculate the crc for an UInt8_t array(data) 
*          with size sz and return the crc value.
*  @param data Pointer to UInt8_t type for the array.
*  @param sz Size of the array.
*  @return crc value type UInt8_t.
*/
UInt8_t crcCheck( UInt8_t* data, size_t sz);

/** @brief Calculate the crc for an UInt16_t array(data)
*          with size sz and return the crc value.
*  @param data Pointer to UInt8_t type for the array.
*  @param sz Size of the array.
*  @return crc value type UInt16_t.
*/
UInt16_t crcCheck( UInt16_t* data, size_t sz);

/** @brief Calculate the crc for an UInt32_t array(data)
*          with size sz and return the crc value.
*  @param data Pointer to UInt8_t type for the array.
*  @param sz Size of the array.
*  @return crc value type UInt32_t.
*/
UInt32_t crcCheck( UInt32_t* data, size_t sz);

#endif /* __CRC_CHECK_H__ */