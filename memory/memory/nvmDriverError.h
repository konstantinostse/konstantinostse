/**
 * @file nvmDriverError.h
 *
 * @brief NVM error codes 
 *        nvm is simulated with file so the
 *        error codes  are matched with the error codes in files.
 */

#ifndef __NVM_DRIVER_ERROR__
#define __NVM_DRIVER_ERROR__

#define  NVM_NO_ERROR                0x00      /* No error */
#define  NVM_PROGRAM_ERROR           0x01      /* Program error */
#define  NVM_PROTECTED_BLOCK_ERROR   0x02      /* Memory block protection */
#define  NVM_UNKNOWN_PROGRAM_ERROR   0x04      /* Unknown error */
#define  NVM_ACCESS_DENIED_ERROR     0x08      /* Permission denied */
#define  NVM_OUT_OF_MEMORY_ERROR     0x10      /* Out of memory */
#define  NVM_BAD_ADDRESS_ERROR       0x02      /* Bad address */

#endif /* __NVM_DRIVER_ERROR__ */