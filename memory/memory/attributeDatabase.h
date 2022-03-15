/**
* @file attributeDatabase.h
*
* @brief Heep database resources declarations.
*
* This contains the attributes declarations for the heap database
* and  any macros, constants or global variables you will need.
*
*/

#ifndef __ATTRIBUTE_DATABASE__
#define __ATTRIBUTE_DATABASE__

#include <stdbool.h>
#include "attributes.h"

#define DATABASE_SIZE 6

/* Database attribute maximum data lenght is 512 bytes. */
#define ATTRIBUTE_MAXIMUM_DATA_LENGHT 512 

/* Flag to check if database is loaded in the nvm */
extern bool isDatabaseLoadedInNvm;

/* Flag to check if database is loaded in heap */
extern bool isDatabaseLoadedInHeap;

/* Flag to check if database is being loaded from heap to nvm or the opposite */
extern bool isDatabaseLoadingInProgress;

/* Double Pointer for loading all the database to heap from the nvm or the stack*/
extern attUIntX_t** attUIntX_inst;

/* Database size in attributes*/
extern UInt32_t databaseSize;

/*
    Database 's lookup tables
*/

/* Dynamic Database 's lookup table for attributes size in the heap. */
extern UInt32_t*        attributeSize;

/* Dynamic Database 's lookup table for attributes byte offset in the heap. */
extern UInt8_t*        attributeOffset;

/* Dynamic Database 's lookup table for attributes types in the heap.*/
extern UInt8_t*      attributeType;

/* Offset for the first attribute in the database in the heap*/
extern UInt16_t firstAttributeOffset;
#endif /* ATTRIBUTE_DATABASE */