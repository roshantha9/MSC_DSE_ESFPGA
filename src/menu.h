/************************************************************************
 * @file	menu.h
 * @author  106033467
 * @version 1.0
 *
 * @brief 	Contains function prototype defines and constant defines
 *		  	related to menu.c
 *
 ************************************************************************/
#ifndef MENU_H_GUARD
#define MENU_H_GUARD

//----------------------------------------------------------------------
// SYSTEM INCLUDES
//----------------------------------------------------------------------
#include "xbasic_types.h"

//----------------------------------------------------------------------
// CONSTANT DEFINES
//----------------------------------------------------------------------
#define DCLOCK_MENU_START_IX 	1
#define DCLOCK_MENU_MAX_IX		4

//----------------------------------------------------------------------
// MACROS
//----------------------------------------------------------------------
#define ISBITSET(variable,position) ((variable) & (1<<(position)))

//----------------------------------------------------------------------
// GLOBAL VARIABLES/ENUMS
//----------------------------------------------------------------------

// Menu Enumerator, can keep adding to this as more functionality is
// added to the system
extern enum DeviceMenuItem {
	 DCLOCK_MENU_CLOCK 		= DCLOCK_MENU_START_IX,
	 DCLOCK_MENU_ALARM 		= DCLOCK_MENU_START_IX+1,
	 DCLOCK_MENU_STOPWATCH 	= DCLOCK_MENU_START_IX+2,
	 DCLOCK_MENU_TIMER 		= DCLOCK_MENU_START_IX+3
 }enumMenu; 

//----------------------------------------------------------------------
// GLOBAL FUNCTIONS - EXTERN
//----------------------------------------------------------------------

extern void MENU_IncrementMenuIx();
extern void MENU_DecrementMenuIx();
extern int MENU_GetMenuIx();
extern void MENU_SetMenuIx(int intMIx);
extern void MENU_Initialise();

#endif
