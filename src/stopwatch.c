/************************************************************************
 * @file	stopwatch.c
 * @author  106033467
 * @version 1.0
 *
 * @brief This file contains the stopwatch related functions required by 
 *		  the multifunction digital clock.
 * 
 * Functionality:
 *	- Start/Stop Stopwatch
 *  - Clear Stopwatch
 *  - Increment time, miliseconds, increment hours/mins/seconds accordingly
 *  - Manage the Miliseconds timer.
 *
 ************************************************************************/
// user defined headers
#include "stopwatch.h"
#include "led.h"

//----------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------
static int intSWMs=0;
static int intSWSec=0;
static int intSWMins=0;
static int intSWHours=0;	
static int intSWActive=0;   // 0=inactive, 1=active
 
 /***********************************************************
 *  @fn  	STOPWATCH_GetDisplayString
 *  @brief 	Populates the character array with the stopwatch 
 *			running time. Converts a given Time unit in 
 *			Integer to Char Array
 *		 	   LCD (16 chars width): [0123456789012345]
 *		 format of the time string : 'HH:MM:SS:MS     '
 *
 *  @param 	char* strFormattedTime, a pointer to a char 
 *			array that will be populated with a formatted 
 * 			time string once the function returns
 *  @return void
 ***********************************************************/
 void STOPWATCH_GetDisplayString(char* strFormattedTime){
				
	// format hours - convert int to char	
	strFormattedTime[0]=(char)((intSWHours/10)+48);	
	strFormattedTime[1]=(char)((intSWHours%10)+48);
	
	// colon seperator
	strFormattedTime[2]=':';
	
	// format mins - convert int to char
	strFormattedTime[3]=(char)((intSWMins/10)+48);	
	strFormattedTime[4]=(char)((intSWMins%10)+48);
	
	// colon seperator
	strFormattedTime[5]=':';
	
	// format secs - convert int to char
	strFormattedTime[6]=(char)((intSWSec/10)+48);	
	strFormattedTime[7]=(char)((intSWSec%10)+48);
	
	//colon seperator
	strFormattedTime[8]=':';
	
	// format miliseconds - convert int to char
	strFormattedTime[9]=(char)((intSWMs/10)+48);	
	strFormattedTime[10]=(char)((intSWMs%10)+48);
				
	// empty spaces	
	strFormattedTime[11]=' ';
	strFormattedTime[12]=' ';
	strFormattedTime[13]=' ';
	strFormattedTime[14]=' ';
	strFormattedTime[15]=' ';
	strFormattedTime[16]='\0'; // null terminator
}

//----------------------------------------------------------------------
// INCREMENT/DECREMENT STOPWATCH FUNCTIONS
// Milisecs: 	0 to 99
// Secs: 		0 to 59
// Mins: 		0 to 59	
// Hours: 		0 to 99
//----------------------------------------------------------------------
 
 /**********************************************************
 *  @fn  	STOPWATCH_IncrementMs
 *  @brief 	increment the stop watch miliseconds integer. 
 * 			calculate seconds, minutes, hours accordngly
 *  @param  None
 *  @return void
 **********************************************************/
void STOPWATCH_IncrementMs(){
	
	// check if stopwatch is active before incrementing time
	if(intSWActive==1){
		intSWMs++;	
		if(intSWMs>=100){		
			intSWMs = 0;
			intSWSec++;
		}
		if(intSWSec>=60){
			intSWSec = 0;
			intSWMins++;
		}	
		if(intSWMins>=60){
			intSWMins = 0;
			intSWHours++;
		}
		if(intSWHours>=100){
			intSWHours=0;
		}
	}
}

//----------------------------------------------------------------------
// STOPWATCH, GETTER FUNCTIONS
//----------------------------------------------------------------------

 /**********************************************************
 *  @fn  	STOPWATCH_GetMs
 *  @brief 	Get miliseconds value
 *  @param  None
 *  @return int intSWMs (miliseconds variable)
 **********************************************************/
int STOPWATCH_GetMs(){
	return intSWMs;
}

 /**********************************************************
 *  @fn  	STOPWATCH_GetSecs
 *  @brief 	Get seconds value
 *  @param  None
 *  @return int intSWSec (seconds variable)
 **********************************************************/
int STOPWATCH_GetSecs(){
	return intSWSec;
}

 /**********************************************************
 *  @fn  	STOPWATCH_GetMins
 *  @brief 	Get mins value
 *  @param  None
 *  @return int intSWMins (minutes variable)
 **********************************************************/
int STOPWATCH_GetMins(){
	return intSWMins;
}

 /**********************************************************
 *  @fn  	STOPWATCH_GetHours
 *  @brief 	Get hours value
 *  @param  None
 *  @return int intSWHours (hours variable)
 ***********************************************************/
int STOPWATCH_GetHours(){
	return intSWHours;
}

 /**********************************************************
 *  @fn  	STOPWATCH_GetStatus
 *  @brief 	Get the stopwatch status
 *  @param  None
 *  @return int intSWActive (sw active variable, boolean)
 **********************************************************/
