/**
 * @file debugFunctions.h
 *
 * @brief
 */

#if !defined(__DEBUG_FUNCTIONS__) && !defined(__cplusplus)
#define __DEBUG_FUNCTIONS__

#include <stdint.h>
#include <stdio.h>
#include "attributes.h"
#include <stdarg.h>

#define OVERLOAD_PRINT_DATA

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

void printData(void* data, UInt8_t length, UInt8_t dataType  ); 

#ifdef OVERLOAD_PRINT_DATA 
#define __FILENAME__ (strrchr(__FILE__, 92) ? strrchr(__FILE__, 92) + 1 : __FILE__) /* Ascii '/' -> 92 dec */
void printDataFileFunctionLine( void* data, UInt8_t length, UInt8_t dataType , const char *fileName , const char *functionName, UInt32_t line );
#define printData(...)  printDataFileFunctionLine( __VA_ARGS__ ,__FILENAME__, __func__ , __LINE__ )
#endif /* OVERLOAD_PRINT_DATA */


#endif /* __DEBUG_FUNCTIONS__ */

