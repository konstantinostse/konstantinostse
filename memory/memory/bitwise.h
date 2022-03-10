/**
 * @file bitwise.h
 *
 * @brief bitwise function implementation.
 *
 *
 */

#ifndef __BITWISE__
#define __BITWISE__


#include <stdint.h>
#include <stdbool.h>


/* Data access macros         */

#define SHIF16(a) ((a)&0x0001? 0:(a)&0x0002? 1:(a)&0x0004? 2:(a)&0x0008? 3: \
                   (a)&0x0010? 4:(a)&0x0020? 5:(a)&0x0040? 6:(a)&0x0080? 7: \
                   (a)&0x0100? 8:(a)&0x0200? 9:(a)&0x0400?10:(a)&0x0800?11: \
                   (a)&0x1000?12:(a)&0x2000?13:(a)&0x4000?14:           15)

#define SHIF32(a)((a)&0x00000001? 0:(a)&0x00000002? 1:(a)&0x00000004? 2:(a)&0x00000008? 3:\
                  (a)&0x00000010? 4:(a)&0x00000020? 5:(a)&0x00000040? 6:(a)&0x00000080? 7:\
                  (a)&0x00000100? 8:(a)&0x00000200? 9:(a)&0x00000400?10:(a)&0x00000800?11:\
                  (a)&0x00001000?12:(a)&0x00002000?13:(a)&0x00004000?14:(a)&0x00008000?15:\
                  (a)&0x00010000?16:(a)&0x00020000?17:(a)&0x00040000?18:(a)&0x00080000?19:\
                  (a)&0x00100000?20:(a)&0x00200000?21:(a)&0x00400000?22:(a)&0x00800000?23:\
                  (a)&0x01000000?24:(a)&0x02000000?25:(a)&0x04000000?26:(a)&0x08000000?27:\
                  (a)&0x10000000?28:(a)&0x20000000?29:(a)&0x40000000?30:               31)

#define SetWord8(a,d)  (* ( volatile uint8_t*) (a)=(d) )
#define SetWord16(a,d) (* ( volatile uint16_t*)(a)=(d) )
#define SetWord32(a,d) (* ( volatile uint32_t*)(a)=(d) )
#define SetWord64(a,d) (* ( volatile uint64_t*)(a)=(d) )

#define GetWord8(a)    (* ( volatile uint8_t*) (a) )
#define GetWord16(a)   (* ( volatile uint16_t*)(a) )
#define GetWord32(a)   (* ( volatile uint32_t*)(a) )
#define GetWord64(a)   (* ( volatile uint64_t*)(a) )

#define SetByte(a,d) SetWord8((a),(d))
#define GetByte(a)   GetWord8((a))

#define SetBits16(a,f,d) ( SetWord16( (a), (GetWord16(a)&(~(uint16_t)(f))) | (((uint16_t)(d))<<SHIF16((f))) ))
#define SetBits32(a,f,d) ( SetWord32( (a), (GetWord32(a)&(~(uint32_t)(f))) | (((uint32_t)(d))<<SHIF32((f))) ))

#define GetBits16(a,f) ( (GetWord16(a)&( (uint16_t)(f) )) >> SHIF16(f) )
#define GetBits32(a,f) ( (GetWord32(a)&( (uint32_t)(f) )) >> SHIF32(f) )

// Macro's used in the recommended_settings function
// All fields are combined into a single mask.
#define SetWordMasked16(a,m,d) SetWord16(a, (GetWord16(a)&~m) | (d&m))
#define SetWordMasked32(a,m,d) SetWord32(a, (GetWord32(a)&~m) | (d&m))

/*
*   bitNum  0-15 bits 
* 
*/

inline void SetBit16(uint16_t* const num, uint8_t const bitNum, bool const bitValue)
{
    if (1 == bitValue)
    {
        *num = ((0 >= bitNum) && (15 < bitNum)) ? *num : (*num | (0x0001 << (bitNum)));
    }
    else
    {
        *num = ((0 >= bitNum) && (15 < bitNum)) ? *num : (*num & (~(0x0001 << (bitNum))) );
    }
}

#endif /* __BITWISE__ */