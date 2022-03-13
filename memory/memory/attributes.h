/**
 * @file attributes.h
 *
 * @brief Types definitions for the Database attributes.
 */

#ifndef __ATTRIBUTES__
#define __ATTRIBUTES__

/* Database 's data types according API specifications */
enum intType
{
    uint8PtrType  = 0,
    uint16PtrType,
    uint32PtrType
};

/* Database 's data types definitions according API specifications */
typedef unsigned char  UInt8_t;
typedef unsigned short UInt16_t;
typedef unsigned int   UInt32_t;

typedef UInt8_t gpNvm_AttrId;
typedef UInt8_t gpNvm_Result;

/* Database 's structure type definition according API specifications */
/* Structure size offset = sizeof(gpNvm_AttrId)+sizeof(UInt32_t)+sizeof(UInt8_t)+sizeof(intType)
  + sizeof(data) + sizeof(crc); */
typedef struct attUIntX
{
    gpNvm_AttrId id;
    UInt32_t     option;
    UInt8_t      length;
    void*        data;
    void*        crc;
}attUIntX_t;

#endif /* __ATTRIBUTES__ */
