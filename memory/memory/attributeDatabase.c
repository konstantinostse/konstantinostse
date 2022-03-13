/**
 * @file attributeDatabase.h
 *
 * @brief Database attributes definitions.
 *
 * This contains the attributes definitions for the Database.   
 *
 */

#include "attributeDatabase.h"

/* Flag to check if database for testing  is loaded localy in Ram */
bool isDatabaseLoadedInRam = false;

/* Flag to check if database for testing  is loaded localy in NVM */
bool isDatabaseLoadedInNvm = false;

/* Double Pointer for loading all the database localy to RAM ( Heap) */
attUIntX_t** attUIntX_inst;

/*
    Database 's lookup tables
*/

/* Database lookup table for attributes size */
UInt32_t    attributeSize[DATABASE_SIZE]    = { 0 };
/* Database lookup table for attributes byte offset in memory */
UInt8_t     attributeOffset[DATABASE_SIZE]  = { 0 };
/* Database lookup table for attributes types */
UInt8_t     attributeType[DATABASE_SIZE]    = { uint8PtrType, uint8PtrType , uint16PtrType , uint32PtrType,uint32PtrType,uint16PtrType };
