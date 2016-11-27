/************************************************************************
 * @file	misc.c
 * @author  106033467
 * @version 1.0
 *
 * @brief   This file implements the Miscellaneous functions used by the
 *			other application files
 *
 *  Functionality:
 *  	- ASCII to Integer conversion
 ************************************************************************/
 
//----------------------------------------------------------------------
// USER HEADER INCLUDES
//----------------------------------------------------------------------
#include "misc.h"

 
 /***********************************************************
 *  @fn  	MISC_ItoA
 *  @brief 	Converts a given Time unit in Integer to Char 
 *			Array.
 *			Max value : 60 
 * 			Min value : 00
 *  @param 	int intTimeUnit (Integer Time unit)
 *  @param 	char* strTimeUnit (string, conversion result)
 *  @return void
 ***********************************************************/ 
 
void MISC_ItoA(int intTimeUnit, char* strTimeUnit ){
	
	strTimeUnit[0]=(char)((intTimeUnit/10)+48);	
	strTimeUnit[1]=(char)((intTimeUnit%10)+48);	
	
}
