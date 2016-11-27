/************************************************************************
 * @file	led.c
 * @author  106033467
 * @version 1.0
 *
 * @brief 	This file implements LED setting/unsetting value management
 *			It basically performs bitwise operations on the current LED
 *			value
 ************************************************************************/
//----------------------------------------------------------------------
// USER HEADER INCLUDES
//----------------------------------------------------------------------
#include "led.h"

//----------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------
static Xuint32 intLEDStatus=0x00;


//----------------------------------------------------------------------
// LED OUTPUT FOR ALARM NOTIFICATION
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	LED_GetAlarmON
 *  @brief  save and return the led value for alarm 
 * 			notification status
 *  @param  None
 *  @return Xuint32 intLEDStatus
 **********************************************************/
Xuint32 LED_GetAlarmON(){	
	intLEDStatus = intLEDStatus | LED_LIGHT_0;	// bitwise OR
	return intLEDStatus;
}

/**********************************************************
 *  @fn  	LED_GetAlarmOFF
 *  @brief  save and return the led value for alarm 
 * 			switched off status	notification status
 *  @param  None
 *  @return Xuint32 intLEDStatus
 **********************************************************/
Xuint32 LED_GetAlarmOFF(){
	// bitwise AND - bit masking
	intLEDStatus = intLEDStatus & LED_UNSET_MASK_LIGHT_0;	
	return intLEDStatus;
}

//----------------------------------------------------------------------
// LED OUTPUT FOR TIMER EXPIRY NOTIFICATION
//----------------------------------------------------------------------
/**********************************************************
 *  @fn  	LED_GetTimerON
 *  @brief  save and return the led value for timer 
 * 			switched on status	notification led value
 *  @param  None
 *  @return Xuint32 intLEDStatus
 **********************************************************/
Xuint32 LED_GetTimerON(){	
	intLEDStatus = intLEDStatus | LED_LIGHT_1;	// bitwise OR
	return intLEDStatus;
}

/**********************************************************
 *  @fn  	LED_GetTimerOFF
 *  @brief  save and return the led value for timer 
 * 			switched off status	notification led value
 *  @param  None
 *  @return Xuint32 intLEDStatus
 **********************************************************/
Xuint32 LED_GetTimerOFF(){	
	// bitwise AND - bit masking
	intLEDStatus = intLEDStatus & LED_UNSET_MASK_LIGHT_1;
	return intLEDStatus;
}
