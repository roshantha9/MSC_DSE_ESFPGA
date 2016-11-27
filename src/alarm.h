/************************************************************************
 * @file	alarm.h
 * @author  106033467
 * @version 1.0
 *
 * @brief Contains function prototype defines and constant defines
 *		  related to alarm.c
 ************************************************************************/
#ifndef ALARM_H_GUARD
#define ALARM_H_GUARD

//----------------------------------------------------------------------
// SYSTEM INCLUDES
//----------------------------------------------------------------------
#include "xbasic_types.h"
#include "xgpio.h"

//----------------------------------------------------------------------
// CONSTANT DEFINES
//----------------------------------------------------------------------
#define DCLOCK_ALARM_SET				1	// alarm set
#define DCLOCK_ALARM_NSET				0	// alarm not set

#define DCLOCK_ALARM_SNOOZE_SET			1	// snooze set
#define DCLOCK_ALARM_SNOOZE_NSET		0	// snooze not set

#define ALARM_SNOOZE_DELAY_MINS			5	// snooze delay in minutes


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
extern void ALARM_GetDisplayString(char* strFormattedTime);

// increment time
extern void ALARM_IncrementSecs();
extern void ALARM_IncrementMins();
extern void ALARM_IncrementHours();

// decrement time
extern void ALARM_DecrementSecs();
extern void ALARM_DecrementMins();
extern void ALARM_DecrementHours();

// getter funcs
extern int ALARM_GetSecs();
extern int ALARM_GetMins();
extern int ALARM_GetHours();

// setter funcs
extern void ALARM_SetSecs(int intNewSec);
extern void ALARM_SetMins(int intNewMins);
extern void ALARM_SetHours(int intNewHours);


// get/set alarm status
extern void ALARM_SetAlarm(Xuint32 intDipSwVal);
extern int ALARM_GetAlarmStatus();

// alarm notification related functions
extern int ALARM_SwitchONOFFAlarmNotification(XGpio GpioOutputLEDAdr,\
											  Xuint32 buzzer);
											  
extern void ALARM_SwitchOffAlarmNotification(XGpio GpioOutputLEDAdr,\
											 Xuint32 buzzer);
											 
extern void ALARM_SwitchOnAlarmNotification(XGpio GpioOutputLEDAdr,\
											Xuint32 buzzer);

// alarm snooze related functions, set snooze status, get status
extern void ALARM_SetSnooze();
extern int ALARM_GetSnooze();

// alarm reset
extern void ALARM_Initialise();

//----------------------------------------------------------------------
// LOCAL FUNCTIONS - HELPERS
//----------------------------------------------------------------------
static int __Get12Hour(int intHours);

#endif
