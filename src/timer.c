/************************************************************************
 * @file	timer.c
 * @author  106033467
 * @version 1.0
 *
 * @brief This file implements the Timer functionality of the system
 *
 *  Functionality:
 *  	- Count Down timer (granularity - seconds)
 * 		- Set Timer initial value
 *		- Activate/Inactivate Timer
 *		- Notify of Timer Expiry via buzzer(audibly) and led(visual)
 ************************************************************************/

//-----------------------------------------------------------------------
// USER INCLUDE FILES
//-----------------------------------------------------------------------
#include "timer.h"
#include "sec_timer_gated.h"
#include "menu.h"
#include "pbuzzer.h"
#include "common.h"

//-----------------------------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------------------------
// current running timer values
static Xint8 intTimerSec=0;
static Xint8 intTimerMins=0;
static Xint8 intTimerHours=0;
static Xint8 intTimerStatus=0;	//0=active, 1=inactive

// timer values, before the countdown
static Xint8 intInitTimerSec=0;
static Xint8 intInitTimerMins=0;
static Xint8 intInitTimerHours=0;

// boolean to check if timer nofitifcation is on/off
static Xint8 intTimerNotificationStatus=0;


 /**********************************************************
 *  @fn  	TIMER_GetDisplayString
 *  @brief 	Populates the character array with the timer 
 *			countdown time. Converts a given Time unit in 
 *			Integer to Char Array
 *		 	   LCD (16 chars width): [0123456789012345]
 *		 format of the time string : 'HH:MM:SS	      '.
 *
 *  @param 	char* strFormattedTime, a pointer to a char 
 *			array that will be populated with a formatted 
 * 			time string once the function returns
 *  @return void
 ***********************************************************/
void TIMER_GetDisplayString(char* strFormattedTime){
		
	// 	   LCD (16 chars width):   [0123456789012345]
	// format of the time string : 'HH:MM:SS    |OFF'
	
	// format hours	
	strFormattedTime[0]=(char)((intTimerHours/10)+48);	
	strFormattedTime[1]=(char)((intTimerHours%10)+48);
	
	// colon seperator
	strFormattedTime[2]=':';
	
	// format mins
	strFormattedTime[3]=(char)((intTimerMins/10)+48);	
	strFormattedTime[4]=(char)((intTimerMins%10)+48);
	
	// colon seperator
	strFormattedTime[5]=':';
	
	// format secs
	strFormattedTime[6]=(char)((intTimerSec/10)+48);	
	strFormattedTime[7]=(char)((intTimerSec%10)+48);
	
	// empty spaces
	strFormattedTime[8]=' ';
	strFormattedTime[9]=' ';
	strFormattedTime[10]=' ';
	strFormattedTime[11]=' ';
	strFormattedTime[12]=' ';
	strFormattedTime[13]=' ';
	strFormattedTime[14]=' ';
	strFormattedTime[15]=' ';	
	strFormattedTime[16]='\0';
}

//----------------------------------------------------------------------
// INCREMENT/DECREMENT TIMER VALUES
//----------------------------------------------------------------------

 /**********************************************************
 *  @fn  	TIMER_IncrementSecs
 *  @brief 	used when user, sets the initial timer seconds 
 * 			value calculate minutes, hours accordngly
 *  @param  None
 *  @return void
 **********************************************************/
void TIMER_IncrementSecs(){
	
	intTimerSec++;	
	if(intTimerSec>=60){		
		intTimerSec = 0;
		intTimerMins++;
	}
	if(intTimerMins>=60){
		intTimerMins = 0;
		intTimerHours++;
	}
	if(intTimerHours>=24){
		intTimerHours=0;
	}
}

 /**********************************************************
 *  @fn  	TIMER_IncrementMins
 *  @brief 	used when user, sets the initial timer minutes
 * 			value calculate hours accordngly
 *  @param  None
 *  @return void
 **********************************************************/
void TIMER_IncrementMins(){
	
	intTimerMins++;	
	if(intTimerMins>=60){
		intTimerMins = 0;
		intTimerHours++;
	}
	if(intTimerHours>=24){
		intTimerHours=0;
	}
}

 /**********************************************************
 *  @fn  	TIMER_IncrementHours
 *  @brief 	used when user, sets the initial timer hours
 * 			set upper bound = 24
 *  @param  None
 *  @return void
 **********************************************************/
void TIMER_IncrementHours(){
	
	intTimerHours++;	
	if(intTimerHours>=24){
		intTimerHours=0;
	}
}

 /**********************************************************
 *  @fn  	TIMER_DecrementSecs
 *  @brief 	used for counting down, set mins, hours 
 *			accordingly.
 *			Time Ranges:
 *			Sec  : 59 to 0
 *			Mins : 59 to 0
 *			Hours: 24 to 0
 *			
 *  @param  None
 *  @return void
 **********************************************************/
