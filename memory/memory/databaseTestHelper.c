/**
 * @file databaseTestHelper.c
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
#include <string.h>
#include <malloc.h>
#include "databaseTestHelper.h"
#include "attributeDatabase.h"
#include "DBInterface.h"
#include "debugFunctions.h"

 /* Enable debug option for the databaseTestHelper.c.*/
#define _ENABLE_DEBUG_




 /*
   Attributes for database  to load localy in RAM for testing
 */

 /* Attribute data to load localy in RAM for testing */
static UInt8_t  att00_data[4] = { 12,17,53,20 };
static UInt8_t  att01_data[5] = { 2,7,9,0,5 };
static UInt16_t att02_data[5] = { 0x1112,0x3417,0x5419,0x2320,0x9865 };
static UInt32_t att03_data[5] = { 0xE5DD1112,0x65EF3417,0x89AD5419,0x22BB2320,0x31AA9865 };
static UInt32_t att04_data[5] = { 0xE5DD1112,0x65EF3417,0x89AD5419,0x22BB2320,0x31AA9865 };
static UInt16_t att05_data[7] = { 0x1112,0x3417,0x5419,0x2320,0x9865,0x3333,0x4444 };

/* Database ( Array with attributes) to load localy in RAM for testing */
attUIntXdb_t attArray[DATABASE_SIZE] = {
                                        { 0, 2, 4,  att00_data, 0},
                                        { 1, 8, 5,  att01_data, 0},
                                        { 2, 9, 5,  att02_data, 0},
                                        { 3, 3, 5,  att03_data, 0},
                                        { 4, 6, 5,  att04_data, 0},
                                        { 5, 3, 7,  att05_data, 0}
};

/** @brief Copy attributes between databases.
*  @param targetAttribute  Destination Attribute
*  @param sourceAttribute  Source Attribute.
*  @param type Type of the attribute's data.
*  @param entrySize Size of the current attribute in bytes.
*  @return Void.
*/
static int copyDatabaseEntries(attUIntX_t* targetAttribute, attUIntXdb_t* sourceAttribute, UInt8_t type, UInt32_t* entrySize)
{

    switch (type)
    {
    case uint8PtrType:
        targetAttribute->id = sourceAttribute->id;
        targetAttribute->option = sourceAttribute->option;
        targetAttribute->length = sourceAttribute->length;
        targetAttribute->data = (UInt8_t*)malloc(sourceAttribute->length * sizeof(UInt8_t));
        if (targetAttribute->data == NULL)
        {
            return -1;
        }
        memcpy(targetAttribute->data, sourceAttribute->data, sourceAttribute->length);
        targetAttribute->crc = (UInt8_t*)malloc(sizeof(UInt8_t));
        if (targetAttribute->crc == NULL)
        {
            return -1;
        }
        *((UInt8_t*)(targetAttribute->crc)) = (UInt8_t)(sourceAttribute->crc);        

        /*Estimates attribute's size in bytes and updates  lookup table attributeSize[DATABASE_SIZE]*/
        *entrySize = sizeof(gpNvm_AttrId) + sizeof(UInt32_t) + sizeof(UInt8_t) + targetAttribute->length * sizeof(UInt8_t) + sizeof(UInt8_t);
#ifdef _ENABLE_DEBUG_
        printData((UInt8_t*)(targetAttribute->data), targetAttribute->length, uint8PtrType);
#endif
        break;

    case uint16PtrType:
        targetAttribute->id = sourceAttribute->id;
        targetAttribute->option = sourceAttribute->option;
        targetAttribute->length = sourceAttribute->length;
        targetAttribute->data = (UInt16_t*)malloc(sourceAttribute->length * sizeof(UInt16_t));
        if (targetAttribute->data == NULL)
        {
            return -1;
        }
        memcpy(targetAttribute->data, sourceAttribute->data, sourceAttribute->length * sizeof(UInt16_t));
        targetAttribute->crc = (UInt16_t*)malloc(sizeof(UInt16_t));
        if (targetAttribute->crc == NULL)
        {
            return -1;
        }
        *((UInt16_t*)(targetAttribute->crc)) = (UInt16_t)sourceAttribute->crc;        

        /*Estimates attribute's size in bytes and updates  lookup table attributeSize[DATABASE_SIZE]*/
        *entrySize = sizeof(gpNvm_AttrId) + sizeof(UInt32_t) + sizeof(UInt8_t) + targetAttribute->length * sizeof(UInt16_t) + sizeof(UInt16_t);
#ifdef _ENABLE_DEBUG_
        printData((UInt16_t*)(targetAttribute->data), targetAttribute->length, uint16PtrType);
#endif
        break;

    case uint32PtrType:
        targetAttribute->id = sourceAttribute->id;
        targetAttribute->option = sourceAttribute->option;
        targetAttribute->length = sourceAttribute->length;
        targetAttribute->data = (UInt32_t*)malloc(sourceAttribute->length * sizeof(UInt32_t));
        if (targetAttribute->data == NULL)
        {
            return -1;
        }
        memcpy(targetAttribute->data, sourceAttribute->data, sourceAttribute->length * sizeof(UInt32_t));
        targetAttribute->crc = (UInt32_t*)malloc(sizeof(UInt32_t));
        if (targetAttribute->crc == NULL)
        {
            return -1;
        }
        *((UInt32_t*)(targetAttribute->crc)) = (UInt32_t)sourceAttribute->crc;

        /*Estimates attribute's size in bytes and updates  lookup table attributeSize[DATABASE_SIZE]*/
        *entrySize = sizeof(gpNvm_AttrId) + sizeof(UInt32_t) + sizeof(UInt8_t) + targetAttribute->length * sizeof(UInt32_t) + sizeof(UInt32_t);
#ifdef _ENABLE_DEBUG_
        printData((UInt32_t*)(targetAttribute->data), targetAttribute->length, uint32PtrType);
#endif
        break;
    default:
        /* Invalid Data type */
        return -1;
        break;
    }

    return 0;
}

int  loadDatabaseInHeapFromStack()
{
    attUIntX_inst = (attUIntX_t**)malloc(DATABASE_SIZE * sizeof(attUIntX_t*));
    if (attUIntX_inst == NULL)
    {
        return -1;
    }
    for (int i = 0; i < DATABASE_SIZE; i++)
    {
        attUIntX_inst[i] = (attUIntX_t*)malloc(sizeof(attUIntX_t));
        if (attUIntX_inst[i] == NULL)
        {
            return -1;
        }
        if (i > 0)
        {
            attributeOffset[i] = attributeOffset[i - 1] + attributeSize[i - 1];
        }
        if (-1 == copyDatabaseEntries(attUIntX_inst[i], (attArray + i), attributeType[i], attributeSize + i))
        {
            free(attUIntX_inst[i]);
            attUIntX_inst[i] = NULL;
            return -1;
        }
    }

    /* Attributes Database is loaded in Ram. */
    isDatabaseLoadedInRam = true;

    /* Succes to load the database in Ram!*/
    return 0;
}
