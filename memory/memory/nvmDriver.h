/**
 * @file nvmDriver.h
 *
 * @brief NVM read/write driver function prototypes
 *
 * This contains the prototypes for the NVM
 * driver and  any macros, constants,
 * or global variables you will need.
 *
 */

#ifndef __NVM_DRIVER__
#define __NVM_DRIVER__

#include <stdio.h>

/* PF: Comment: 
 1. Why use of int instead of uint32_t for the address field ? Unless you support relative addressing please use unsigned types.
 2. Please put in the function description the address lenght and maximum memory size supported by the read function.
 3. The doxygen documentation should denote the direction of parameters, since C doens't offer any protection in case of pointers.
 4. Please standardize error reporting by creating nvmDriverError.h having all the error codes, even if its just one error for failure.
 
 Comments apply on all functions.
*/

/** @brief Read N (byteSize) Bytes from the NVM 
 *         in the address nvmAddress and write them in the Buffer.
 *  @param[out]   buffer      Pointer to an array or Struct to write the bytes from the NVM.
 *  @param[in]    nvmAddress  The address in NVM from which the buffer will read the N (byteSize) Bytes. 
 *  @param[int]   byteSize    The size of bytes which they will be read from the NVM. 
 *  @return                   Read status. 0 for Success and -1 for failure.
 */
int readNvm( void* const buffer, int nvmAddress, int byteSize);

/** @brief Writes  N (byteSize) Bytes from the buffer
 *         in the address nvmAddress of the NVM memory.
 *  @param buffer Array or Struct to write in NVM.
 *  @param nvmAddress The address in NVM to which the buffer will be written.
 *  @param byteSize The size in bytes of the buffer which it will be written in the NVM.
 *  @return Write status. 0 for Success and -1 for failure.
 */
int writeNvm( void* const buffer, int nvmAddress, int byteSize);

#endif /* __NVM_DRIVER__ */
