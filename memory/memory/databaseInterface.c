/**
* @file databaseInterface.cpp
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

#include "databaseInterface.h"
#include "nvmDriver.h"
#include "attributeDatabase.h"
#include "crcCheck.h"
#include "debugFunctions.h"

/* Enable debug option for the databaseInterface.c.*/
#define _ENABLE_DEBUG_

/*----------------Static functions prototypes declaration ----------------------------------*/
static int writeDatabaseAttribute(attUIntX_t* const attribute, UInt8_t attributeNum);
static int readDatabaseAttribute(attUIntX_t* const attribute, UInt8_t attributeNum);

int unloadDatabaseFromHeap()
{
    /* Is Attribute database loaded in Heap? */
    if ((true == isDatabaseLoadedInHeap) && (databaseSize > 0) )
    {
        for (int i = 0; i < databaseSize; i++)
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

        isDatabaseLoadedInHeap = false;

        /* Success to unload the database from Heap!*/
        return 0;
    }
    else
    {
        /* Attribute database is already unloaded from Heap? */
        return -1;
    }
}

int writeDatabaseFromHeapToNvm()
{
    /* Is Attribute database loaded in Heap in attUIntX_inst 
    *  If database is loaded in nvm , then rewrite the database in nvm.
    */
    if ( ( true == isDatabaseLoadedInHeap) && (databaseSize > 0) )
    {
        /* Database elements offset in bytes.*/
        UInt16_t offset = 0;

        if (-1 == writeNvm( &databaseSize, offset, sizeof(UInt32_t)))
        {
            /* NVM write failure! */
            return -1;
        }
        
        printData(attributeSize, databaseSize, uint32PtrType);

        offset += sizeof(UInt32_t);
        if (-1 == writeNvm( attributeSize, offset, databaseSize * sizeof(UInt32_t)))
        {
            /* NVM write failure! */
            return -1;
        }

        printData(attributeOffset, databaseSize, uint8PtrType);

        offset += databaseSize * sizeof(UInt32_t);
        if (-1 == writeNvm( attributeOffset, offset, databaseSize * sizeof(UInt8_t)))
        {
            /* NVM write failure! */
            return -1;
        }

        printData(attributeType, databaseSize, uint8PtrType);

        offset += databaseSize * sizeof(UInt8_t);
        if (-1 == writeNvm( attributeType, offset, databaseSize * sizeof(UInt8_t)))
        {
            /* NVM write failure! */
            return -1;
        }

        offset += databaseSize * sizeof(UInt8_t);
        /* Set the global variable for the offset for the first attribute in the database */
        firstAttributeOffset = offset;

        for (int i = 0; i < databaseSize; i++)
        {
            if (-1 == writeDatabaseAttribute( attUIntX_inst[i], i) )
            {
                /* Failure to write attribute in NVM. */
                return -1;
            }
            printData(attUIntX_inst[i]->data, attUIntX_inst[i]->length, attributeType[i]);
        }

        isDatabaseLoadedInNvm = true;

        return 0;
    }
    else
    {
        /* Attribute database is not loaded in Heap in attUIntX_inst
        *  in order to write it in the NVM. Call first loadDatabase();
        */
        return -1;
    }
}

