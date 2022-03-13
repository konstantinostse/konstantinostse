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
#include <stdint.h>

/** @brief Read N(byteSize) Bytes from the NVM
 *         in the address nvmAddress and write them in the Buffer.
 *  @param[out]   buffer      Pointer to an array or Struct to read the bytes from the NVM.
 *  @param[in]    nvmAddress  The address in NVM from which the buffer will read the N(byteSize) Bytes.
 *  @param[in]    byteSize    The size of bytes which they will be read from the NVM.
 *  @return                   Read status. 0 for Success and -1 for failure.
 */
int readNvm( void* const buffer, uint32_t nvmAddress, uint32_t byteSize);

/** @brief Writes N(byteSize) Bytes from the buffer
 *          in the address nvmAddress of the NVM memory.
 *  @param[out]   buffer      Pointer to an array or Struct to write bytes in the NVM.
 *  @param[in]    nvmAddress  The address in NVM to which the buffer will be written.
 *  @param[in]    byteSize    The size in bytes of the buffer which it will be written in the NVM.
 *  @return                   Write status. 0 for Success and -1 for failure.
 */
int writeNvm( void* const buffer, uint32_t nvmAddress, uint32_t byteSize);

#endif /* __NVM_DRIVER__ */
