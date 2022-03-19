/**
* @file databaseTestHelper.h
*
* @brief Database attributes declarations.
*
* This contains the attributes declarations for the Database
* and  any macros, constants or global variables you will need.
*
* Also it contains the attributes declarations for the
* testing Database which it will be loaded localy in RAM
* for testing purposes
*
*/

#if !defined(__DATABASE_TEST_HELPER__) && !defined(__cplusplus)
#define __DATABASE_TEST_HELPER__

#include <stdbool.h>
#include "attributes.h"

#define DATABASE_SIZE 6


/* Database 's structure type definition for loading database in the Stack for testing */
typedef struct attUIntXdb
{
    gpNvm_AttrId id;
    UInt32_t     option;
    UInt8_t      length;
    void*        data;
    UInt32_t     crc;
}attUIntXdb_t;

/* Database to load localy in the Stack for testing */
extern attUIntXdb_t attArray[DATABASE_SIZE];

/*Flag to check if database for testing  is loaded localy in the Stack */
extern bool isDatabaseLoadedInStack;

/*
    Database 's lookup tables for Stack memory
*/

/* Database 's lookup table for attributes size */
extern UInt32_t       attributeSizeStack[DATABASE_SIZE];
/* Database 's lookup table for attributes byte offset in memory */
extern UInt8_t        attributeOffsetStack[DATABASE_SIZE];
/* Database 's lookup table for attributes types */
extern UInt8_t        attributeTypeStack[DATABASE_SIZE];


/** @brief Load to the heap (attUIntX_inst)
 *         all the attribute from the attArray[].
 *         This function allocate resources
 *         in the Heap memory for the Database.
 *  @return  0 for Success and -1 for failure.
 */
int loadDatabaseInHeapFromStack();

#endif /* __DATABASE_TEST_HELPER__ */