/**
*
*/
int readDatabaseFromNvmToHeap()
{
    /* Is Attribute database loaded in Heap in attUIntX_inst ? */
    if ( ( NULL == attUIntX_inst) && ( false == isDatabaseLoadedInHeap) && (true == isDatabaseLoadedInNvm) )
    {
        /* No Database loaded in Heap but it is loaded in the nvm.*/
        databaseSize = 0;
        /* Database elements offset in bytes.*/
        UInt16_t offset = 0;

        if (-1 == readNvm(&databaseSize, offset, sizeof( UInt32_t )) )
        {
            /* NVM read failure! */
            return -1;
        }

        if (databaseSize > 0)
        {
            offset += sizeof(UInt32_t);
            attributeSize = (UInt32_t*)malloc(databaseSize * sizeof(UInt32_t));
            if (NULL == attributeSize)
            {
                return -1;
            }
            if (-1 == readNvm(attributeSize, offset, databaseSize * sizeof(UInt32_t)))
            {
                /* NVM read failure! */
                return -1;
            }

            printData(attributeSize, databaseSize, uint32PtrType);

            offset += databaseSize * sizeof(UInt32_t);
            attributeOffset = (UInt8_t*)malloc(databaseSize * sizeof(UInt8_t));
            if (NULL == attributeOffset)
            {
                return -1;
            }
            if (-1 == readNvm(attributeOffset, offset, databaseSize * sizeof(UInt8_t)))
            {
                /* NVM read failure! */
                return -1;
            }

            printData(attributeOffset, databaseSize, uint8PtrType);
            offset += databaseSize * sizeof(UInt8_t);
            attributeType = (UInt8_t*)malloc(databaseSize * sizeof(UInt8_t));
            if (NULL == attributeType)
            {
                return -1;
            }
            if (-1 == readNvm(attributeType, offset, databaseSize * sizeof(UInt8_t)))
            {
                /* NVM read failure! */
                return -1;
            }

            printData(attributeType, databaseSize, uint8PtrType);
            offset += databaseSize * sizeof(UInt8_t);            
            /* Set the global variable for the offset for the first attribute in the database */
            firstAttributeOffset = offset;

            /* data size offset = sizeof(id) + sizeof(option) + sizeof(length) + sizeof(crc) */
            UInt32_t attributeDataSizeOffset = sizeof(gpNvm_AttrId) + sizeof(UInt32_t) + sizeof(UInt8_t);
            UInt32_t attributeDataLength = 0;            
            attUIntX_inst = (attUIntX_t**)malloc(databaseSize * sizeof(attUIntX_t*));
            if (attUIntX_inst == NULL)
            {
                return -1;
            }
            /* Start loading database in the heap from the nvm*/
            isDatabaseLoadingInProgress = true;

            for (UInt32_t i = 0; i < databaseSize; i++)
            {
                attUIntX_inst[i] = (attUIntX_t*)malloc(sizeof(attUIntX_t));
                if (attUIntX_inst[i] == NULL)
                {
                    return -1;
                }
                /* Check crc  data type in order to extract attribute data length */
                switch (attributeType[i])
                {
                    case uint8PtrType:
                        attributeDataLength = attributeSize[i] - attributeDataSizeOffset - sizeof(UInt8_t);
                        attUIntX_inst[i]->data = (UInt8_t*)malloc(attributeDataLength * sizeof(UInt8_t));
                        attUIntX_inst[i]->crc = (UInt8_t*)malloc(sizeof(UInt8_t));
                        break;
                    case uint16PtrType:
                        attributeDataLength = attributeSize[i] - attributeDataSizeOffset - sizeof(UInt16_t);
                        attUIntX_inst[i]->data = (UInt16_t*)malloc(attributeDataLength * sizeof(UInt16_t));
                        attUIntX_inst[i]->crc = (UInt16_t*)malloc(sizeof(UInt16_t));
                        break;
                    case uint32PtrType:
                        attributeDataLength = attributeSize[i] - attributeDataSizeOffset - sizeof(UInt32_t);
                        attUIntX_inst[i]->data = (UInt32_t*)malloc(attributeDataLength * sizeof(UInt32_t));
                        attUIntX_inst[i]->crc = (UInt32_t*)malloc(sizeof(UInt32_t));
                        break;
                    default:
                        /* Unknown Data type */
                        return -1;
                        break;
                }
                if (NULL == attUIntX_inst[i]->data)
                {
                    return -1;
                }
                if (NULL == attUIntX_inst[i]->crc)
                {
                    return -1;
                }
                if (-1 == readDatabaseAttribute(attUIntX_inst[i], i))
                {
                    /* Failure to read attribute from NVM. */
                    return -1;
                }
            }
        }
        else
        {
            /* No entry in the database. */
            return -1;
        }
        /* Database loading completed.*/
        isDatabaseLoadingInProgress = false;
        /* Attributes Database is loaded in Heap. */
        isDatabaseLoadedInHeap = true;        
        return 0;
    }
    else
    {
        /* Attribute database is already loaded in Heap in attUIntX_inst
        *  you can read write/read in the NVM. 
        */
        return -1;
    }
}

