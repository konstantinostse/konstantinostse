/**
* @file DBInterface.cpp
*
* @brief Database interface function implementations.
*
* Function implementations for interacting with the Attributes Database. 
*
*/

// Disable assert
//#define NDEBUG
#include <assert.h>
#include <string.h>
#include <malloc.h>

#include "DBInterface.h"
#include "nvmDriver.h"
#include "attributeDatabase.h"
#include "crcCheck.h"
#include "debugFunctions.h"

/* Enable debug option for the DBInterface.cpp.*/
#define _ENABLE_DEBUG_

/** @brief Copy attributes between databases.
*  @param targetAttribute  Destination Attribute 
*  @param sourceAttribute  Source Attribute.
*  @param type Type of the attribute's data.
*  @param entrySize Size of the current attribute in bytes.
*  @return Void.
*/
static int copyDatabaseEntries( attUIntX_t* targetAttribute , attUIntXdb_t* sourceAttribute, UInt8_t type, UInt32_t *entrySize)
{	

	switch (type)
	{
	case uint8PtrType:		
        targetAttribute->id = sourceAttribute->id;
		targetAttribute->option = sourceAttribute->option;
		targetAttribute->length = sourceAttribute->length;
		targetAttribute->data = (UInt8_t*)malloc(sourceAttribute->length * sizeof(UInt8_t));
		memcpy(targetAttribute->data, sourceAttribute->data, sourceAttribute->length);
		targetAttribute->crc = (UInt8_t*)malloc(sizeof(UInt8_t));
		*((UInt8_t*)(targetAttribute->crc)) = (UInt8_t)sourceAttribute->crc;

         /*Estimates attribute's size in bytes and updates  lookup table attributeSize[DATABASE_SIZE]*/ 
		*entrySize = sizeof(gpNvm_AttrId) + sizeof(UInt32_t) + sizeof(UInt8_t) + targetAttribute->length * sizeof(UInt8_t) + sizeof(UInt8_t);
#ifdef _ENABLE_DEBUG_
		printData((UInt8_t*)(targetAttribute->data), targetAttribute->length);
#endif	
		break;	

	case uint16PtrType:	
		targetAttribute->id = sourceAttribute->id;
		targetAttribute->option = sourceAttribute->option;
		targetAttribute->length = sourceAttribute->length;
		targetAttribute->data = (UInt16_t*)malloc(sourceAttribute->length * sizeof(UInt16_t));
		memcpy(targetAttribute->data, sourceAttribute->data, sourceAttribute->length * sizeof(UInt16_t));
		targetAttribute->crc = (UInt16_t*)malloc(sizeof(UInt16_t));
		*((UInt16_t*)(targetAttribute->crc)) = (UInt16_t)sourceAttribute->crc;

		/*Estimates attribute's size in bytes and updates  lookup table attributeSize[DATABASE_SIZE]*/
		*entrySize = sizeof(gpNvm_AttrId) + sizeof(UInt32_t) + sizeof(UInt8_t) + targetAttribute->length * sizeof(UInt16_t) + sizeof(UInt16_t);
#ifdef _ENABLE_DEBUG_
		printData((UInt16_t*)(targetAttribute->data), targetAttribute->length);
#endif
		break;

	case uint32PtrType:
		targetAttribute->id = sourceAttribute->id;
		targetAttribute->option = sourceAttribute->option;
		targetAttribute->length = sourceAttribute->length;
		targetAttribute->data = (UInt32_t*)malloc(sourceAttribute->length * sizeof(UInt32_t));
		memcpy(targetAttribute->data, sourceAttribute->data, sourceAttribute->length * sizeof(UInt32_t));
		targetAttribute->crc = (UInt32_t*)malloc(sizeof(UInt32_t));
		*((UInt32_t*)(targetAttribute->crc)) = (UInt32_t)sourceAttribute->crc;

		/*Estimates attribute's size in bytes and updates  lookup table attributeSize[DATABASE_SIZE]*/
		*entrySize = sizeof(gpNvm_AttrId) + sizeof(UInt32_t) + sizeof(UInt8_t)+ targetAttribute->length * sizeof(UInt32_t) + sizeof(UInt32_t) ;
#ifdef _ENABLE_DEBUG_
		printData((UInt32_t*)(targetAttribute->data), targetAttribute->length);
#endif
		break;
	default:
		/* Invalid Data type */
		return -1;
		break;
	}

	return 0;
}