int STOPWATCH_GetStatus(){
	return intSWActive;
}


//----------------------------------------------------------------------
// STOPWATCH, SETTER FUNCTIONS
//----------------------------------------------------------------------

 /**********************************************************
 *  @fn  	STOPWATCH_SetMs
 *  @brief 	set the stopwatch ms value
 *  @param  int intNewMs (new miliseconds value)
 *  @return void
 **********************************************************/
void STOPWATCH_SetMs(int intNewMs){
	intSWMs = intNewMs;
}

 /**********************************************************
 *  @fn  	STOPWATCH_SetSecs
 *  @brief 	set the stopwatch seconds value
 *  @param  int intNewSec (new seconds value)
 *  @return void
 ***********************************************************/
void STOPWATCH_SetSecs(int intNewSec){
	intSWSec = intNewSec;
}

 /**********************************************************
 *  @fn  	STOPWATCH_SetMins
 *  @brief 	set the stopwatch mins value
 *  @param  int intNewSec (new seconds value)
 *  @return void
 ***********************************************************/
void STOPWATCH_SetMins(int intNewMins){
	intSWMins = intNewMins;
}

 /**********************************************************
 *  @fn  	STOPWATCH_SetHours
 *  @brief 	set the stopwatch hours value
 *  @param  int intNewHours (new hours value)
 *  @return void
 ***********************************************************/
void STOPWATCH_SetHours(int intNewHours){
	intSWHours = intNewHours;
}


//----------------------------------------------------------------------
// STOPWATCH, STOP/START/CLEAR FUNCTIONS
//----------------------------------------------------------------------

 /**********************************************************
 *  @fn  	STOPWATCH_Start
 *  @brief 	start the stopwatch (ms_timer)
 *  @param  Xuint32 ms_timer_addr - milisecond timer address
 *  @return void
 ***********************************************************/
void STOPWATCH_Start(Xuint32 ms_timer_addr){
	
	// start the ms_timer
	MILISEC_TIMER_mWriteSlaveReg1(ms_timer_addr,0, MS_TIMER_START);
	
	// update internal status
	intSWActive = 1;	
}

/**********************************************************
 *  @fn  	STOPWATCH_Stop
 *  @brief 	stop the stopwatch (ms_timer)
 *  @param  Xuint32 ms_timer_addr - milisecond timer address
 *  @return void
 **********************************************************/
void STOPWATCH_Stop(Xuint32 ms_timer_addr){
	
	// stop ms_timer
	MILISEC_TIMER_mWriteSlaveReg1(ms_timer_addr,0, MS_TIMER_STOP);
	
	// update internal status
	intSWActive = 0;
	
}


/**********************************************************
 *  @fn  	STOPWATCH_StartStop
 *  @brief 	start/stop the stopwatch toggling id performed
 *  @param  Xuint32 ms_timer_addr - milisecond timer address
 *  @return void
 **********************************************************/
void STOPWATCH_StartStop(Xuint32 ms_timer_addr){
	
	// if stopwatch is active then stop it
	if(intSWActive==1){
		MILISEC_TIMER_mWriteSlaveReg1(ms_timer_addr,0, MS_TIMER_STOP);
		intSWActive = 0;		
	}
	else 
	// if stopwatch	is inactive then start it
	if(intSWActive==0){
		MILISEC_TIMER_mWriteSlaveReg1(ms_timer_addr,0, MS_TIMER_START);
		intSWActive = 1;		
	}
	// unknown sw status, just stop it, and report
	else{
		MILISEC_TIMER_mWriteSlaveReg1(ms_timer_addr,0, MS_TIMER_STOP);
		intSWActive = 0;
		xil_printf("**WARNING[STOPWATCH_StartStop, intSWActive]: Unknown intSWActive=%d \r\n",intSWActive);
	}	
}


/**********************************************************
 *  @fn  	STOPWATCH_Clear
 *  @brief 	clear the stopwatch (hr,min,sec,ms) values,
 *			and also stop the timer.
 *  @param  Xuint32 ms_timer_addr - milisecond timer address
 *  @return void
 **********************************************************/
void STOPWATCH_Clear(Xuint32 ms_timer_addr){

	// also stop the stop watch - safety
	MILISEC_TIMER_mWriteSlaveReg1(ms_timer_addr,0, MS_TIMER_STOP);
	
	// initialise all global vars 
	intSWMs=0;
	intSWSec=0;
	intSWMins=0;
	intSWHours=0;	
	intSWActive=0;		
}


//----------------------------------------------------------------------
// STOPWATCH INTERNAL RESET
//----------------------------------------------------------------------

/**********************************************************
 *  @fn  	STOPWATCH_Initialise
 *  @brief  initialises all global variables
 *  @param  None
 *  @return void
 **********************************************************/
void STOPWATCH_Initialise(){
	
	// initialise all global vars 	
	intSWMs=0;
	intSWSec=0;
	intSWMins=0;
	intSWHours=0;	
	intSWActive=0;   // 0=inactive, 1=active	
}