gpNvm_Result gpNvm_setAttribute(gpNvm_AttrId attrId, UInt8_t pLenght, void* pValue)
{    
    /* Is Attribute database loaded in the Nvm ? */
    if (( true == isDatabaseLoadedInNvm) && ( true == isDatabaseLoadedInHeap))
    {
        if (pLenght <= attUIntX_inst[attrId]->length)
        {
            /* Attribute offset  for the data in bytes.*/
            UInt16_t offset = 0;
            /* Attribute data size + crc size in bytes.*/
            UInt8_t dataSize = 0;
            UInt8_t* buffer = NULL;

            /* id offset */
            offset = 0 ;
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
                buffer = (UInt8_t*)calloc( (attUIntX_inst[attrId]->length + 1) * sizeof(UInt8_t), sizeof(UInt8_t));
                if (buffer == NULL)
                {
                    return -1;
                }
                /* Copy new attribute value in the buffer. */
                memcpy(buffer, (UInt8_t*)pValue, pLenght * sizeof(UInt8_t));
                /* Update the new attribute value in the RAM array. */
                memcpy(attUIntX_inst[attrId]->data, (UInt8_t*)pValue, pLenght * sizeof(UInt8_t));
                dataSize = attUIntX_inst[attrId]->length * sizeof(UInt8_t);
                /* copy CRC */                 
                crcCheck((UInt8_t*)(pValue), (UInt8_t*)((attUIntX_inst[attrId])->crc), pLenght, uint8PtrType);
                memcpy(buffer + dataSize, (UInt8_t*)attUIntX_inst[attrId]->crc, sizeof(UInt8_t));
                dataSize += sizeof(UInt8_t);
                break;
            case uint16PtrType:
                /* Allocate buffer with size attUIntX_inst[attrId]->length and not pLenght.
                *  Maximum Attribute data field size in the database cannot change in runtime.*/
                buffer = (UInt8_t*)calloc((attUIntX_inst[attrId]->length + 1) * sizeof(UInt16_t), sizeof(UInt16_t));
                if (buffer == NULL)
                {
                    return -1;
                }
                /* Copy new attribute value in the buffer. */
                memcpy(buffer, (UInt8_t*)pValue, pLenght * sizeof(UInt16_t));
                /* Update the new attribute value in the RAM array. */
                memcpy(attUIntX_inst[attrId]->data, (UInt8_t*)pValue, pLenght * sizeof(UInt16_t));
                dataSize = attUIntX_inst[attrId]->length * sizeof(UInt16_t);
                /* copy CRC */                
                crcCheck((UInt16_t*)(pValue), (UInt16_t*)((attUIntX_inst[attrId])->crc), pLenght, uint8PtrType);
                memcpy(buffer + dataSize, (UInt8_t*)attUIntX_inst[attrId]->crc, sizeof(UInt16_t));
                dataSize += sizeof(UInt16_t);
                break;
            case uint32PtrType:
                /* Allocate buffer with size attUIntX_inst[attrId]->length and not pLenght.
                *  Maximum Attribute data field size in the database cannot change in runtime.*/
                buffer = (UInt8_t*)calloc((attUIntX_inst[attrId]->length + 1) * sizeof(UInt32_t), sizeof(UInt32_t));
                if (buffer == NULL)
                {
                    return -1;
                }
                /* Copy mew attribute value in the buffer. */
                memcpy(buffer, (UInt8_t*)pValue, pLenght * sizeof(UInt32_t));
                memcpy(attUIntX_inst[attrId]->data, (UInt8_t*)pValue, pLenght * sizeof(UInt32_t));
                dataSize = attUIntX_inst[attrId]->length * sizeof(UInt32_t);
                /* copy CRC */
                crcCheck((UInt32_t*)(pValue), (UInt32_t*)((attUIntX_inst[attrId])->crc), pLenght, uint8PtrType);
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
            printData((UInt8_t*)(buffer), dataSize, uint8PtrType);
#endif            
            if (-1 == writeNvm(buffer, firstAttributeOffset+attributeOffset[attrId] + offset, dataSize))
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
        /* Attribute database is not loaded in the Nvm and Heap.
        *  No database in nvm to write!
        */
        return -1;
    }
}

gpNvm_Result gpNvm_getAttribute(gpNvm_AttrId attrId, UInt8_t* pLenght, void* pValue)
{
    /* Is Attribute database loaded in the Nvm  and Heap ? */
    if ((true == isDatabaseLoadedInNvm) && (true == isDatabaseLoadedInHeap))
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
        /* Attribute database is not loaded in the Nvm and Heap.
        *  No database in nvm and ram to read!
        */
        return -1;
    }
}

