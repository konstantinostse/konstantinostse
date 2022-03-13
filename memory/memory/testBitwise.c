#include "testBitwise.h"
#include "attributes.h"
#include "bitwise.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void testBitwise()
{
    uint16_t regVal1 = 0;//(uint16_t)0x8001;
    uint16_t regVal2 = 0x0000;
    uint32_t regVal3 = SHIF32(0x0100);
    
    SetWord16(&regVal1, 0x8010);
    //SHIF16(regVal1);
    int j = 0;
    for (uint16_t i = 0x0001; i < 0x8000; i <<= 1,j++)
    {
        printf(" Print %d : result :0x %x  shift16 : %x i : %.4x \n",j, SetBits16(&regVal1, i, 1), SHIF16(i),i);
        SetWord16(&regVal1, 0x8010);
    }    
    printf(" /---------------------------------------------------/\n");
    SetWordMasked16(&regVal1, 0x0FF, 0x0045);
    printf(" Print result :0x%.4x \n", regVal1);
    printf(" /---------------------------------------------------/\n");



    for (int i = 0; i < 18; i++)
    {
        SetBit16(&regVal2, i, 1);
        printf(" Print result %d : 0x%.4x \n",i, regVal2);
    }

    for (int i = 0; i < 18; i++)
    {
        SetBit16(&regVal2, i, 0);
        printf(" Print result %d : 0x%.4x \n", i, regVal2);
    }

}
