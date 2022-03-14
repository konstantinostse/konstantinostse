/**
 * @file attributeDatabase.h
 *
 * @brief Heap database resources  definitions.
 *
 * This contains the attributes definitions for the heap database.   
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

/* Dyanmic Database lookup table for the attributes size in the heap database*/
UInt32_t*    attributeSize = NULL;

/* Dynamic Database lookup table for attributes byte offset in the heap database */
UInt8_t*     attributeOffset = NULL;

/* Dynamic Database lookup table for the attributes types in the heap database*/
UInt8_t*     attributeType = NULL;

/* Offset for the first attribute in the heap database */
UInt16_t firstAttributeOffset = 0;;

