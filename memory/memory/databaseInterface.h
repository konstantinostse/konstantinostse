/**
 * @file databaseInterface.h
 *
 * @brief Database interface function prototypes.
 * 
 * Functions prototypes for interfacing with the Attributes Database. 
 */

#ifndef __NVM_DB_INTERFACE__
#define __NVM_DB_INTERFACE__

#include "attributeDatabase.h"

/** @brief Free attUIntX_inst pointer (Heap Database) from the heap.
*          This function deallocate resources
*          in the Heap memory which they were allocated for the heap Database.
*  @return 0 for Success and -1 for failure.
*/
int unloadDatabaseFromHeap();

/** @brief Write Database from Heap memory (attUIntX_t)
*          to the NVM.
*  @return 0 for Success and -1 for failure.
*/
int writeDatabaseFromHeapToNvm();

/** @brief Read Database from the NVM to Heap memory (attUIntX_t)
*  @return 0 for Success and -1 for failure.
*/
int readDatabaseFromNvmToHeap();

/** @brief Read the data from an attribute from the nvm.
 *         In order to use this function the database 
 *         should be loaded both in the nvm and in the heap.
 *  @param[in]   attrId  Attribute ID in the database.
 *  @param[out]  pLenght Size of the attributes data field.
 *                       Size is in bytes and it is an output in this function.
 *  @param[out]  pValue Pointer to array with the attrinute's data.
 *                       It supports UInt8_t , UInt16_t and UInt32_t types.
 *  @return              0 for Success and -1 for failure.
 */
gpNvm_Result gpNvm_getAttribute(gpNvm_AttrId attrId, UInt8_t* pLenght, void* pValue);

/** @brief Writes an attribute in the NVM Database.
 *         In order to use this function , the database
 *         should be loaded both in the NVM and in the heap.
 *  @param[in]   attrId  Attribute ID in the database.
 *  @param[in]   pLenght Size of the data for writing in NVM.It should not be bigger
 *                       from the maximum data lenght of the attribute.
 *                       Size should be in bytes.
 *  @param[in]   pValue  Pointer to Array with the attribute's data to 
 *                       be written in the nvm Database.It supports UInt8_t ,
 *                       UInt16_t and UInt32_t types.
 *  @return              0 for Success and -1 for failure.
 */
gpNvm_Result gpNvm_setAttribute(gpNvm_AttrId attrId, UInt8_t pLenght, void* pValue);

#endif /* __NVM_DB_INTERFACE__ */