/** @brief Writes an attribute in the NVM Database.
 *         In order to use this function , the database
 *         should be loaded both in NVM ( writeDatabaseToNvm() )
 *         and Ram ( loadDatabaseInHeap() ).
 *  @param[in]  attribute    Pointer to attUIntX_t type. It contains the attrinute's data which
 *                           they was read from the nvm Database. attUIntX_t pointer is used also
 *                           for loading the database in Ram.
 *  @param[in]  attributeNum Attribute ID.
 *  @return                  0 for Success and -1 for failure.
 */
static int writeDatabaseAttribute(attUIntX_t* const attribute , UInt8_t attributeNum)
{   
    /* Is Attribute database loaded in the Heap ? 
    *  Use this to create the Database in the NVM.
    */
    if ( ( (true == isDatabaseLoadedInHeap) || (isDatabaseLoadingInProgress) ) && (NULL != attUIntX_inst) )
    {
        attUIntX_t* attributeLocal = attribute;
        UInt16_t offset = 0;

        UInt8_t* buffer = (UInt8_t*)malloc(attributeSize[attributeNum] * sizeof(UInt8_t));
        if (buffer == NULL)
        {
            return -1;
        }
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
            crcCheck((UInt8_t*)(attributeLocal->data), (UInt8_t*)(attributeLocal->crc), attributeLocal->length, uint8PtrType);
            memcpy(buffer + offset, attributeLocal->crc, sizeof(UInt8_t));
            offset += sizeof(UInt8_t);
            break;
        case uint16PtrType:
            memcpy(buffer + offset, (UInt8_t*)(attributeLocal->data), attributeLocal->length * sizeof(UInt16_t));
            offset += attributeLocal->length * sizeof(UInt16_t);
            /* copy CRC */
            crcCheck((UInt16_t*)(attributeLocal->data), (UInt16_t*)(attributeLocal->crc), attributeLocal->length, uint16PtrType);
            memcpy(buffer + offset, (UInt8_t*)(attributeLocal->crc), sizeof(UInt16_t));
            offset += sizeof(UInt16_t);
            break;
        case uint32PtrType:
            memcpy(buffer + offset, (UInt8_t*)(attributeLocal->data), attributeLocal->length * sizeof(UInt32_t));
            offset += attributeLocal->length * sizeof(UInt32_t);
            /* copy CRC */
            crcCheck((UInt32_t*)(attributeLocal->data), (UInt32_t*)(attributeLocal->crc), attributeLocal->length, uint32PtrType);
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
        printData((UInt8_t*)(buffer), attributeSize[attributeNum], uint8PtrType);
#endif
        if (-1 == writeNvm(buffer, firstAttributeOffset + attributeOffset[attributeNum], attributeSize[attributeNum]))
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
    else
    {
        /* Attribute database is not loaded in the Nvm and Heap.
        *  No database in nvm and ram to read!
        */
        return -1;
    }
}

/** @brief Reads an attribute from the NVM Database.
 *         In order to use this function , the database
 *         should be loaded both in NVM ( writeDatabaseToNvm() )
 *         and Ram ( loadDatabaseInHeap() ).
 *  @param[in]  attribute    Pointer to attUIntX_t type. It contains the attrinute's data which
 *                           they was read from the nvm Database. attUIntX_t pointer is used also
 *                           for loading the database in Ram.
 *  @param[in]  attributeNum Attribute ID.
 *  @return                  0 for Success and -1 for failure.
 */
static int readDatabaseAttribute(attUIntX_t* const attribute, UInt8_t attributeNum)
{
     /* Is the Attribute database loaded in the Nvm  and Heap ? */
    if ((true == isDatabaseLoadedInNvm) && ( (true == isDatabaseLoadedInHeap) || (true == isDatabaseLoadingInProgress) ) && (NULL != attUIntX_inst))
    {
        attUIntX_t* attributeLocal = attribute;
        UInt16_t offset = 0;
        UInt32_t crcLocal = 0;

        assert(0 != attributeSize[attributeNum]);
        if ( 0 == attributeSize[attributeNum])
        {
            return -1;
        }
        UInt8_t* buffer = (UInt8_t*)malloc(attributeSize[attributeNum] * sizeof(UInt8_t));
        if (buffer == NULL)
        {
            return -1;
        }

        if (-1 == readNvm(buffer, firstAttributeOffset + attributeOffset[attributeNum], attributeSize[attributeNum]))
        {
            free(buffer);
            buffer = NULL;
            /* NVM write failure! */
            return -1;
        }
#ifdef _ENABLE_DEBUG_
        //printData((UInt8_t*)(buffer), attributeSize[attributeNum], uint8PtrType);
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
            crcCheck((UInt8_t*)(attributeLocal->data), (UInt8_t*)(&crcLocal), attributeLocal->length, uint8PtrType);
            assert(crcLocal == *((UInt8_t*)(attributeLocal->crc)));
            if (crcLocal != *((UInt8_t*)(attributeLocal->crc)))
            {
                free(buffer);
                buffer = NULL;
                /* Error reading Data from the NVM*/
                return -1;
            }
#ifdef _ENABLE_DEBUG_
            printData((UInt8_t*)(attributeLocal->data), attributeLocal->length, uint8PtrType);
#endif
            break;
        case uint16PtrType:
            memcpy(attributeLocal->data, buffer + offset, attributeLocal->length * sizeof(UInt16_t));
            offset += attributeLocal->length * sizeof(UInt16_t);
            /* copy CRC */
            memcpy(attributeLocal->crc, buffer + offset, sizeof(UInt16_t));
            offset += sizeof(UInt16_t);

            /* Check CRC error in Data after reading from NVM  */
            crcCheck((UInt16_t*)(attributeLocal->data), (UInt16_t*)(&crcLocal), attributeLocal->length, uint16PtrType);
            assert(crcLocal == *((UInt16_t*)(attributeLocal->crc)));
            if (crcLocal != *((UInt16_t*)(attributeLocal->crc)))
            {
                free(buffer);
                buffer = NULL;
                /* Error reading Data from the NVM*/
                return -1;
            }
#ifdef _ENABLE_DEBUG_
            printData((UInt16_t*)(attributeLocal->data), attributeLocal->length, uint16PtrType);
#endif
            break;
        case uint32PtrType:
            memcpy(attributeLocal->data, buffer + offset, attributeLocal->length * sizeof(UInt32_t));
            offset += attributeLocal->length * sizeof(UInt32_t);
            /* Copy CRC */
            memcpy(attributeLocal->crc, buffer + offset, sizeof(UInt32_t));
            offset += sizeof(UInt32_t);

            /* Check CRC error in Data after reading from NVM  */
            crcCheck((UInt32_t*)(attributeLocal->data), (UInt32_t*)(&crcLocal), attributeLocal->length, uint32PtrType);
#if 0
            assert(crcLocal == *((UInt32_t*)(attributeLocal->crc)));
            if (crcLocal != *((UInt32_t*)(attributeLocal->crc)))
            {
                free(buffer);
                buffer = NULL;
                /* Error reading Data from the NVM*/
                return -1;
            }
#endif
#ifdef _ENABLE_DEBUG_
            printData((UInt32_t*)(attributeLocal->data), attributeLocal->length, uint32PtrType);
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

