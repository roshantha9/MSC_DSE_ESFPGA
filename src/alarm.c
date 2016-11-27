/************************************************************************
 * @file	alarm.c
 * @author  106033467
 * @version 1.0
 *
 * @brief This file implements the Alarm functionality of the system.
 *
 *  Functionality:
 *  	- Increment/Decrement/Set/Get Alarm values (hh, mm)
 *		- Construct the 2nd Line string for the alarm screen
 *		- Set/Unset Alarm
 *		- Functions to check validity of the alarm
 *		- Switch ON/OFF Alarm notifications
 *		- Set Snooze functionality
 ************************************************************************/

//----------------------------------------------------------------------
// USER HEADER INCLUDES
//----------------------------------------------------------------------
#include "alarm.h"
#include "led.h"
#include "clock.h"
#include "pbuzzer.h"
#include "common.h"

//----------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------
static int intAlarmSec=0;
static int intAlarmMins=0;
static int intAlarmHours=12;	// initially set to noon
static int intAlarmSet=0; 		// 0=not set, 1=set

static int intSnoozePressed=0;	// snooze pressed=1, not pressed=0
static int intSnoozeDelay=0;	// delay introduced by presseing snooze
								// this will be either 0 or 
								// ALARM_SNOOZE_DELAY_MINS
								
 /***********************************************************
 *  @fn  	ALARM_GetDisplayString
 *  @brief 	Populates the character array with the alarm 
 *			time. Converts a given Time unit in Integer to 
 * 			Char Array
 *		 	   LCD (16 chars width): [0123456789012345]
 *		 format of the time string : 'HH:MM   |SNZ|OFF'.
 *
 *  @param 	char* strFormattedTime, a pointer to a char 
 *			array that will be populated with a formatted 
 * 			time string once the function returns
 *  @return void
 ***********************************************************/ 
void ALARM_GetDisplayString(char* strFormattedTime){
		
	// 	   LCD (16 chars width):   [0123456789012345]
	// format of the time string : 'HH:MM:SS    |OFF'
	
	// format hours	
	strFormattedTime[0]=(char)((intAlarmHours/10)+48);	
	strFormattedTime[1]=(char)((intAlarmHours%10)+48);
	
	// colon seperator
	strFormattedTime[2]=':';
	
	// format mins
	strFormattedTime[3]=(char)((intAlarmMins/10)+48);	
	strFormattedTime[4]=(char)((intAlarmMins%10)+48);
	
	// colon seperator
	strFormattedTime[5]=' ';		
		
	strFormattedTime[6]=' ';
	strFormattedTime[7]=' ';
	
	// show snooze status "|SNZ" <- means snooze has been pressed
	if(intSnoozePressed==1){
		strFormattedTime[8]='|';
		strFormattedTime[9]='S';
		strFormattedTime[10]='N';
		strFormattedTime[11]='Z';
	}
	else{
		strFormattedTime[8]=' ';
		strFormattedTime[9]=' ';
		strFormattedTime[10]=' ';
		strFormattedTime[11]=' ';
	}		
		
	// empty space and alarm seperator	
	strFormattedTime[12]='|';
	
	// set alarm status
	if(ALARM_GetAlarmStatus()==DCLOCK_ALARM_SET){
		strFormattedTime[13]='O';
		strFormattedTime[14]='N';
		strFormattedTime[15]=' ';
	}
	else{
		strFormattedTime[13]='O';
		strFormattedTime[14]='F';
		strFormattedTime[15]='F';
	}	
	
	strFormattedTime[16]='\0';
}

//----------------------------------------------------------------------
// INCREMENT/DECREMENT ALARM FUNCTIONS
// Secs: 	0 to 59
// Mins: 	0 to 59	
// Hours: 	0 to 24
//----------------------------------------------------------------------

 /**********************************************************
 *  @fn  	ALARM_IncrementSecs
 *  @brief 	used when user, sets the initial alarm seconds 
 * 			value calculate minutes, hours accordngly
 *  @param  None
 *  @return void
 **********************************************************/
void ALARM_IncrementSecs(){
	
	intAlarmSec++;	
	if(intAlarmSec>=60){		
		intAlarmSec = 0;
		intAlarmMins++;
	}
	if(intAlarmMins>=60){
		intAlarmMins = 0;
		intAlarmHours++;
	}
	if(intAlarmHours>=24){
		intAlarmHours=0;
	}
}

/**********************************************************
 *  @fn  	ALARM_IncrementMins
 *  @brief 	used when user, sets the initial alarm minutes 
 * 			value calculate hours accordngly.
 *  @param  None
 *  @return void
 **********************************************************/
void ALARM_IncrementMins(){
	
	intAlarmMins++;	
	if(intAlarmMins>=60){
		intAlarmMins = 0;
		intAlarmHours++;
	}
	if(intAlarmHours>=24){
		intAlarmHours=0;
	}
}

/**********************************************************
 *  @fn  	ALARM_IncrementHours
 *  @brief 	used when user, sets the initial alarm hours 
 * 			value , set upper bound to 24.
 *  @param  None
 *  @return void
 **********************************************************/
