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

#ifndef __DATABASE_TEST_HELPER__
#define __DATABASE_TEST_HELPER__

#include <stdbool.h>
#include "attributes.h"

#define DATABASE_SIZE 6


/* Database 's structure type definition for loading database localy in RAM for testing */
typedef struct attUIntXdb
{
    gpNvm_AttrId id;
    UInt32_t     option;
    UInt8_t      length;
    void*        data;
    UInt32_t     crc;
}attUIntXdb_t;

/* Database to load localy in RAM for testing */
extern attUIntXdb_t attArray[DATABASE_SIZE];

/** @brief Load to the heap (attUIntX_inst)
 *         all the attribute from the attArray[].
 *         This function allocate resources
 *         in the Heap memory for the Database.
 *  @return  0 for Success and -1 for failure.
 */
int loadDatabaseInHeapFromStack();

#endif /* __DATABASE_TEST_HELPER__ */

