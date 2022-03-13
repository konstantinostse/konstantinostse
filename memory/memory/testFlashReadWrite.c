/** @file testFlashReadWrite.cpp
* @brief Test NVM read / write driver implementation.
*        Test Database interface.
*/

#include "DBInterface.h"
#include "databaseTestHelper.h"
#include <malloc.h>
#include <stdio.h>

/* Testing attributes*/
attUIntX_t* attS00_t;
attUIntX_t* attS05_t;
attUIntX_t* attS04_t;

enum attID
{
    att00Data = 0,
    att01Data,
    att02Data,
    att03Data,
    att04Data,
    att05Data    
};

static void setupTest()
{
    /* This gets run before every test.*/
    if (-1 == loadDatabaseInHeapFromStack())
    {
        printf(" Cannot load database in Heap");
        return;
    }

    if (-1 == writeDatabaseFromHeapToNvm())
    {
        printf(" Cannot load database in Nvm");
        return;
    }

    /* Initialise Testing attributes */
    attS00_t = (attUIntX_t*)malloc(sizeof(attUIntX_t));
    if (attS00_t == NULL)
    {
        return;
    }
    attS00_t->data = (UInt8_t*) malloc( 4 * sizeof(UInt8_t));
    attS00_t->crc  = (UInt8_t*) malloc( sizeof(UInt8_t));

    attS05_t       = (attUIntX_t*)malloc(sizeof(attUIntX_t));
    if (attS05_t == NULL)
    {
        return ;
    }
    attS05_t->data = (UInt16_t*)malloc(7 * sizeof(UInt16_t));
    attS05_t->crc  = (UInt16_t*)malloc(sizeof(UInt16_t));

    attS04_t       = (attUIntX_t*)malloc(sizeof(attUIntX_t));
    if (attS04_t == NULL)
    {
        return;
    }
    attS04_t->data = (UInt32_t*)malloc(5 * sizeof(UInt32_t));
    attS04_t->crc  = (UInt32_t*)malloc(sizeof(UInt32_t));
}

static void teardownTest()
{
    /* Release resources for the testing attributes */
    free(attS00_t->crc);
    free(attS00_t->data);
    free(attS00_t);

    free(attS04_t->crc);
    free(attS04_t->data);
    free(attS04_t);

    free(attS05_t->crc);
    free(attS05_t->data);
    free(attS05_t);

    /* This gets run after every test. */    
    if (-1 == unloadDatabaseFromHeap())
    {
        printf(" Database is not loaded");
        return;
    }
}

void testFlashReadWrite()
{
    setupTest();    
        
    readDatabaseAttribute(attS00_t, att00Data);
    printf(" ### ------------------------------- Test Point 1 -------------------------------------- ###\n");
    readDatabaseAttribute(attS05_t, att05Data);
    printf(" ### ------------------------------- Test Point 2 -------------------------------------- ###\n");
    

    /* Change data .*/
    *((UInt8_t*)(attS00_t->data) + 0) = 0xa;
    *((UInt8_t*)(attS00_t->data) + 1) = 0xb;
    *((UInt8_t*)(attS00_t->data) + 2) = 0xc;
    *((UInt8_t*)(attS00_t->data) + 3) = 0xd;    
    attS00_t->length = 4;

    //gpNvm_setAttribute(att0, &(attS00_t->length), attS00_t->data);
    //gpNvm_getAttribute(att0, &(attS00_t->length), attS00_t->data);
    //writeDatabaseAttribute(attS00_t, 0);
    gpNvm_setAttribute(0, 4, (UInt8_t*)(attS00_t->data));
    readDatabaseAttribute(attS00_t, 0);
    
    //UInt8_t pLenght = 0;
    //gpNvm_getAttribute(3, &pLenght, NULL);

    teardownTest();
    
    return;
}