void ALARM_IncrementHours(){
	
	intAlarmHours++;	
	if(intAlarmHours>=24){
		intAlarmHours=0;
	}
}

 /**********************************************************
 *  @fn  	ALARM_DecrementSecs
 *  @brief 	used when user, sets the initial alarm seconds 
 * 			value calculate mins, hours accordingly
 *  @param  None
 *  @return void
 **********************************************************/
void ALARM_DecrementSecs(){
	
	// decrement seconds
	intAlarmSec--;	
	if(intAlarmSec<=0){		
		intAlarmSec = 60;
		intAlarmMins--;
	}
	if(intAlarmMins<=0){
		intAlarmMins = 60;
		intAlarmHours--;
	}
	if(intAlarmHours<=0){
		intAlarmHours=0;
	}
}

 /**********************************************************
 *  @fn  	ALARM_DecrementMins
 *  @brief 	used when user, sets the initial alarm minutes 
 * 			value calculate hours accordingly.
 *  @param  None
 *  @return void
 **********************************************************/
void ALARM_DecrementMins(){
	
	intAlarmMins--;		
	if(intAlarmMins<=0){
		intAlarmMins = 60;
		intAlarmHours--;
	}
	if(intAlarmHours<=0){
		intAlarmHours=0;
	}
}

 /**********************************************************
 *  @fn  	ALARM_DecrementHours
 *  @brief 	used when user, sets the initial alarm minutes 
 * 			set lower limit =0.
 *  @param  None
 *  @return void
 **********************************************************/
void ALARM_DecrementHours(){
	
	intAlarmHours--;			
	if(intAlarmHours<=0){
		intAlarmHours=0;
	}
}


//----------------------------------------------------------------------
// ALARM GETTER FUNCTIONS
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	ALARM_GetSecs
 *  @brief 	get alarm seconds value.
 *  @param  None
 *  @return int intAlarmSec
 **********************************************************/
int ALARM_GetSecs(){
	return intAlarmSec;
}

/**********************************************************
 *  @fn  	ALARM_GetMins
 *  @brief 	get alarm mins value.
 *  @param  None
 *  @return int intAlarmMins
 **********************************************************/
int ALARM_GetMins(){
	return intAlarmMins;
}

/**********************************************************
 *  @fn  	ALARM_GetHours
 *  @brief 	get alarm hours value.
 *  @param  None
 *  @return int intAlarmHours
 **********************************************************/
int ALARM_GetHours(){
	return intAlarmHours;
}


//----------------------------------------------------------------------
// ALARM SETTER FUNCTIONS
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	ALARM_SetSecs
 *  @brief  set alarm seconds value.
 *  @param  int intNewSec (new seconds value)
 *  @return void
 **********************************************************/
void ALARM_SetSecs(int intNewSec){
	intAlarmSec = intNewSec;
}

/**********************************************************
 *  @fn  	ALARM_SetMins
 *  @brief  set alarm mins value.
 *  @param  int intNewMins (new mins value)
 *  @return void
 **********************************************************/
void ALARM_SetMins(int intNewMins){
	intAlarmMins = intNewMins;
}

/**********************************************************
 *  @fn  	ALARM_SetHours
 *  @brief  set alarm hours value.
 *  @param  int intNewHours (new hours value)
 *  @return void
 **********************************************************/
void ALARM_SetHours(int intNewHours){
	intAlarmHours = intNewHours;
}


//----------------------------------------------------------------------
// SET/UNSET ALARM
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	ALARM_SetAlarm
 *  @brief  check the dip switch value and set/unset alarm 
 * 			accordingly.
 *  @param  Xuint32 intDipSwVal (dip switch value)
 *  @param  XGpio GpioOutputLEDAdr (led address)
 *  @return void
 **********************************************************/
void ALARM_SetAlarm(Xuint32 intDipSwVal){

	if(ISBITSET(intDipSwVal,6)){
		// set alarm
		intAlarmSet = DCLOCK_ALARM_SET;		
	}
	else{
		// not set alarm
		intAlarmSet = DCLOCK_ALARM_NSET;			
	}
}

/**********************************************************
 *  @fn  	ALARM_GetAlarmStatus
 *  @brief  return alarm status.
 *  @param  None
 *  @return int intAlarmSet
 **********************************************************/
int ALARM_GetAlarmStatus(){
	return intAlarmSet;
}


//----------------------------------------------------------------------
// ALARM NOTIFICATION SWITCH OFF/ON
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	ALARM_SwitchONOFFAlarmNotification
 *  @brief  Switch Alarm Notification (buzzer+leds) on/off,
 *			the snooze delay is taken into account, when 
 *			matching the alarm time to the current clock
 *			time.
 *  @param  XGpio GpioOutputLEDAdr (LED GPIO base address)
 *	@param	Xuint32 buzzer (Buzzer Base address)
 *  @return int alarm notification set/not set (1 or 0)
 **********************************************************/
