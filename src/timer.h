/************************************************************************
 * @file	timer.h
 * @author  106033467
 * @version 1.0
 *
 * @brief Contains function prototype defines and constant defines
 *		  related to timer.c
 ************************************************************************/

#ifndef TIMER_H_GUARD
#define TIMER_H_GUARD

//----------------------------------------------------------------------
// SYSTEM INCLUDES
//----------------------------------------------------------------------
#include "xbasic_types.h"
#include "xgpio.h"

//----------------------------------------------------------------------
// MACROS
//----------------------------------------------------------------------
// produce 1 or 0, shift 1 by given positions and perform bitwise AND 
// with supplied variable
#define ISBITSET(variable,position) ((variable) & (1<<(position)))

//----------------------------------------------------------------------
// CONSTANT DEFINES
//----------------------------------------------------------------------
#define DCLOCK_TIMER_ENABLED	1
#define DCLOCK_TIMER_DISABLED	0

// mstimer peripheral, values written to the slave register
#define TIMERINT_STOP		0x00000000
#define TIMERINT_START		0x40000000 

//----------------------------------------------------------------------
// GLOBAL FUNCTIONS
//----------------------------------------------------------------------

// construct display time string
extern void TIMER_GetDisplayString(char* strFormattedTime);

// increment time
extern void TIMER_IncrementSecs();
extern void TIMER_IncrementMins();
extern void TIMER_IncrementHours();

// decrement time
extern void TIMER_DecrementSecs();
extern void TIMER_DecrementMins();
extern void TIMER_DecrementHours();

// getter funcs
extern Xint8 TIMER_GetSecs();
extern Xint8 TIMER_GetMins();
extern Xint8 TIMER_GetHours();

// setter funcs
extern void TIMER_SetSecs(Xint8 intNewSec);
extern void TIMER_SetMins(Xint8 intNewMins);
extern void TIMER_SetHours(Xint8 intNewHours);

// timer status and notification related function
extern void TIMER_SetStatus(Xuint32 intDipSwVal, XGpio GpioOutputLEDAdr,\
							Xuint32 buzzer, Xuint32 sec_gated_timer_baddr);
							
extern int TIMER_ShowExpiryNotification(XGpio GpioOutputLEDAdr,\
							Xuint32 buzzer, Xuint32 sec_gated_timer_baddr);
							
extern void TIMER_SwitchOffNotification(XGpio GpioOutputLEDAdr, Xuint32 buzzer);
extern void TIMER_SwitchOnNotification(XGpio GpioOutputLEDAdr, Xuint32 buzzer);

// initialise Timer locals
extern void TIMER_Initialise();

// save and restore initially set timer values
extern void TIMER_SaveTimerValues();
extern void TIMER_RestoreInitialTimerValues();

//----------------------------------------------------------------------
// LOCAL/HELPER FUNCTIONS
//----------------------------------------------------------------------
static int __Get12Hour(int intHours);

#endif
