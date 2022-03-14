/** @file testFlashReadWrite.cpp
*   @brief Test gpNvm_getAttribute() and gpNvm_setAttribute() with the nvm database.
*          For the needs of the test , the NVM database ( File attDb.bin which simulate the NVM) is used.
*          First define LOAD_DATABASE_IN_NVM in order to load the database from the Stack to Heap and subsequently to the nvm.
*          The attributes values are defined in the file databaseTestHelper.c in the array attArray. The database is loaded
*          in the stack through attArray. Subsequently undefine the LOAD_DATABASE_IN_NVM in order to start the test.
*          This test read attributes data from the nvm database and writes attributes data in the nvm database.
*          It tests the function gpNvm_getAttribute() and function gpNvm_setAttribute(). The crc is used for the check.
*/

#include "databaseInterface.h"
#include "databaseTestHelper.h"
#include <malloc.h>
#include <stdio.h>


//#define LOAD_DATABASE_IN_NVM

/* Define a global array for set and get Attributes from the nvm database */
UInt8_t  AttributeDataUint8Buffer [ATTRIBUTE_MAXIMUM_DATA_LENGHT] = { 0 };
UInt8_t  testVectorUint8[10] = { 0x13, 0x55, 0x33, 0x15, 0xaa, 0xcc, 0xcf, 0x87, 0xee, 0x68};
UInt16_t AttributeDataUint16Buffer[ATTRIBUTE_MAXIMUM_DATA_LENGHT] = { 0 };
UInt16_t  testVectorUint16[10] = { 0x1377, 0x5513, 0x3CD3, 0x1AE5, 0xaBBa, 0xcc1B, 0xcf23, 0x87FF, 0xee33, 0x680F };
UInt32_t AttributeDataUint32Buffer[ATTRIBUTE_MAXIMUM_DATA_LENGHT] = { 0 };
UInt32_t  testVectorUint32[10] = { 0x13771377, 0x55135513, 0x3CD33CD3, 0x1AE51AE5, 0xaBBaaBBa, 0xcc1Bcc1B, 0xcf23cf23, 0x87FF87FF, 0xee33ee33, 0x680F680F };

/* Attributes IDs according attArray[] in databaseTestHelper.c */
enum attributeID
{                   /* Attribute data type */
    att00Data = 0,  /* UInt8_t */
    att01Data,      /* UInt8_t */
    att02Data,      /* UInt16_t */
    att03Data,      /* UInt32_t */
    att04Data,      /* UInt32_t */
    att05Data       /* UInt16_t */
};

static void setupTest()
{
    /* This gets run before every test.*/
#ifdef LOAD_DATABASE_IN_NVM
    /* Load database for the array attArray in the Heap attUIntX_inst*/
    if (-1 == loadDatabaseInHeapFromStack())
    {
        printf(" Cannot load database in Heap");
        return;
    }
    /* Load database for the Heap attUIntX_inst in the nvm ( File attDb.bin) */
    if (-1 == writeDatabaseFromHeapToNvm())
    {
        printf(" Cannot load database in Nvm");
        return;
    }

#else
    isDatabaseLoadedInNvm = true;
    /* Read the database from the nvm and load it in the Heap attUIntX_inst */
    if (-1 == readDatabaseFromNvmToHeap())
    {
        printf(" Cannot read database from Nvm \n");
        return;
    }
    
#endif /* LOAD_DATABASE_IN_NVM */
}

static void teardownTest()
{
    /* This gets run after every test. */

    /* Unload the database from the Heap*/
    if (-1 == unloadDatabaseFromHeap())
    {
        printf(" Database is not loaded");
        return;
    }
}

void testSetGetAttribute()
{
    setupTest();

#ifndef LOAD_DATABASE_IN_NVM

    UInt8_t attributeLenght = 0;

    /* Get the data and the data length from the first attribute att00Data from the Database in the NVM */
    if (-1 == gpNvm_getAttribute(att00Data, &attributeLenght, AttributeDataUint8Buffer))
    {
        printf(" Cannot load the attribute's data from the nvm . \n ");
        return;
    }

    printData(AttributeDataUint8Buffer, attributeLenght, uint8PtrType);

    /* Copy the first 3 bytes from testVectorUint8 to AttributeDataUint8Buffer*/
    attributeLenght = 3;
    memcpy(AttributeDataUint8Buffer, testVectorUint8 , attributeLenght);
    /* Set the data and the data length from the first attribute att00Data from the Database in the NVM */
    if (-1 == gpNvm_setAttribute(att00Data, attributeLenght, AttributeDataUint8Buffer))
    {
        printf(" Cannot set the attribute's data in the nvm . \n ");
        return;
    }

    /* Get the data and the data length from the first attribute att00Data from the Database in the NVM */
    if (-1 == gpNvm_getAttribute(att00Data, &attributeLenght, AttributeDataUint8Buffer))
    {
        printf(" Cannot load the attribute's data from the nvm . \n ");
        return;
    }
    printData(AttributeDataUint8Buffer, attributeLenght, uint8PtrType);

    /* Copy the first 4 bytes from testVectorUint8+4 address to AttributeDataUint8Buffer address */
    attributeLenght = 4;
    memcpy(AttributeDataUint8Buffer, testVectorUint8+4, attributeLenght);
    /* Set the data and the data length from the first attribute att00Data from the Database in the NVM */
    if (-1 == gpNvm_setAttribute(att00Data, attributeLenght, AttributeDataUint8Buffer))
    {
        printf(" Cannot set the attribute's data in the nvm . \n ");
        return;
    }

    /* Get the data and the data length from the first attribute att00Data from the Database in the NVM */
    if (-1 == gpNvm_getAttribute(att00Data, &attributeLenght, AttributeDataUint8Buffer))
    {
        printf(" Cannot load the attribute's data from the nvm . \n ");
        return;
    }
    printData(AttributeDataUint8Buffer, attributeLenght, uint8PtrType);

    /*      Test UInt32_t attribute data types      */

    /* Get the data and the data length from the first attribute att00Data from the Database in the NVM */
    if (-1 == gpNvm_getAttribute(att04Data, &attributeLenght, AttributeDataUint32Buffer))
    {
        printf(" Cannot load the attribute's data from the nvm . \n ");
        return;
    }

    printData(AttributeDataUint32Buffer, attributeLenght, uint32PtrType);

    /* Copy the first 3* sizeof(UInt32_t) bytes from testVectorUint32 to AttributeDataUint32Buffer*/
    attributeLenght = 3;
    memcpy(AttributeDataUint32Buffer, testVectorUint32, attributeLenght * sizeof(UInt32_t) );
    /* Set the data and the data length from the first attribute att04Data from the Database in the NVM */
    if (-1 == gpNvm_setAttribute(att04Data, attributeLenght, AttributeDataUint32Buffer))
    {
        printf(" Cannot set the attribute's data in the nvm . \n ");
        return;
    }

    /* Get the data and the data length from the first attribute att04Data from the Database in the NVM */
    if (-1 == gpNvm_getAttribute(att04Data, &attributeLenght, AttributeDataUint32Buffer))
    {
        printf(" Cannot load the attribute's data from the nvm . \n ");
        return;
    }
    printData(AttributeDataUint32Buffer, attributeLenght, uint32PtrType);

#endif /* LOAD_DATABASE_IN_NVM */
    teardownTest();

    return;
}