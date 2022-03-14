/**
 * @file attributeDatabase.h
 *
 * @brief Database attributes definitions.
 *
 * This contains the attributes definitions for the Database.   
 *
 */

#include <malloc.h>
#include "attributeDatabase.h"

/* Flag to check if database is loaded in Heap */
bool isDatabaseLoadedInHeap = false;

/* Flag to check if database is loaded in NVM */
bool isDatabaseLoadedInNvm = false;

/* Flag to check if database is being loaded from Heap to Nvm or the opposite */
bool isDatabaseLoadingInProgress = false;

/* Double Pointer for loading all the database to Heap */
attUIntX_t** attUIntX_inst = NULL;

/* Database size in Attributes */
UInt32_t databaseSize = 0;

/*
    Database 's lookup tables
*/

/* Dyanmic Database lookup table for attributes size */
UInt32_t*    attributeSize = NULL;

/* Dynamic Database lookup table for attributes byte offset in memory */
UInt8_t*     attributeOffset = NULL;

/* Dynamic Database lookup table for attributes types */
UInt8_t*     attributeType = NULL;

/* Offset for the first attribute in the database */
UInt16_t firstAttributeOffset = 0;;

