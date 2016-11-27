/************************************************************************
 * @file	clock.h
 * @author  106033467
 * @version 1.0
 *
 * @brief Contains function prototype defines and constant defines
 *		  related to clock.c
 ************************************************************************/

#ifndef CLOCK_H_GUARD
#define CLOCK_H_GUARD


//----------------------------------------------------------------------
// SYSTEM INCLUDES
//----------------------------------------------------------------------
#include "xbasic_types.h"

//----------------------------------------------------------------------
// CONSTANT DEFINES
//----------------------------------------------------------------------
// constants
#define DCLOCK_FORMAT_24HR			0
#define DCLOCK_FORMAT_12HR			1

//----------------------------------------------------------------------
// MACROS
//----------------------------------------------------------------------
// produce 1 or 0, shift 1 by given positions and perform bitwise AND 
// with supplied variable
#define ISBITSET(variable,position) ((variable) & (1<<(position)))


//----------------------------------------------------------------------
// GLOBAL FUNCTIONS - EXTERNS
//----------------------------------------------------------------------

// construct display time string
extern void CLOCK_GetDisplayString(char* strFormattedTime);

// increment time
extern void CLOCK_IncrementSecs();
extern void CLOCK_IncrementMins();
extern void CLOCK_IncrementHours();

// decrement time
extern void CLOCK_DecrementSecs();
extern void CLOCK_DecrementMins();
extern void CLOCK_DecrementHours();

// getter functions
extern int CLOCK_GetSecs();
extern int CLOCK_GetMins();
extern int CLOCK_GetHours();

// setter functions
extern void CLOCK_SetSecs(int intNewSec);
extern void CLOCK_SetMins(int intNewMins);
extern void CLOCK_SetHours(int intNewHours);

// clock display formating
extern int CLOCK_GetTimeFormat();
//24hr/12hr clock
extern void CLOCK_SetTimeFormat(Xuint32 intDipSwVal);	

// reset clock
extern void CLOCK_Initialise();

//----------------------------------------------------------------------
// LOCAL/STATIC HELPER FUNCTIONS
//----------------------------------------------------------------------

static int __Get12Hour(int intHours);

#endif
