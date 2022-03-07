/**
* @file attributeDatabase.h
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

#ifndef __ATTRIBUTE_DATABASE__
#define __ATTRIBUTE_DATABASE__

#include "attributes.h"

#define DATABASE_SIZE 6

/* Database to load localy in RAM for testing */
extern attUIntXdb_t attArray[DATABASE_SIZE];

/* Flag to check if database for testing  is loaded localy in Ram */
extern bool isDatabaseLoadedInRam;

/* Flag to check if database for testing  is loaded localy in Ram */
extern bool isDatabaseLoadedInNvm;

/* Double Pointer for loading all the database localy to RAM from the NVM */
extern attUIntX_t** attUIntX_inst;

/*
    Database 's lookup tables
*/

/* Database 's lookup table for attributes size */
extern UInt32_t		attributeSize[DATABASE_SIZE];
/* Database 's lookup table for attributes byte offset in memory */
extern UInt8_t		attributeOffset[DATABASE_SIZE];
/* Database 's lookup table for attributes types */
extern UInt8_t      attributeType[DATABASE_SIZE];

#endif /* ATTRIBUTE_DATABASE */