int  loadDatabaseInHeap()
{	
	attUIntX_inst = (attUIntX_t**) malloc (DATABASE_SIZE * sizeof(attUIntX_t*));
	for (int i = 0; i < DATABASE_SIZE; i++)
	{
		attUIntX_inst[i] = (attUIntX_t*)malloc(sizeof(attUIntX_t));
		if (i > 0)
		{
			attributeOffset[i] = attributeOffset[i-1] + attributeSize[i - 1];
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

int unloadDatabaseFromHeap()
{
	/* Is Attribute database loaded in Ram? */
	if (true == isDatabaseLoadedInRam)
	{
		for (int i = 0; i < DATABASE_SIZE; i++)
		{
			free(attUIntX_inst[i]->crc);
			attUIntX_inst[i]->crc = NULL;
			free(attUIntX_inst[i]->data);
			attUIntX_inst[i]->data = NULL;
			free(attUIntX_inst[i]);
			attUIntX_inst[i] = NULL;
		}
		free(attUIntX_inst);
		attUIntX_inst = NULL;
		isDatabaseLoadedInRam = false;

		/* Success to unload the database from Ram!*/
		return 0;
	}
	else
	{
		/* Attribute database is already unloaded from Ram? */
		return -1;
	}
}

gpNvm_Result gpNvm_setAttribute(gpNvm_AttrId attrId, UInt8_t pLenght, void* pValue)
{
	/* Is Attribute database loaded in the Nvm ? */
	if (( true == isDatabaseLoadedInNvm) && ( true == isDatabaseLoadedInRam))
	{
		if (pLenght <= attUIntX_inst[attrId]->length)
		{			
			/* Attribute offset  for the data in bytes.*/
			UInt16_t offset = 0;
			/* Attribute data size + crc size in bytes.*/
			UInt16_t dataSize = 0;
			UInt8_t* buffer = NULL;

			/* id offset */
			offset = 0;
			/* option offset */
			offset += sizeof(gpNvm_AttrId);
			/* lenght offset */
			offset += sizeof(UInt32_t);
			/* data offset */
			offset += sizeof(UInt8_t);
			switch (attributeType[attrId])
			{
			case uint8PtrType:
				/* Allocate buffer with size attUIntX_inst[attrId]->length and not pLenght.
				*  Maximum Attribute data field size in the database cannot change in runtime.*/
				buffer = (UInt8_t*)calloc((attUIntX_inst[attrId]->length + 1) * sizeof(UInt8_t), sizeof(UInt8_t));
				/* Copy new attribute value in the buffer. */
				memcpy(buffer, (UInt8_t*)pValue, pLenght * sizeof(UInt8_t));
				/* Update the new attribute value in the RAM array. */
				memcpy(attUIntX_inst[attrId]->data, (UInt8_t*)pValue, pLenght * sizeof(UInt8_t));
				dataSize = attUIntX_inst[attrId]->length * sizeof(UInt8_t);
				/* copy CRC */
				*((UInt8_t*)((attUIntX_inst[attrId])->crc)) = crcCheck((UInt8_t*)(pValue), pLenght);
				memcpy(buffer + dataSize, (UInt8_t*)attUIntX_inst[attrId]->crc, sizeof(UInt8_t));				
				dataSize += sizeof(UInt8_t);
				break;
			case uint16PtrType:
				/* Allocate buffer with size attUIntX_inst[attrId]->length and not pLenght.
				*  Maximum Attribute data field size in the database cannot change in runtime.*/
				buffer = (UInt8_t*)calloc((attUIntX_inst[attrId]->length + 1) * sizeof(UInt16_t), sizeof(UInt16_t));
				/* Copy new attribute value in the buffer. */
				memcpy(buffer, (UInt8_t*)pValue, pLenght * sizeof(UInt16_t));
				/* Update the new attribute value in the RAM array. */
				memcpy(attUIntX_inst[attrId]->data, (UInt8_t*)pValue, pLenght * sizeof(UInt16_t));
				dataSize = attUIntX_inst[attrId]->length * sizeof(UInt16_t);
				/* copy CRC */
				*((UInt16_t*)((attUIntX_inst[attrId])->crc)) = crcCheck((UInt16_t*)(pValue), pLenght);
				memcpy(buffer + dataSize, (UInt8_t*)attUIntX_inst[attrId]->crc, sizeof(UInt16_t));
				dataSize += sizeof(UInt16_t);
				break;
			case uint32PtrType:
				/* Allocate buffer with size attUIntX_inst[attrId]->length and not pLenght.
				*  Maximum Attribute data field size in the database cannot change in runtime.*/
				buffer = (UInt8_t*)calloc((attUIntX_inst[attrId]->length + 1) * sizeof(UInt32_t), sizeof(UInt32_t));
				/* Copy mew attribute value in the buffer. */
				memcpy(buffer, (UInt8_t*)pValue, pLenght * sizeof(UInt32_t));
				memcpy(attUIntX_inst[attrId]->data, (UInt8_t*)pValue, pLenght * sizeof(UInt32_t));
				dataSize = attUIntX_inst[attrId]->length * sizeof(UInt32_t);
				/* copy CRC */
				*((UInt32_t*)((attUIntX_inst[attrId])->crc)) = crcCheck((UInt32_t*)(pValue), pLenght);
				memcpy(buffer + dataSize, (UInt8_t*)attUIntX_inst[attrId]->crc, sizeof(UInt32_t));
				dataSize += sizeof(UInt32_t);
				break;
			default:
				free(buffer);
				buffer = NULL;
				/* Invalid Data type */
				return -1;
				break;
			}
			/* Check if the offset in the database is ok. */
			assert((offset + dataSize) == attributeSize[attrId]);
#ifdef _ENABLE_DEBUG_
			printData(buffer, dataSize);
#endif			
			if (-1 == writeNvm(buffer, attributeOffset[attrId] + offset, dataSize))
			{
				free(buffer);
				buffer = NULL;
				/* NVM write failure! */
				return -1;
			}
			free(buffer);
			buffer = NULL;

			/* NVM successfully written! */
			return 0;
		}
		else
		{
			/* pLenght is greater from the length of the data in this attribute  */
			assert(pLenght <= attUIntX_inst[attrId]->length);
			return -1;
		}
	}
	else
	{
		/* Attribute database is not loaded in the Nvm and Ram.
		*  No database in nvm to write!
		*/
		return -1;
	}
}

gpNvm_Result gpNvm_getAttribute(gpNvm_AttrId attrId, UInt8_t* pLenght, void* pValue)
{
	/* Is Attribute database loaded in the Nvm  and Ram ? */
	if ((true == isDatabaseLoadedInNvm) && (true == isDatabaseLoadedInRam))
	{		
		if (-1 == readDatabaseAttribute(attUIntX_inst[attrId], attrId))
		{		
		    /* NVM read failure! */
			return -1;
		}		
		*pLenght = attUIntX_inst[attrId]->length;		
		if (NULL != pValue)
		{
			switch (attributeType[attrId])
			{
			case uint8PtrType:
				memcpy(pValue, attUIntX_inst[attrId]->data, attUIntX_inst[attrId]->length * sizeof(UInt8_t));
				break;
			case uint16PtrType:
				memcpy(pValue, attUIntX_inst[attrId]->data, attUIntX_inst[attrId]->length * sizeof(UInt16_t));
				break;
			case uint32PtrType:
				memcpy(pValue, attUIntX_inst[attrId]->data, attUIntX_inst[attrId]->length * sizeof(UInt32_t));
				break;
			default:
				/* Invalid Data type */
				return -1;
				break;
			}
		}
		/* NVM read succeeded ! */
		return 0;
	}
	else
	{
		/* Attribute database is not loaded in the Nvm and Ram.
		*  No database in nvm and ram to read!
		*/
		return -1;
	}
}

int writeDatabaseAttribute(attUIntX_t* const attribute , UInt8_t attributeNum)
{    
	attUIntX_t* attributeLocal = attribute;
	UInt16_t offset = 0;

	UInt8_t* buffer = (UInt8_t*)malloc(attributeSize[attributeNum] * sizeof(UInt8_t));

	/* copy id */
	memcpy(buffer + offset, &(attributeLocal->id), sizeof(gpNvm_AttrId));
	/* copy option */
	offset += sizeof(gpNvm_AttrId);
	memcpy(buffer + offset, &(attributeLocal->option), sizeof(UInt32_t));
	/* copy lenght */
	offset += sizeof(UInt32_t);
	memcpy(buffer + offset, &(attributeLocal->length), sizeof(UInt8_t));
	/* copy data */
	offset += sizeof(UInt8_t);
	switch (attributeType[attributeNum])
	{
	case uint8PtrType:
		memcpy(buffer + offset, attributeLocal->data, attributeLocal->length * sizeof(UInt8_t));
		offset += attributeLocal->length * sizeof(UInt8_t);
		/* copy CRC */
		*((UInt8_t*)(attributeLocal->crc)) = crcCheck((UInt8_t*)(attributeLocal->data), attributeLocal->length);
		memcpy(buffer + offset, attributeLocal->crc, sizeof(UInt8_t));
		offset += sizeof(UInt8_t);
		break;
	case uint16PtrType:
		memcpy(buffer + offset, (UInt8_t*)(attributeLocal->data), attributeLocal->length * sizeof(UInt16_t));
		offset += attributeLocal->length * sizeof(UInt16_t);
		/* copy CRC */
		*((UInt16_t*)(attributeLocal->crc)) = crcCheck((UInt16_t*)(attributeLocal->data), attributeLocal->length);
		memcpy(buffer + offset, (UInt8_t*)(attributeLocal->crc), sizeof(UInt16_t));
		offset += sizeof(UInt16_t);
		break;
	case uint32PtrType:
		memcpy(buffer + offset, (UInt8_t*)(attributeLocal->data), attributeLocal->length * sizeof(UInt32_t));
		offset += attributeLocal->length * sizeof(UInt32_t);
		/* copy CRC */
		*((UInt32_t*)(attributeLocal->crc)) = crcCheck((UInt32_t*)(attributeLocal->data), attributeLocal->length);
		memcpy(buffer + offset, (UInt32_t*)(attributeLocal->crc), sizeof(UInt32_t));		
		offset += sizeof(UInt32_t);
		break;
	default:
		free(buffer);
		buffer = NULL;
		/* Invalid Data type */
		return -1;
		break;
	}
	/* Check if the offset in the database is ok. */
	assert(offset == attributeSize[attributeNum]);
#ifdef _ENABLE_DEBUG_
	printData(buffer, attributeSize[attributeNum]);
#endif			
	if( -1 == writeNvm(buffer, attributeOffset[attributeNum], attributeSize[attributeNum]))
	{
		free(buffer);
		buffer = NULL;
		/* NVM write failure! */
		return -1;
	}
	free(buffer);
	buffer = NULL;
	/* NVM successfully write! */
	return 0;	
}

int readDatabaseAttribute(attUIntX_t* const attribute, UInt8_t attributeNum)
{
	/* Is Attribute database loaded in the nvm ? */
	if (true == isDatabaseLoadedInNvm)
	{
		attUIntX_t* attributeLocal = attribute;
		UInt16_t offset = 0;		
		UInt32_t crcLocal = 0;

		UInt8_t* buffer = (UInt8_t*)malloc(attributeSize[attributeNum] * sizeof(UInt8_t));
				
		if (-1 == readNvm(buffer, attributeOffset[attributeNum], attributeSize[attributeNum]))
		{
			free(buffer);
			buffer = NULL;
			/* NVM write failure! */
			return -1;
		}
#ifdef _ENABLE_DEBUG_
		printData(buffer, attributeSize[attributeNum]);
#endif
		/* copy id */
		memcpy(&(attributeLocal->id), buffer + offset, sizeof(gpNvm_AttrId));
		/* copy option */
		offset += sizeof(gpNvm_AttrId);
		memcpy(&(attributeLocal->option), buffer + offset, sizeof(UInt32_t));
		/* copy lenght */
		offset += sizeof(UInt32_t);
		memcpy(&(attributeLocal->length), buffer + offset, sizeof(UInt8_t));
		/* copy data */
		offset += sizeof(UInt8_t);
		switch (attributeType[attributeNum])
		{

		case uint8PtrType:
			memcpy(attributeLocal->data, buffer + offset, attributeLocal->length * sizeof(UInt8_t));
			offset += attributeLocal->length * sizeof(UInt8_t);
			/* copy CRC */
			memcpy(attributeLocal->crc, buffer + offset, sizeof(UInt8_t));
			offset += sizeof(UInt8_t);

			/* Check CRC error in Data after reading from NVM  */
			crcLocal = crcCheck((UInt8_t*)(attributeLocal->data), attributeLocal->length);
			assert(crcLocal == *((UInt8_t*)(attributeLocal->crc)));
			if (crcLocal != *((UInt8_t*)(attributeLocal->crc)))
			{
				free(buffer);
				buffer = NULL;
				/* Error reading Data from the NVM*/
				return -1;
			}
#ifdef _ENABLE_DEBUG_
			printData((UInt8_t*)attributeLocal->data, attributeLocal->length);
#endif
			break;
		case uint16PtrType:
			memcpy(attributeLocal->data, buffer + offset, attributeLocal->length * sizeof(UInt16_t));
			offset += attributeLocal->length * sizeof(UInt16_t);
			/* copy CRC */
			memcpy(attributeLocal->crc, buffer + offset, sizeof(UInt16_t));
			offset += sizeof(UInt16_t);

			/* Check CRC error in Data after reading from NVM  */
			crcLocal = crcCheck((UInt16_t*)(attributeLocal->data), attributeLocal->length);
			assert(crcLocal == *((UInt16_t*)(attributeLocal->crc)));
			if (crcLocal != *((UInt16_t*)(attributeLocal->crc)))
			{
				free(buffer);
				buffer = NULL;
				/* Error reading Data from the NVM*/
				return -1;
			}
#ifdef _ENABLE_DEBUG_
			printData((UInt16_t*)attributeLocal->data, attributeLocal->length);
#endif
			break;
		case uint32PtrType:
			memcpy(attributeLocal->data, buffer + offset, attributeLocal->length * sizeof(UInt32_t));
			offset += attributeLocal->length * sizeof(UInt32_t);
			/* Copy CRC */
			memcpy(attributeLocal->crc, buffer + offset, sizeof(UInt32_t));
			offset += sizeof(UInt32_t);

			/* Check CRC error in Data after reading from NVM  */			
			crcLocal = crcCheck((UInt32_t*)(attributeLocal->data), attributeLocal->length);				
			assert(crcLocal == *((UInt32_t*)(attributeLocal->crc)));
			if (crcLocal != *((UInt32_t*)(attributeLocal->crc)))
			{
				free(buffer);
				buffer = NULL;
				/* Error reading Data from the NVM*/
				return -1;
			}
#ifdef _ENABLE_DEBUG_
			printData((UInt32_t*)attributeLocal->data, attributeLocal->length);
#endif
			break;
		default:
			free(buffer);
			buffer = NULL;
			/* Invalid Data type */
			return -1;
			break;
		}

		/* Check if the offset in the database is ok. */
		assert(offset == attributeSize[attributeNum]);

		free(buffer);
		buffer = 0;

		/* NVM successfully read */
		return 0;
	}
	else
	{
		/* Attribute database is not loaded in the nvm.
		*  No database in nvm to read!
		*/
		return -1;
    }
}

int writeDatabaseToNvm()
{
	/* Is Attribute database loaded in RAM in attUIntX_inst ? */
	if (true == isDatabaseLoadedInRam)
	{
		for (int i = 0; i < DATABASE_SIZE; i++)
		{
			if (-1 == writeDatabaseAttribute(attUIntX_inst[i], i))
			{
				/* Failure to write attribute in NVM. */
				return -1;
			}
		}

		isDatabaseLoadedInNvm = true;

		return 0;
	}
	else
	{
		/* Attribute database is not loaded in RAM in attUIntX_inst
		*  in order to write it in the NVM. Call first loadDatabase();
		*/
		return -1;
	}
}