void TIMER_DecrementSecs(){
	
	// only start decrementing the seconds, when
	// the timer is enabled.
	if(intTimerStatus == DCLOCK_TIMER_ENABLED){
		intTimerSec--;	
		if(intTimerSec<0){		
			intTimerSec = 59;
			
			intTimerMins--;			
			if(intTimerMins<0){
				intTimerMins = 59;
				
				intTimerHours--;				
				if(intTimerHours<=0){
					intTimerHours=0;
				}				
			}			
		}		
	}
}

//----------------------------------------------------------------------
// TIMER GETTER FUNCTIONS
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	TIMER_GetSecs
 *  @brief 	get seconds value
 *  @param  None
 *  @return int intTimerSec
 **********************************************************/
Xint8 TIMER_GetSecs(){
	return intTimerSec;
}

/**********************************************************
 *  @fn  	TIMER_GetMins
 *  @brief 	get mins value
 *  @param  None
 *  @return int intTimerMins
 **********************************************************/
Xint8 TIMER_GetMins(){
	return intTimerMins;
}

/**********************************************************
 *  @fn  	TIMER_GetHours
 *  @brief 	get hours value
 *  @param  None
 *  @return int intTimerHours
 **********************************************************/
Xint8 TIMER_GetHours(){
	return intTimerHours;
}

/**********************************************************
 *  @fn  	TIMER_GetStatus
 *  @brief 	get timer status value
 *  @param  None
 *  @return int intTimerStatus
 **********************************************************/
Xint8 TIMER_GetStatus(){
	return intTimerStatus;
}

//----------------------------------------------------------------------
// TIMER SETTER FUNCTIONS
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	TIMER_SetSecs
 *  @brief 	set new seconds value
 *  @param  int intNewSec
 *  @return void
 **********************************************************/
void TIMER_SetSecs(Xint8 intNewSec){
	intTimerSec = intNewSec;
}

/**********************************************************
 *  @fn  	TIMER_SetMins
 *  @brief 	set new mins value
 *  @param  int intNewMins
 *  @return void
 **********************************************************/
void TIMER_SetMins(Xint8 intNewMins){
	intTimerMins = intNewMins;
}

/**********************************************************
 *  @fn  	TIMER_SetHours
 *  @brief 	set new hours value
 *  @param  int intNewHours
 *  @return void
 **********************************************************/
void TIMER_SetHours(Xint8 intNewHours){
	intTimerHours = intNewHours;
}


//----------------------------------------------------------------------
//  STATUS SETUP/NOTIFICATION FUNCTIONS 
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	TIMER_SetStatus
 *  @brief 	check the dip switch value and set timer 
 * 			status accordingly, the gated timer will also
 *			need to be stopped/started accordingly.
 *  @param  Xuint32 intDipSwVal	(dipswitch value)
 *  @param 	XGpio GpioOutputLEDAdr (led base address)
 *  @param  Xuint32 buzzer (buzzer base address)
 *  @param  Xuint32 sec_gated_timer_baddr (counter base address)
 *  @return void
 **********************************************************/
void TIMER_SetStatus(Xuint32 intDipSwVal, XGpio GpioOutputLEDAdr, Xuint32 buzzer, Xuint32 sec_gated_timer_baddr){
		
	enum DeviceMenuItem MIx;
	
	if(ISBITSET(intDipSwVal,5)){		
		// check if user is in correct menu before starting timer
		// NB: Starting the timer can only be done in the timer screen
		MIx = MENU_GetMenuIx();		
		if(MIx == DCLOCK_MENU_TIMER){
			
			// set timer
			intTimerStatus = DCLOCK_TIMER_ENABLED;
		
			// save the timer values, so we can restore them back 
			// after the countdown has finished
			TIMER_SaveTimerValues();
			
			// enable the gated seconds timer
			SEC_TIMER_GATED_mWriteSlaveReg1(sec_gated_timer_baddr,0,TIMERINT_START);
		}		
	}
	else{
		
		// NB: Stopping the timer can be done in any screen
	
		// not set timer, switch off any ative notifications
		intTimerStatus = DCLOCK_TIMER_DISABLED;	
		TIMER_SwitchOffNotification(GpioOutputLEDAdr, buzzer);		
		
		// disable the gated seconds timer
		SEC_TIMER_GATED_mWriteSlaveReg1(sec_gated_timer_baddr,0,TIMERINT_STOP);		
	}
}

