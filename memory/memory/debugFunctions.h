/**
 * @file debugFunctions.h
 *
 * @brief
 */

#ifndef  __DEBUG_FUNCTIONS__
#define __DEBUG_FUNCTIONS__

#include <stdint.h>
#include <stdio.h>
#include "attributes.h"



void printData(UInt8_t* data, UInt8_t length);

void printData(UInt16_t* data, UInt8_t length);

void printData(UInt32_t* data, UInt8_t length);

#endif /* __DEBUG_FUNCTIONS__ */