int ALARM_SwitchONOFFAlarmNotification(XGpio GpioOutputLEDAdr, Xuint32 buzzer){
		
	// check current hours/mins against alarm hours/mins and 
	// set the alarm notification accordingly
	if(((intAlarmMins+intSnoozeDelay)==CLOCK_GetMins()) &&
	   (intAlarmHours==CLOCK_GetHours())){
		
	   // check if alarm nofitification is set
	   if(intAlarmSet==DCLOCK_ALARM_SET){
	   
			// switch on alarm notification
			ALARM_SwitchOnAlarmNotification(GpioOutputLEDAdr, buzzer);
			intSnoozePressed=0;
			//intSnoozeDelay=0;			
			return 1;	// return true - yes set alarm;
	   }
	   else{
			// switch off alarm notification
			ALARM_SwitchOffAlarmNotification(GpioOutputLEDAdr, buzzer);
			return 0;
	   }	
	}
	else{		
			// switch off alarm notification
			ALARM_SwitchOffAlarmNotification(GpioOutputLEDAdr, buzzer);
			return 0;
	}	
}


/**********************************************************
 *  @fn  	ALARM_SwitchOffAlarmNotification
 *  @brief 	switch off alarm notification led + buzzer
 *  @param  XGpio GpioOutputLEDAdr (led address)
 *	@param  Xuint32 buzzer (buzzer base address)
 *  @return void
 **********************************************************/
void ALARM_SwitchOffAlarmNotification(XGpio GpioOutputLEDAdr, Xuint32 buzzer){	
	Xuint32 intLedStatus=0;
	
	// get alarm led value
	intLedStatus = LED_GetAlarmOFF();
	
	// write out to gpio-leds
	XGpio_DiscreteWrite(&GpioOutputLEDAdr, 1, intLedStatus);
	
	// stop the buzzer
	//PBUZZER_mWriteSlaveReg1(buzzer,0, BUZZER_STOP);		
}

/**********************************************************
 *  @fn  	ALARM_SwitchOnAlarmNotification
 *  @brief 	switch on alarm notification led + buzzer
 *  @param  XGpio GpioOutputLEDAdr (led address)
 *	@param  Xuint32 buzzer (buzzer base address)
 *  @return void
 **********************************************************/
void ALARM_SwitchOnAlarmNotification(XGpio GpioOutputLEDAdr, Xuint32 buzzer){
	Xuint32 intLedStatus=0;
		
	// get alarm led value
	intLedStatus = LED_GetAlarmON();
	
	// write out to gpio-leds
	XGpio_DiscreteWrite(&GpioOutputLEDAdr, 1, intLedStatus);
	
	// start the buzzer
	//PBUZZER_mWriteSlaveReg1(buzzer,0, BUZZER_START);		
}


//----------------------------------------------------------------------
// ALARM SNOOZE RELATED - Set/Get snooze pressed
//----------------------------------------------------------------------
/**********************************************************
 *  @fn  	ALARM_SetSnooze
 *  @brief 	Snooze can only be set if the alarm is set and
 *			matched (when the alarm notification is ringing).
 *  @param  None
 *  @return void
 **********************************************************/
void ALARM_SetSnooze(){
	// check current hours/mins against alarm hours/mins and 
	// set the alarm notification accordingly
	if((intAlarmMins==CLOCK_GetMins()) &&
	   (intAlarmHours==CLOCK_GetHours())){
		
	   // check if alarm nofitification is set
	   if(intAlarmSet==DCLOCK_ALARM_SET){
			// set snooze to true
			intSnoozePressed = 1;
			intSnoozeDelay = intSnoozeDelay+ALARM_SNOOZE_DELAY_MINS;				
	   }
	   else{
			intSnoozePressed = 0;
			intSnoozeDelay = 0;
	   }	
	}
	else{
		intSnoozePressed = 0;
		intSnoozeDelay = 0;
	}	
}


/**********************************************************
 *  @fn  	ALARM_GetSnooze
 *  @brief 	return snooze status.
 *  @param  None
 *  @return intSnoozePressed (integer)
 **********************************************************/
int ALARM_GetSnooze(){
	return intSnoozePressed;
}




//----------------------------------------------------------------------
// ALARM INTERNAL RESET
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	ALARM_Initialise
 *  @brief  initialises all global variables.
 *  @param  None
 *  @return void
 **********************************************************/
void ALARM_Initialise(){
	
	// initialise all global vars 
	
	intAlarmSec=0;
	intAlarmMins=0;
	intAlarmHours=12;	// initially set to noon
	intAlarmSet=0; 		// 0=not set, 1=set
	intSnoozePressed=0;	// snooze pressed=1, not pressed=0
	intSnoozeDelay=0;	
}



//----------------------------------------------------------------------
// HELPER FUNCTIONS
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	__Get12Hour
 *  @brief 	convert 24hr hour integer to 12hr hour int
 *  @param  int intAlarmHours
 *  @return int int12Hr (12 hr format)
 **********************************************************/
int __Get12Hour(int intAlarmHours){
	
	int int12Hr = 0;
	
	if(intAlarmHours <=12 && intAlarmHours>0){
		int12Hr = intAlarmHours;
	}
	else if(intAlarmHours>=13 && intAlarmHours<=24){
		int12Hr = (intAlarmHours-12);
	}
	return int12Hr;
}







