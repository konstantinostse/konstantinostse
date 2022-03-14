/* memory.cpp */
//

/*
*    Implementf a non - volatile memory storage component.This component should be able to backup and restore values 
*    corresponding to a unique attribute identifier. For the simplicity of the exercise the underlying non - volatile memory
*    (eeprom, flash, …) can be modelled as a file.
*
*    • Embedded System supports MMU so malloc can be used without fragmentation issues.
*
*    Component Specification
*
*    • Implement the basic implementation of this component based on following API description :
*
*    typedef unsigned char UInt8;
*    typedef UInt8 gpNvm_AttrId;
*    typedef UInt8 gpNvm_Result;
*    gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId,  UInt8* pLength,  UInt8* pValue);
*    gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId,  UInt8 length,  UInt8* pValue);
*
*
*    • Make sure the implementation supports different datatypes.Some examples :
*        o Basic datatypes like UInt8, UInt32, …
*        o An array of UInt8
*        o Structs like for instance :
*
*    #define MAX_LENGTH 20
*        typedef struct {
*        UInt8 id;
*        UInt32 options;
*        UInt8 length;
*        UInt8 data[MAX_LENGTH];
*    } gpTestData_t;
*
*    • Maximum attribute data lenght is 512 bytes.
*    • Extend the your solution with an unit test.
*/

/*
*      Attribute Database description.
* The  nvm driver handles  a database  stored  in the non-volatile memory and 
* a copy of the nvm database stored in Ram (stack) in the heap. Nvm database should be 
* synchronized with the heap databased.  
* Heap database is defined by the double pointer  attUIntX_inst  in the attributeDatabase.cpp
* For testing purposes the array  attArray (in attributeDatabase.cpp ) is defined in order 
* to initialize the heap database and the nvm database with values during the tests . 
* The function loadDatabaseInHeap()   loads the database entries (attributes) values 
* from the attArray in the Heap database (  attUIntX_inst  )  . Subsequently  the writeDatabaseToNvm() 
* loads the Heap database in the NVM. Check unit test testFlashReadWrite.cpp .
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

#include "testFlashReadWrite.h"
#include "testCrcCheck.h"
#include "testBitwise.h"
#include "testSetGetAttribute.h"

#endif /* ENABLE_UNIT_TEST */

int main()
{
    testSetGetAttribute();
    //testFlashReadWrite();
    //testCrcCheck();
    //testBitwise();

    return 0;
}
