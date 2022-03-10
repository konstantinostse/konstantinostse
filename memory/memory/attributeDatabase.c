/**
 * @file attributeDatabase.h
 *
 * @brief Database attributes definitions.
 *
 * This contains the attributes definitions for the Database. 
 *
 * Also it contains the attributes definitions for the
 * testing Database which it will be loaded localy in RAM
 * for testing purposes
 *
 */

#include "attributeDatabase.h"

/*
  Attributes for database  to load localy in RAM for testing 
*/

/* Attribute data to load localy in RAM for testing */
static UInt8_t  att00_data[4]  = { 12,17,53,20};
static UInt8_t  att01_data[5]  = { 2,7,9,0,5 };
static UInt16_t att02_data[5]  = { 0x1112,0x3417,0x5419,0x2320,0x9865 };
static UInt32_t att03_data[5]  = { 0xE5DD1112,0x65EF3417,0x89AD5419,0x22BB2320,0x31AA9865 };
static UInt32_t att04_data[5]  = { 0xE5DD1112,0x65EF3417,0x89AD5419,0x22BB2320,0x31AA9865 };                                 
static UInt16_t att05_data[7]  = { 0x1112,0x3417,0x5419,0x2320,0x9865,0x3333,0x4444};

/* Database ( Array with attributes) to load localy in RAM for testing */
attUIntXdb_t attArray[DATABASE_SIZE] = {
                                        { 0, 2, 4,  att00_data, 0},
                                        { 1, 8, 5,  att01_data, 0},
                                        { 2, 9, 5,  att02_data, 0},
                                        { 3, 3, 5,  att03_data, 0},
                                        { 4, 6, 5, att04_data, 0},
                                        { 5, 3, 7,  att05_data, 0}
                                       };

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

