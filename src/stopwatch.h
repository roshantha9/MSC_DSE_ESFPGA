/************************************************************************
 * @file	stopwatch.h
 * @author  106033467
 * @version 1.0
 *
 * @brief 	Contains function prototype defines and constant defines
 *		  	related to stopwatch.c
 *
 ************************************************************************/
#ifndef STOPWATCH_H_GUARD
#define STOPWATCH_H_GUARD

//----------------------------------------------------------------------
// SYSTEM INCLUDES
//----------------------------------------------------------------------
#include "xbasic_types.h"
#include "xgpio.h"
#include "milisec_timer.h"

//----------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------
#define MS_TIMER_STOP		0x00000000
#define MS_TIMER_START		0x40000000

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
extern void STOPWATCH_GetDisplayString(char* strFormattedTime);

// increment time
extern void STOPWATCH_IncrementMs();

// getter funcs
extern int STOPWATCH_GetMs();
extern int STOPWATCH_GetSecs();
extern int STOPWATCH_GetMins();
extern int STOPWATCH_GetHours();
extern int STOPWATCH_GetStatus();

// setter funcs
extern void STOPWATCH_SetMs(int intNewMs);
extern void STOPWATCH_SetSecs(int intNewSec);
extern void STOPWATCH_SetMins(int intNewMins);
extern void STOPWATCH_SetHours(int intNewHours);

// start/stop/clear stopwatch
extern void STOPWATCH_Start(Xuint32 ms_timer_addr);
extern void STOPWATCH_Stop(Xuint32 ms_timer_addr);
extern void STOPWATCH_Clear(Xuint32 ms_timer_addr);
extern void STOPWATCH_StartStop(Xuint32 ms_timer_addr);

// stopwatch reset
extern void STOPWATCH_Initialise();

#endif