/**********************************************************
 *  @fn  	TIMER_ShowExpiryNotification
 *  @brief 	check timer status and if timer has expired
 * 			i.e: reached 00:00:00, then switch on led
 * 			status accordingly and stop the timer interrupt
 *  @param  XGpio GpioOutputLEDAdr	(led base address)
 *  @param  Xuint32 buzzer (buzzer base address)
 *  @param  Xuint32 sec_gated_timer_baddr (counter base address)
 *  @return int (0 or 1)
 **********************************************************/
int TIMER_ShowExpiryNotification(XGpio GpioOutputLEDAdr, Xuint32 buzzer, Xuint32 sec_gated_timer_baddr){
	
	Xuint32 intLedStatus=0;
	
	// check if timer has counted down to 00:00:00, 
	// and if timer enabled=true then switch on led
	if((intTimerMins==0) &&
	   (intTimerHours==0) &&
	   (intTimerSec==0)){
		   
	   if(intTimerStatus==DCLOCK_TIMER_ENABLED){
			// switch on timer notifications
			TIMER_SwitchOnNotification(GpioOutputLEDAdr, buzzer);		
			
			// disable the gated seconds timer
			SEC_TIMER_GATED_mWriteSlaveReg1(sec_gated_timer_baddr,0,TIMERINT_STOP);		
			
			return 1;	// return true - yes set timer expiry notification;
	   }
	   else{
		   return 0;
	   }	
	}
	else{
		return 0; // return false - timer, not expired/inactive 
	}
}

/**********************************************************
 *  @fn  	TIMER_SwitchOffNotification
 *  @brief 	Switch off all notifications related to the timer
 *			(LED + buzzer)
 *  @param  XGpio GpioOutputLEDAdr (led address)
 *  @param  Xuint32 buzzer (buzzer base address)
 *  @return void
 **********************************************************/
void TIMER_SwitchOffNotification(XGpio GpioOutputLEDAdr, Xuint32 buzzer){	
	Xuint32 intLedStatus=0;	
	
	//intTimerNotificationStatus=0;
	
	// get alarm led value
	intLedStatus = LED_GetTimerOFF();
	
	// write out to gpio-leds
	XGpio_DiscreteWrite(&GpioOutputLEDAdr, 1, intLedStatus);	
	
	// stop the buzzer
	PBUZZER_mWriteSlaveReg1(buzzer,0, BUZZER_START);		
}

/**********************************************************
 *  @fn  	TIMER_SwitchOnNotification
 *  @brief 	Switch on all notifications related to the timer
 *			(LED + buzzer)
 *  @param  XGpio GpioOutputLEDAdr (led address)
 *  @param  Xuint32 buzzer (buzzer base address)
 *  @return void
 **********************************************************/
void TIMER_SwitchOnNotification(XGpio GpioOutputLEDAdr, Xuint32 buzzer){	
	Xuint32 intLedStatus=0;	
	
	//intTimerNotificationStatus=1;
	
	// get alarm led value
	intLedStatus = LED_GetTimerON();
	
	// write out to gpio-leds
	XGpio_DiscreteWrite(&GpioOutputLEDAdr, 1, intLedStatus);
	
	// start the buzzer
	PBUZZER_mWriteSlaveReg1(buzzer,0, BUZZER_START);				
}


//----------------------------------------------------------------------
//  INITIALISE/SAVE/RESTORE GLOBAL TIMER VALUES
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	TIMER_Initialise
 *  @brief  initialises all global variables.
 *  @param  None
 *  @return void
 **********************************************************/
void TIMER_Initialise(){
	intTimerSec=0;
	intTimerMins=0;
	intTimerHours=0;
	intTimerStatus=0;	//0=active, 1=inactive
}

/**********************************************************
 *  @fn  	TIMER_SaveTimerValues
 *  @brief 	Used to keep track of the set timer values,
 *			before the countdown began
 *  @param  None
 *  @return void
 **********************************************************/
void TIMER_SaveTimerValues(){
	intInitTimerSec   = intTimerSec;
	intInitTimerMins  = intTimerMins;
	intInitTimerHours = intTimerHours;
}

/**********************************************************
 *  @fn  	TIMER_RestoreInitialTimerValues
 *  @brief 	Able to restore the timer face values back to
 *			the original values (before countdown).
 *  @param  None
 *  @return void
 **********************************************************/
void TIMER_RestoreInitialTimerValues(){
	intTimerSec   = intInitTimerSec;
	intTimerMins  = intInitTimerMins;
	intTimerHours = intInitTimerHours;
}

