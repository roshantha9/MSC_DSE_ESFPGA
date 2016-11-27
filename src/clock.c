/************************************************************************
 * @file	clock.c
 * @author  106033467
 * @version 1.0
 *
 * @brief This file implements the Clock functionality of the system
 *
 *  Functionality:
 *  	- Set clock time : seconds, hours, minutes
 * 		- set/show alarm on/off
 *		- clock 24hr/12 format conversion
 ************************************************************************/

//----------------------------------------------------------------------
// USER HEADER INCLUDES
//----------------------------------------------------------------------
#include "clock.h"
#include "alarm.h"


//----------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------
static int intSec=0;
static int intMins=0;
static int intHours=0;
static int intClockFormat=0;	// 0-24hr, 1-12hr

 /***********************************************************
 *  @fn  	CLOCK_GetDisplayString
 *  @brief 	Populates the character array with the clock 
 *			time. Converts a given Time unit in Integer to 
 * 			Char Array
 *			   LCD (16 chars width):   [0123456789012345]
 * 		   format of the time string : 'HH:MM:SS AM |OFF'.
 *
 *  @param 	char* strFormattedTime, a pointer to a char 
 *			array that will be populated with a formatted 
 * 			time string once the function returns
 *  @return void
 ***********************************************************/ 
void CLOCK_GetDisplayString(char* strFormattedTime){
	
	char strTimeFormat[8] = "AM";
	
	// format hours
	if(CLOCK_GetTimeFormat()==DCLOCK_FORMAT_24HR){
		strFormattedTime[0]=(char)((intHours/10)+48);	
		strFormattedTime[1]=(char)((intHours%10)+48);
	}
	else{		
		// convert 24hr integer to 12 hour format before 
		// constructing the string
		strFormattedTime[0]=(char)((__Get12Hour(intHours)/10)+48);	
		strFormattedTime[1]=(char)((__Get12Hour(intHours)%10)+48);
	}	
	
	// colon seperator
	strFormattedTime[2]=':';
	
	// format mins
	strFormattedTime[3]=(char)((intMins/10)+48);	
	strFormattedTime[4]=(char)((intMins%10)+48);
	
	// colon seperator
	strFormattedTime[5]=':';
	
	// format secs
	strFormattedTime[6]=(char)((intSec/10)+48);	
	strFormattedTime[7]=(char)((intSec%10)+48);
	
	strFormattedTime[8]=' ';
	
	// set am or pm based on 24hr/12hr clock
	if(CLOCK_GetTimeFormat()==DCLOCK_FORMAT_24HR){		
		
		// -- 24hr format --		
		strFormattedTime[9]=' ';
		strFormattedTime[10]=' ';
		strFormattedTime[11]=' ';
		strFormattedTime[12]=' ';
	}
	else{	
		// -- 12 hr format --
		// check if am or pm based on hour
		if(intHours>12){
			strTimeFormat[0] = 'P';
			strTimeFormat[1] = 'M';
		}
		else{
			strTimeFormat[0] = 'A';
			strTimeFormat[1] = 'M';
		}				
		strFormattedTime[9]=strTimeFormat[0];
		strFormattedTime[10]=strTimeFormat[1];	
	}
	
	// empty space and alarm seperator
	strFormattedTime[11]=' ';
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
// INCREMENT/DECREMENT TIME FUNCTIONS
// Secs: 	0 to 59
// Mins: 	0 to 59	
// Hours: 	0 to 24
//----------------------------------------------------------------------

 /**********************************************************
 *  @fn  	CLOCK_IncrementSecs
 *  @brief 	used when user, sets the clock seconds 
 * 			value calculate minutes, hours accordingly.
 *  @param  None
 *  @return void
 **********************************************************/
void CLOCK_IncrementSecs(){
	
	intSec++;	
	if(intSec>=60){		
		intSec = 0;
		intMins++;
	}
	if(intMins>=60){
		intMins = 0;
		intHours++;
	}
	if(intHours>=24){
		intHours=0;
	}
}

/**********************************************************
 *  @fn  	CLOCK_IncrementMins
 *  @brief 	used when user, sets the clock minutes 
 * 			value calculate hours accordngly.
 *  @param  None
 *  @return void
 **********************************************************/
void CLOCK_IncrementMins(){
	
	intMins++;	
	if(intMins>=60){
		intMins = 0;
		intHours++;
	}
	if(intHours>=24){
		intHours=0;
	}
}

/**********************************************************
 *  @fn  	CLOCK_IncrementHours
 *  @brief 	used when user, sets the clock hours 
 * 			setu uppset limit to 24
 *  @param  None
 *  @return void
 **********************************************************/
void CLOCK_IncrementHours(){
	
	intHours++;	
	if(intHours>=24){
		intHours=0;
	}
}

 /**********************************************************
 *  @fn  	CLOCK_DecrementSecs
 *  @brief 	used when user, sets the initial clock seconds 
 * 			value calculate mins, hours accordngly
 *  @param  None
 *  @return void
 **********************************************************/
void CLOCK_DecrementSecs(){
	
	intSec--;	
	if(intSec<=0){		
		intSec = 60;
		intMins--;
	}
	if(intMins<=0){
		intMins = 60;
		intHours--;
	}
	if(intHours<=0){
		intHours=0;
	}
}

 /**********************************************************
 *  @fn  	CLOCK_DecrementMins
 *  @brief 	used when user, sets the initial clock mins 
 * 			value calculate  hours accordngly
 *  @param  None
 *  @return void
 **********************************************************/
void CLOCK_DecrementMins(){
	
	intMins--;		
	if(intMins<=0){
		intMins = 60;
		intHours--;
	}
	if(intHours<=0){
		intHours=0;
	}
}

/**********************************************************
 *  @fn  	CLOCK_DecrementHours
 *  @brief 	used when user, sets the initial clock mins 
 * 			lower limit =0
 *  @param  None
 *  @return void
 **********************************************************/
void CLOCK_DecrementHours(){
	
	intHours--;			
	if(intHours<=0){
		intHours=0;
	}
}


//----------------------------------------------------------------------
// TIME - GETTER FUNCTIONS
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	CLOCK_GetSecs
 *  @brief 	get clock seconds value
 *  @param  None
 *  @return int intSec
 **********************************************************/
int CLOCK_GetSecs(){
	return intSec;
}

/**********************************************************
 *  @fn  	CLOCK_GetMins
 *  @brief 	get clock mins value
 *  @param  None
 *  @return int intMins
 **********************************************************/
int CLOCK_GetMins(){
	return intMins;
}

/**********************************************************
 *  @fn  	CLOCK_GetHours
 *  @brief 	get clock hours value
 *  @param  None
 *  @return int intHours
 **********************************************************/
int CLOCK_GetHours(){
	return intHours;
}


//----------------------------------------------------------------------
// TIME - SETTER FUNCTIONS
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	CLOCK_SetSecs
 *  @brief  set clock seconds value
 *  @param  int intNewSec (new seconds value)
 *  @return void
 **********************************************************/
void CLOCK_SetSecs(int intNewSec){
	intSec = intNewSec;
}

/**********************************************************
 *  @fn  	CLOCK_SetMins
 *  @brief  set clock mins value
 *  @param  int intNewMins (new mins value)
 *  @return void
 **********************************************************/
void CLOCK_SetMins(int intNewMins){
	intMins = intNewMins;
}

/**********************************************************
 *  @fn  	CLOCK_SetHours
 *  @brief  set clock hours value
 *  @param  int intNewHours (new hours value)
 *  @return void
 **********************************************************/
void CLOCK_SetHours(int intNewHours){
	intHours = intNewHours;
}

//----------------------------------------------------------------------
// CLOCK DISPLAY FORMAT FUNCTIONS - 24hr/12hr, set/unset
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	CLOCK_GetTimeFormat
 *  @brief  return clock format (24hr/12hr)
 *  @param  None
 *  @return int intClockFormat
 **********************************************************/
int CLOCK_GetTimeFormat(){
	return intClockFormat;
}

/**********************************************************
 *  @fn  	CLOCK_SetTimeFormat
 *  @brief  set clock format , based on dip switch value
 * 			if dip switch is on then, set to 12hr format, 
 * 			else 24hr
 *  @param  Xuint32 intDipSwVal (dip[ switch value)
 *  @return void
 **********************************************************/
void CLOCK_SetTimeFormat(Xuint32 intDipSwVal){
	
	if(ISBITSET(intDipSwVal,7)){
		// 12hr
		intClockFormat = DCLOCK_FORMAT_12HR;		
	}
	else{
		// 24hr
		intClockFormat = DCLOCK_FORMAT_24HR;		
	}
}


//----------------------------------------------------------------------
// CLOCK INTERNAL RESET
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	CLOCK_Initialise
 *  @brief  initialises all global variables
 *  @param  None
 *  @return void
 **********************************************************/
void CLOCK_Initialise(){
	
	// initialise all global vars 	
	intSec=0;
	intMins=0;
	intHours=0;
	intClockFormat=0;	// 0-24hr, 1-12hr	
}



//----------------------------------------------------------------------
// HELPER FUNCTIONS
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	__Get12Hour
 *  @brief  convert 24hr hour integer to 12hr hour int
 *  @param  int intHours (24hours time, hours)
 *  @return int int12Hr (12 hour format)
 **********************************************************/
int __Get12Hour(int intHours){
	
	int int12Hr = 0;
	
	if(intHours==0){
		int12Hr = 12;
	}	
	else if(intHours<=12 && intHours>0){
		int12Hr = intHours;
	}
	else if(intHours>=13 && intHours<=24){
		int12Hr = (intHours-12);
	}
	
	return int12Hr;
}







