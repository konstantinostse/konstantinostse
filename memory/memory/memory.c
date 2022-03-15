/* memory.c */

/**
 *   Requirements and Specifications. 
 *
 *   Implement a non - volatile memory storage component. This component should be able to backup and restore values
 *   corresponding to a unique attribute identifier. For the simplicity of the exercise the underlying non - volatile memory (eeprom, flash, …) can be modelled as a file.
 *
 *   • This Embedded System supports MMU so malloc can be used without fragmentation issues.
 *
 *   Component Specification
 *
 *   • Implement the basic implementation of this component based on following API description
 *
 *   typedef unsigned char UInt8;
 *   typedef UInt8 gpNvm_AttrId;
 *   typedef UInt8 gpNvm_Result;
 *   gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId,  UInt8* pLength,  UInt8* pValue);
 *   gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId,  UInt8 length,  UInt8* pValue);
 *
 *
 *   • Make sure the implementation supports different datatypes.
 *   Some examples :
 *       o Basic datatypes like UInt8, UInt32, …
 *       o An array of UInt8
 *       o Structs like for instance :
 *
 *   #define MAX_LENGTH 20
 *       typedef struct {
 *       UInt8 id;
 *       UInt32 options;
 *       UInt8 length;
 *       UInt8 data[MAX_LENGTH];
 *   } gpTestData_t;
 *
 *   • Maximum attribute data length is 512 bytes.
 *   • Extend your solution with an unit test.
 *
 *   Implementation
 *
 *   The nvm driver writes and reads from a database stored in the non-volatile memory (nvm). A copy of the nvm database
 *   is stored also in the heap which it is pointed by the pointer attUIntX_inst in the file attributeDatabase.c. Every time
 *   that the NVM database is updated through the function gpNvm_setAttribute() the database in the Heap is updated also.
 *   So the Nvm database is synchronized with the heap database. The purpose of the heap database is for quicker access for reading.
 *   The gpNvm_getAttribute()is for direct reading from the nvm.
 *   Heap database is defined by the double pointer attUIntX_inst in the attributeDatabase.c.
 *   For testing purposes the array attArray (in databaseTestHelper.c ) is defined in order to initialize the heap database and
 *   subsequently the nvm database with values during the tests. The function loadDatabaseInHeapFromStack() ( file databaseTestHelpr.c)
 *   loads the database entries (attributes) values from the attArray in the Heap database ( attUIntX_inst ).Subsequently the writeDatabaseToNvm()
 *   loads the Heap database in the NVM. Check unit test testSetGetAttribute.c. The nvm database is simulated by the file attDb.bin. Check the example in
 *   the Unit test folder testSetGetAttribute in order to initialise the nvm database in the file attDb.bin.
 *   In the file databaseInterface.c the readDatabaseFromNvmToHeap() function loads the nvm database in the Heap. Also it creates the database in the
 *   heap pointed by the pointer attUIntX_inst. If the database is loaded in the heap from the loadDatabaseInHeapFromStack() you
 *   cannot use readDatabaseFromNvmToHeap() or the opposite. Everytime the database is allocated in the heap , use the unloadDatabaseFromHeap()
 *   in order to free the memory in the heap. The heap database resources are defined in the attributeDatabase.c.
 *   The heap database utilises three look up tables. attributeSize, attributeOffset and attributeType. attributeSize stores the size
 *   for each attribute in the heap database. attributeOffset stores the offset for each attribute in the heap database. attributeType stores
 *   the data type for each attribute in the heap database. The look up tables are utilised for quick access in the heap database.
 *   The address size for the nvm memory is 32 bits.
 *
 *   Nvm attribute Database structure
 *
 *   All the elements of the nvm database are defined in the attributeDatabase.c
 *   According the attributeDatabase.c the database nvm storage has the following order :
 *
 *   First  element : databaseSize
 *   Second element : attributeSize   look up table
 *   Third  element : attributeOffset look up table
 *   Forth  element : attributeType   look up table
 *   Fifth  element : attUIntX_inst   attribute’s database
 *
 *   Folders
 *
 *   Memory
 *   |
 *   Source Files
 *   |
 *   |______attributeDatabase   Heap database resources definition.
 *   |
 *   |______crcCheck            Crc error check for read/write attribute data from the nvm.
 *   |
 *   |______databaseInterface   Database interface to write/read attributes data in the nvm.
 *   |
 *   |______databaseTestHelper  Stack database and functions in order to load it in the Heap
 *   |
 *   |______nvmDriver           Write and read functions for the nvm.
 *   |
 *   |______debugFunctions      Debug function in order to print arrays for each data type.
 *   |
 *   |______attributes.h        Basic types definition for the database.
 *   |
 *   |
 *   Unit_Tests_Source_Files
 *   |
 *   |______testSetGetAttribute Test gpNvm_setAttribute() gpNvm_getAttribute() functions. 
 *
 *   
 *
 */

#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "databaseInterface.h"

#define ENABLE_UNIT_TEST

#ifdef ENABLE_UNIT_TEST

#include "testSetGetAttribute.h"

#endif /* ENABLE_UNIT_TEST */

int main()
{
    testSetGetAttribute();
    return 0;
}
