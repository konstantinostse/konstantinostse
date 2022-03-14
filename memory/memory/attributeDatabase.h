/**
* @file attributeDatabase.h
*
* @brief Database attributes declarations.
*
* This contains the attributes declarations for the Database
* and  any macros, constants or global variables you will need.
*
*/

#ifndef __ATTRIBUTE_DATABASE__
#define __ATTRIBUTE_DATABASE__

#include <stdbool.h>
#include "attributes.h"

#define DATABASE_SIZE 6

/* Flag to check if database is loaded in Ram */
extern bool isDatabaseLoadedInNvm;

/* Flag to check if database is loaded in Heap */
extern bool isDatabaseLoadedInHeap;

/* Flag to check if database is loading from Heap to Nvm or the opposite */
extern bool isDatabaseLoadingInProgress;

/* Double Pointer for loading all the database localy to RAM from the NVM */
extern attUIntX_t** attUIntX_inst;

/* Database size in Attributes*/
extern UInt32_t databaseSize;
/*
    Database 's lookup tables
*/

/* Dynamic Database 's lookup table for attributes size */
extern UInt32_t*        attributeSize;
/* Dynamic Database 's lookup table for attributes byte offset in memory */
extern UInt8_t*        attributeOffset;
/* Dynamic Database 's lookup table for attributes types */
extern UInt8_t*      attributeType;
/* Offset for the first attribute in the database */
extern UInt16_t firstAttributeOffset;
#endif /* ATTRIBUTE_DATABASE */