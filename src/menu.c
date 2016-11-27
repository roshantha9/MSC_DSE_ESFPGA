/************************************************************************
 * @file	menu.c
 * @author  106033467
 * @version 1.0
 *
 * @brief   This file implements the Menu feature of the system, helps to 
 *			maintain the current menu index.
 *
 *  Functionality:
 *  	- Increment/Decrement Menu index
 *		- Initialise Menu index
 ************************************************************************/

//----------------------------------------------------------------------
// SYSTEM INCLUDES
//----------------------------------------------------------------------
#include "menu.h"


//----------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------
static int intMenuIx=DCLOCK_MENU_START_IX;


//----------------------------------------------------------------------
// INCREMENT/DECREMENT MENU INDEX
//----------------------------------------------------------------------
/***********************************************************
 *  @fn  	MENU_IncrementMenuIx
 *  @brief 	Increments the Menu index.
 *  @param 	None
 *  @return void
 ***********************************************************/ 
void MENU_IncrementMenuIx(){
	intMenuIx++;
	
	if(intMenuIx>DCLOCK_MENU_MAX_IX){
		intMenuIx=DCLOCK_MENU_START_IX;
	}	
}

/***********************************************************
 *  @fn  	MENU_DecrementMenuIx
 *  @brief 	Decrements the Menu index.
 *  @param 	None
 *  @return void
 ***********************************************************/ 
void MENU_DecrementMenuIx(){
	intMenuIx--;
		
	if(intMenuIx<DCLOCK_MENU_START_IX){
		intMenuIx=DCLOCK_MENU_MAX_IX;
	}
}

//----------------------------------------------------------------------
// GET/SET MENU INDEX
//----------------------------------------------------------------------
/***********************************************************
 *  @fn  	MENU_GetMenuIx
 *  @brief 	Returns the Current Menu Index
 *  @param 	None
 *  @return int intMenuIx
 ***********************************************************/ 
 int MENU_GetMenuIx(){
	return intMenuIx;
}

/***********************************************************
 *  @fn  	MENU_GetMenuIx
 *  @brief 	Set the Current Menu Index.
 *  @param 	int intMIx (Value to be set to)
 *  @return void
 ***********************************************************/ 
void MENU_SetMenuIx(int intMIx){
	intMenuIx=intMIx;
}


//----------------------------------------------------------------------
// MENU INTERNAL RESET
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	MENU_Initialise
 *  @brief  initialises all global variables
 *  @param  None
 *  @return void
 **********************************************************/
void MENU_Initialise(){
	
	// initialise all global vars 	
	intMenuIx=DCLOCK_MENU_START_IX;	
}






