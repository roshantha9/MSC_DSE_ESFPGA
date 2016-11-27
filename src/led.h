/************************************************************************
 * @file	led.h
 * @author  106033467
 * @version 1.0
 *
 * @brief 	Contains function prototype defines and constant defines
 *		  	related to led.c
 *
 ************************************************************************/

#ifndef LED_H_GUARD
#define LED_H_GUARD

//----------------------------------------------------------------------
// SYSTEM INCLUDES
//----------------------------------------------------------------------
#include "xbasic_types.h"

//----------------------------------------------------------------------
// CONSTANT DEFINES
//----------------------------------------------------------------------

// individual led light port addresses (setting bits)
#define LED_LIGHT_0  0x80	// 10000000 - light up alarm led
#define LED_LIGHT_1  0x40	// 01000000 - light up timer led
#define LED_LIGHT_2  0x20	// 00100000
#define LED_LIGHT_3  0x10	// 00010000
#define LED_LIGHT_4  0x08	// 00001000
#define LED_LIGHT_5  0x04	// 00000100
#define LED_LIGHT_6  0x02	// 00000010
#define LED_LIGHT_7  0x01	// 00000001

// defines, used for masking off (unsetting bits)
#define LED_UNSET_MASK_LIGHT_0 0x7F		// 01111111 - unset alarm led mask
#define LED_UNSET_MASK_LIGHT_1 0xBF 	// 10111111 - unset timer led mask
#define LED_UNSET_MASK_LIGHT_2 0xDF		// 11011111
#define LED_UNSET_MASK_LIGHT_3 0xEF		// 11101111
#define LED_UNSET_MASK_LIGHT_4 0xF7		// 11110111
#define LED_UNSET_MASK_LIGHT_5 0xFB		// 11111011
#define LED_UNSET_MASK_LIGHT_6 0xFD		// 11111101
#define LED_UNSET_MASK_LIGHT_7 0xFE		// 11111110


//----------------------------------------------------------------------
// GLOBAL FUNCTIONS - EXTERNS
//----------------------------------------------------------------------
extern Xuint32 LED_GetAlarmON();
extern Xuint32 LED_GetAlarmOFF();
extern Xuint32 LED_GetTimerON();
extern Xuint32 LED_GetTimerOFF();

#endif
