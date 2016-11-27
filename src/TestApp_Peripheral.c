/************************************************************************
 * @file	TestApp_Peripheral.c
 * @author  106033467
 * @version 1.0
 *
 * @brief Multifunction Digital Clock (main app file)
 *
 *  Functionality:
 *  	- Digital Clock (24hr/12 format, set alarm)
 * 		- Alarm (set alarm, on/off, snooze, notification via led+buzzer)
 * 		- Timer (set/enable/disable, notification via led+buzzer)
 * 		- Stopwatch (hours:mins:sec:ms, start/stop/clear)
 ************************************************************************/

//-----------------------------------------------------------------------
// INCLUDE FILES
//-----------------------------------------------------------------------
 
// Xilinx, Microblaze specific system headers
#include "xparameters.h"
#include "xbasic_types.h"
#include "xstatus.h"
#include "sec_timer.h"
#include "xintc.h"
#include "xgpio.h"
#include "milisec_timer.h"
#include "xwdttb.h"
#include "pbuzzer.h"
#include "sec_timer_gated.h"

// User defined function headers, used to connect
// different functionality
#include "lcd_helper.h"
#include "clock.h"
#include "alarm.h"
#include "menu.h"
#include "led.h"
#include "stopwatch.h"
#include "common.h"
#include "timer.h"

//-----------------------------------------------------------------------
// CONSTANT DEFINES/ENUMS
//-----------------------------------------------------------------------

/* Push Button Enumerator */
enum PBType {
	 PUSHBTN_N		= 1, // north
	 PUSHBTN_S 		= 2, // south 
	 PUSHBTN_E 		= 3, // east
	 PUSHBTN_W 		= 4, // west
	 PUSHBTN_C		= 5	 // center
 }enumPBType;


//-----------------------------------------------------------------------
// FUNCTION PROTOTYPES
//-----------------------------------------------------------------------
void MS_TIMER_Intr_Handler(void * baseaddr_p);
void SEC_TIMER_Intr_Handler(void * baseaddr_p);
void PB_West_Intr_Handler(void * baseaddr_p);
void PB_Center_Intr_Handler(void * baseaddr_p);
void PB_East_Intr_Handler(void * baseaddr_p);
void PB_North_Intr_Handler(void * baseaddr_p);
void PB_South_Intr_Handler(void * baseaddr_p);
void DCLOCK_DisplayScreen();
void DCLOCK_PBHandler(enum PBType enumPB);
void DCLOCK_PrintScreen(char* strTitle, char* strLine2);
void WD_TIMER_Intr_Handler(void * baseaddr_p);
int main(void);



//-----------------------------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------------------------

XGpio GpioInputDIPSW;		// 8-bit, Dip Switch (input) GPIO
XGpio GpioOutputPIEZO;		// Piezo (output) GPIO 
XGpio GpioOutputLEDs;		// 8-bit, LED (output) GPIO 


Xuint8 intSemPBLock=0;		// display semaphore

static XIntc myIntc;		// interrupt controller instance
static XWdtTb  wd_timer;	// watch dog timer instance

Xuint32 sec_timer;			// seconds timer address

// seconds timer address pointer
unsigned int *sec_timer_p =	
        (unsigned int *) XPAR_SEC_TIMER_0_BASEADDR;
	
Xuint32 ms_timer;			// seconds timer address

// seconds timer address pointer
unsigned int *ms_timer_p =
        (unsigned int *) XPAR_MILISEC_TIMER_0_BASEADDR;
		
Xuint32 buzzer;				// pbuzzer peripheral address

// pbuzzer peripheral address pointer
unsigned int *buzzer_p =
        (unsigned int *) XPAR_PBUZZER_0_BASEADDR;
		
Xuint32 sec_gated_timer;	// gated seconds timer address

// seconds timer address pointer
unsigned int *sec_gated_timer_p =
        (unsigned int *) XPAR_SEC_TIMER_GATED_0_BASEADDR;

//-----------------------------------------------------------------------
// INTERRUPT HANDLING FUNCTIONS 
//-----------------------------------------------------------------------

/***********************************************************
 *  @fn  	SEC_TIMER_Intr_Handler
 *  @brief 	Interrupt Handler for the Seconds timer - 
 * 			this fires every 1s.
 *			Functionality:
 *				- Checks dipswitch status.
 *				- Set clock time format accordingly
 *				- Increment Digital Clock - seconds
 * 				- Check and Set alarm notification
 * 				- Display/Update Clock screen
 *  @param 	baseaddr_p sec_timer base address pointer
 *  @return void
 ***********************************************************/
void SEC_TIMER_Intr_Handler(void * baseaddr_p){
	static int tmrcnt=0;	

	// increment clock seconds
	CLOCK_IncrementSecs();	
}

/***********************************************************
 *  @fn  	PB_West_Intr_Handler
 *  @brief 	Interupt Handler for PushButton West - level-high 
 *			triggered
 *			Functionality:
 *				- Notify Push Button Handler, on West Button 
 *				  press
 *  @param 	baseaddr_p pushbutton_west base address pointer
 *  @return void
 ***********************************************************/
void PB_West_Intr_Handler(void * baseaddr_p){
	static int pbWcnt=0;
	Xuint32 baseaddr;
	
	// avoid pushbuttons to be activates at same time
	if(intSemPBLock == 0){
		intSemPBLock = 1;	// lock the keys
		
		// DEBUG output
		xil_printf( " PushButton W = %06d\r\n", pbWcnt++ );	
		
		// pass keypress to handler
		DCLOCK_PBHandler(PUSHBTN_W);		
		
		intSemPBLock = 0;	// unlock the keys
	}	
}

/***********************************************************
 *  @fn  	PB_Center_Intr_Handler
 *  @brief 	Interupt Handler for PushButton Center 
 *			- level-high triggered
 *			Functionality:
 *				- Notify Push Button Handler, on Center 
 *				  Button press
 *  @param 	baseaddr_p pushbutton_center base address pointer
 *  @return void
 ***********************************************************/
void PB_Center_Intr_Handler(void * baseaddr_p){
	static int pbCcnt=0;
	
	// avoid pushbuttons to be activates at same time
	if(intSemPBLock == 0){
		intSemPBLock = 1;	// lock the keys
	
		// DEBUG output
		xil_printf( " PushButton C = %06d\r\n", pbCcnt++ );   
		
		// pass keypress to handler
		DCLOCK_PBHandler(PUSHBTN_C);
		
		intSemPBLock = 0;	// unlock the keys
	}
}

/***********************************************************
 *  @fn  	PB_East_Intr_Handler
 *  @brief 	Interupt Handler for PushButton East 
 *			- level-high triggered
 *			Functionality:
 *				- Notify Push Button Handler, on East Button 
 * 				  press
 *  @param 	baseaddr_p pushbutton_east base address pointer
 *  @return void
 ***********************************************************/
void PB_East_Intr_Handler(void * baseaddr_p){
	static int pbEcnt=0;
	
	// avoid pushbuttons to be activates at same time
	if(intSemPBLock == 0){
		intSemPBLock = 1;	// lock the keys
	
		// DEBUG output
		xil_printf( " PushButton E = %06d\r\n", pbEcnt++ );

		// pass keypress to handler
		DCLOCK_PBHandler(PUSHBTN_E);	
		
		intSemPBLock = 0;	// unlock the keys
	}	
}

/***********************************************************
 *  @fn  	PB_North_Intr_Handler
 *  @brief 	Interupt Handler for PushButton North 
 * 			- edge-rising triggered
 *			Functionality:
 *				- Notify Push Button Handler, on North Button 
 *				  press
 *  @param 	baseaddr_p pushbutton_north base address pointer
 *  @return void
 ***********************************************************/
void PB_North_Intr_Handler(void * baseaddr_p){
	static int pbNcnt=0;
	//Xuint32 PiezoDataWrite;
	static Xuint32 leddata;
	
	// avoid pushbuttons to be activates at same time
	if(intSemPBLock == 0){
		intSemPBLock = 1;	// lock the keys	
		
		// DEBUG output
		xil_printf( " PushButton N = %06d\r\n", pbNcnt++ );
		
		// pass keypress to handler
		DCLOCK_PBHandler(PUSHBTN_N);		
		
		intSemPBLock = 0;	// unlock the keys
	}
}

/***********************************************************
 *  @fn  	PB_South_Intr_Handler
 *  @brief 	Interupt Handler for PushButton South 
 *			- edge-rising triggered
 *			Functionality:
 *				- Notify Push Button Handler, on South 
 *   			  Button press
 *  @param 	baseaddr_p pushbutton_south base address pointer
 *  @return void
 ***********************************************************/
void PB_South_Intr_Handler(void * baseaddr_p){
	static int pbScnt=0;	
	
	// avoid pushbuttons to be activates at same time
	if(intSemPBLock == 0){
	
		intSemPBLock = 1;	// lock the keys	
	
		// DEBUG output
		xil_printf( " PushButton S = %06d\r\n", pbScnt++ );  
		
		// pass keypress to handler
		DCLOCK_PBHandler(PUSHBTN_S);	
		
		intSemPBLock = 0;	// unlock the keys	
	}
}

/***********************************************************
 *  @fn  	MS_TIMER_Intr_Handler
 *  @brief 	Interupt Handler for Milisecond Timer Interrupt
 *			- edge-rising triggered
 *			Functionality:
 *				- Increment Miliseconds
 *				- Update Screen
 *  @param 	baseaddr_p ms_timer base address pointer
 *  @return void
 ***********************************************************/
void MS_TIMER_Intr_Handler(void * baseaddr_p){	
	static int pbMScnt=0;
	  
	// DEBUG output
	//xil_printf( " MsTimer = %06d\r\n", pbMScnt++ );
	
	// increment the Stop watch miliseconds
	STOPWATCH_IncrementMs();
	DCLOCK_DisplayScreen();	
}

/***********************************************************
 *  @fn  	WD_TIMER_Intr_Handler
 *  @brief 	Watchdog Timer, Interrupt Handler
 *			- edge-rising triggered
 *			Functionality:
 *				- Stop & Restart Watchdog timer
 *  @param 	baseaddr_p wd_timer base address pointer
 *  @return void
 ***********************************************************/
void WD_TIMER_Intr_Handler(void * baseaddr_p){
	static int pbWDtcnt=0;	
	
	XWdtTb_Stop(&wd_timer);
	XWdtTb_RestartWdt(&wd_timer);        
	
	// DEBUG output
	xil_printf( " WDTimer = %06d\r\n", pbWDtcnt++ );
}

/***********************************************************
 *  @fn  	GATED_SEC_TIMER_Intr_Handler
 *  @brief 	Gated Seconds Timer Interrupt Handler
 *			- edge-rising triggered
 *			Functionality:
 *				- Decrement Timer Seconds value(CountDown)
 *  @param 	baseaddr_p gated_sec_timer base address pointer
 *  @return void
 ***********************************************************/
void GATED_SEC_TIMER_Intr_Handler(void * baseaddr_p){
	static int pbTcnt=0;
		  
	// DEBUG output
	xil_printf( " GatedSecTimer = %06d\r\n", pbTcnt++ );
	
	// decrement seconds, and update display	
	TIMER_DecrementSecs();	
}



//-----------------------------------------------------------------------
// UTILITY FUNCTIONS 
//-----------------------------------------------------------------------

/***********************************************************
 *  @fn  	DCLOCK_PBHandler
 *  @brief 	The Pushbuttons will pass on the key handling 
 *			functions to this handler, which will check the 
 *			PushButton type and perform different functions 
 *			accordingly.
 *			- PB North : incr. menu index, show next scr
 *			- PB South : decr. menu index, show previous scr
 *			- PB West  : funct. varies depending on curr. scr
 *				-# Clock 	 - set curr time, inc. hours
 *				-# Alarm 	 - set alarm time, inc. hours
 *				-# StopWatch - start/stop stopwatch
 *				-# Timer 	 - set timer value, inc. hours
 *			- PB Center  : funct. varies depending on curr. scr
 *				-# Clock 	 - set curr time, inc. mins
 *				-# Alarm 	 - set alarm time, inc. mins
 *				-# StopWatch - clear stopwatch
 *				-# Timer 	 - set timer value, inc. mins
 * 			- PB East  : funct. varies depending on curr. scr
 *				-# Clock 	 - set curr time, inc. sec, snooze alarm
 *				-# Alarm 	 - snooze alarm
 *				-# StopWatch - no functionality
 *				-# Timer 	 - set timer value, inc. sec 			
 * 
 *  @param 	enumPB pushbutton enumeration
 *  @return void
 ***********************************************************/
void DCLOCK_PBHandler(enum PBType enumPB){
	
	int intMix=0;
	enum DeviceMenuItem MIx;
	
		
	// get current menu index, will use it 
	// to decide on functionality
	MIx = MENU_GetMenuIx();
	
	switch(enumPB){
	
		case PUSHBTN_N:{
			// ** scroll UP the menu ** 
			MENU_IncrementMenuIx();			
			lcd_clear();			
			
		}break;
		
		case PUSHBTN_S:{
			// ** scroll DOWN the menu ** 
			MENU_DecrementMenuIx();			
			lcd_clear();			
		
		}break;
		
		case PUSHBTN_W:{
			// Functions
			// ---------
			// Clock 	 - set curr time, inc. hours
			// Alarm 	 - set alarm time, inc. hours
			// StopWatch - start stopwatch
			// Timer 	 - set timer value, inc. hours
						
			switch(MIx){
				case DCLOCK_MENU_CLOCK:{
					// update time (inc. hours)
				    CLOCK_IncrementHours();
				}break;
				
				case DCLOCK_MENU_ALARM:{
					// update alarm (inc. hours)
				    ALARM_IncrementHours();
				}break;
				
				case DCLOCK_MENU_STOPWATCH:{
					//xil_printf( " ---- SW started \r\n" );	
					// start/stop the stopwatch
					STOPWATCH_StartStop(ms_timer);
				}break;
				
				case DCLOCK_MENU_TIMER:{
					// update timer hours value
					TIMER_IncrementHours();
				}break;
				
				default:
					xil_printf("WARNING[DCLOCK_PBHandler, PUSHBTN_W]: Unknown MIx=%d \r\n",MIx);
					break;
			}
			
			
		}break;
		
		case PUSHBTN_E:{
			// Functions
			// ---------
			// Clock 	 - set curr time, inc. sec
			// Alarm 	 - set alarm time, inc. sec
			// StopWatch - clear stopwatch
			// Timer 	 - set timer value, inc. sec 
			
			switch(MIx){
				case DCLOCK_MENU_CLOCK:{
					// update time (inc. sec)
					CLOCK_IncrementSecs();
					ALARM_SetSnooze();
					
				}break;
				
				case DCLOCK_MENU_ALARM:{
					// update alarm (inc. sec)
				    //ALARM_IncrementSecs();
					ALARM_SetSnooze();
					
					// no alarm function for East button
				}break;
				
				case DCLOCK_MENU_STOPWATCH:{						
					// no functionality - for now
				}break;
				
				case DCLOCK_MENU_TIMER:{
					// update timer seconds value
					TIMER_IncrementSecs();
				}break;
				
				default:
					xil_printf("****WARNING[DCLOCK_PBHandler, PUSHBTN_E]: Unknown MIx=%d \r\n",MIx);
					break;
			}
			
			
		}break;
		
		case PUSHBTN_C:{
			// Functions
			// ---------
			// Clock 	 - set curr time, inc. mins
			// Alarm 	 - set alarm time, inc. mins
			// StopWatch - stop stopwatch
			// Timer 	 - set timer value, inc. mins 
			
			switch(MIx){
				case DCLOCK_MENU_CLOCK:{
					// update time (inc. mins)
					CLOCK_IncrementMins();
				}break;
				
				case DCLOCK_MENU_ALARM:{
					// update alarm (inc. mins)
				    ALARM_IncrementMins();
				}break;
				
				case DCLOCK_MENU_STOPWATCH:{
					xil_printf( " ---- SW cleared \r\n" );				
					
					// clear/reset the stopwatch
					STOPWATCH_Clear(ms_timer);
					
				}break;
				
				case DCLOCK_MENU_TIMER:{
					// update timer mins value
					TIMER_IncrementMins();
				}break;
				
				default:
					xil_printf("****WARNING[DCLOCK_PBHandler, PUSHBTN_C]: Unknown MIx=%d \r\n",MIx);
					break;
			}
			
			
		}break;
		
		default:
			/* default - do nothing, throw warning */
			xil_printf("****WARNING[DCLOCK_PBHandler]: Unknown pb pressed");	
			break;		
	}
	
	// after handling key press update the display screen	
	DCLOCK_DisplayScreen();
}

/***********************************************************
 *  @fn  	DCLOCK_DisplayScreen
 *  @brief 	This function will be called to draw out the 
 *			screen based on the currently selected screen.
 *			The text that will be displayed on each screen 
 *			will be different. Text will be	comprised of 
 *			'title' on line 1, and respective time values on 
 *			line 2. This function will set the title(line1)
 *			and (line2), and call helper function to send 
 *			out text/command to lcd driver
 *			- Clock Screen
 *				HH:MM:SS    |<AM/PM> |<ALARM:ON/OFF>
 *			- Alarm Screen
 *				HH:MM    	|<SNOOZE> |<ALARM:ON/OFF>
 *			- Stopwatch Screen (counting up, refresh 1ms)
 *				HH:MM:SS:MS		
 *			- Timer Screen	   (counting down, referesh 1sec)
 *				HH:MM:SS
 *  @param 	None
 *  @return void
 ***********************************************************/
void DCLOCK_DisplayScreen(){
	
	static char strScreenBuffer[32];		// used to store screen text
	
	enum DeviceMenuItem MIx;
	
	// get current menu index
	MIx = MENU_GetMenuIx();
	
	switch(MIx){
	
		case DCLOCK_MENU_CLOCK:{			
			/** Show Clock Screen **/
			
			// get clock display string
			//CLOCK_GetDisplayString(strClockBuff);		
			CLOCK_GetDisplayString(strScreenBuffer);	
			
						
			// construct screen on lcd			
			DCLOCK_PrintScreen("*Digital Clock*", strScreenBuffer);
			
		}break;
			
		case DCLOCK_MENU_ALARM:{
			/* Alarm Screen */
			
			// get clock display string
			//ALARM_GetDisplayString(strAlarmBuff);
			ALARM_GetDisplayString(strScreenBuffer);
				
			// construct screen on lcd			
			DCLOCK_PrintScreen("*Alarm*", strScreenBuffer);
		
			}break;
			
		case DCLOCK_MENU_STOPWATCH:{
			/* Stopwatch Screen */
			//STOPWATCH_GetDisplayString(strStopWatchBuff);						
			STOPWATCH_GetDisplayString(strScreenBuffer);	
			
			// construct screen on lcd			
			DCLOCK_PrintScreen("*Stopwatch*", strScreenBuffer);
			
			}break;
			
		case DCLOCK_MENU_TIMER:{
			/* Timer Screen */
			TIMER_GetDisplayString(strScreenBuffer);	
			
			// construct screen on lcd			
			DCLOCK_PrintScreen("*Timer*", strScreenBuffer);			
			
			}break;
		
		default:{
			/* default - do nothing, throw warning */
			xil_printf("WARNING[DCLOCK_DisplayScreen]: Unknown menu item selected");			
			}break;
	}	
}

/***********************************************************
 *  @fn  	DCLOCK_PrintScreen
 *  @brief 	Writes out two lines on the LCD. Cursor movement, 
 *			printout is performed.
 *  @param 	char* strTitle	title (line1) of selected the screen 
 *  @param 	char* strLine2	line2 of selected the screen
 *  @return void
 ***********************************************************/
void DCLOCK_PrintScreen(char* strTitle, char* strLine2){
	
	// move cursor and print title and screen string
	lcd_move(0,0);
	lcd_print(strTitle);
	lcd_move(1,0);
	lcd_print(strLine2);	
}


//-----------------------------------------------------------------------
// Main Program
//-----------------------------------------------------------------------
int main (void){
	Xuint32 i;
	Xuint32 temp;
    Xuint32 DPSW_GPIOStatus;
	Xuint32 PIEZO_GPIOStatus;
	Xuint32 LED_GPIOStatus;
	Xuint32 MSTimer_Status;
	Xuint32 WDTimer_Status;
	static Xuint32 DipSwitchDataRead;
	
	// variables used only for debug
	static char strDEBUGClockBuff[32];		// used to store 'clock screen' text
	static char strDEBUGAlarmBuff[32];		// used to store 'alarm screen' text
	static char strDEBUGStopWatchBuff[32];	// used to store 'stopwatch screen' text
	static char strDEBUGTimerBuff[32];		// used to store 'timer screen' text 	
	
	// Clear the (TTY) screen
	xil_printf("%c[2J",27);
	
	// Check that the peripheral exists
	baseaddr = (Xuint32)XPAR_LCD_0_BASEADDR;
	XASSERT_NONVOID(((Xuint32 *)baseaddr) != XNULL);
	xil_printf("LCD Test\r\n");
	
	xil_printf("main::baseaddr=0x%x\r\n",baseaddr);
	
	//----------------------------------------------------
    // INITIALISE THE INT CONTROLLER
    //----------------------------------------------------	
	xil_printf("Setting up interrupt controller...\r\n");
    XIntc_Initialize(&myIntc, XPAR_INTC_0_DEVICE_ID);
	
	
	//----------------------------------------------------
    // INITIALISE GPIO - DipSwitches (8-bit)
    //----------------------------------------------------
	
	// Initialize the GPIO driver so that it's ready to use,
	DPSW_GPIOStatus = XGpio_Initialize(&GpioInputDIPSW, 
                         XPAR_DIP_SWITCHES_8BIT_DEVICE_ID);
						 
	if (DPSW_GPIOStatus != XST_SUCCESS){
		xil_printf("\r\nERROR: GPIO DipSwitches failed to Initialise");
		return XST_FAILURE;
	}			
	
	// Set the direction for all signals to be inputs
	XGpio_SetDataDirection(&GpioInputDIPSW, 1, 0xFFFFFFFF);	
	
	//----------------------------------------------------
    // INITIALISE BUZZER PERIPHERAL
    //----------------------------------------------------				
	xil_printf("  - Initializing the buzzer peripheral\r\n");					
	// Check that the buzzer peripheral exists
	XASSERT_NONVOID(buzzer_p != XNULL);
	buzzer = (Xuint32) buzzer_p;
	xil_printf("  - stopping buzzer\r\n");
	PBUZZER_mWriteSlaveReg1(buzzer,0, BUZZER_STOP);
	
	//----------------------------------------------------
    // INITIALISE LEDs - (8-bit)
    //----------------------------------------------------	
		
	xil_printf("  - Setting up the LEDs\r\n");
	
    // Initialize the GPIO driver
    LED_GPIOStatus = XGpio_Initialize(&GpioOutputLEDs,
							XPAR_LED_8BIT_DEVICE_ID);
	
	// check if status is successful
	if (LED_GPIOStatus != XST_SUCCESS){
		xil_printf("\r\nERROR: GPIO LEDs failed to Initialise");
		return XST_FAILURE;
	}
	
	// Set the direction for all signals to be outputs
	XGpio_SetDataDirection(&GpioOutputLEDs, 1, 0x0);
  
	// turn off the leds
	XGpio_DiscreteWrite(&GpioOutputLEDs, 1, 0x00); 
	
	//----------------------------------------------------
    // INITIALISE WATCHDOG TIMER
    //----------------------------------------------------	
	
	// Initialize the Timer
	WDTimer_Status = XWdtTb_Initialize(&wd_timer,XPAR_XPS_TIMEBASE_WDT_0_DEVICE_ID);
	if (WDTimer_Status != XST_SUCCESS){
		xil_printf("\r\nERROR: WD Timer failed to initialize\r\n");
		return XST_FAILURE;
	}
	
		
	//----------------------------------------------------
    // INITIALISE MILLISEC_TIMER PERIPHERAL
    //----------------------------------------------------				
	xil_printf("  - Initializing the ms_timer peripheral\r\n");					
	// Check that the timer peripheral exists
	XASSERT_NONVOID(ms_timer_p != XNULL);
	ms_timer = (Xuint32) ms_timer_p;
	
	
	//----------------------------------------------------
    // INITIALISE GATED_SEC_TIMER PERIPHERAL (for timer feature)
    //----------------------------------------------------				
	xil_printf("  - Initializing the gated_sec_timer peripheral\r\n");					
	// Check that the timer peripheral exists
	XASSERT_NONVOID(sec_gated_timer_p != XNULL);
	sec_gated_timer = (Xuint32) sec_gated_timer_p;
	
		
	//----------------------------------------------------
	// INITIALIZE THE SEC_TIMER PERIPHERAL
    //----------------------------------------------------	
	xil_printf("  - Initializing the sec_timer peripheral\r\n");
	// Check that the timer peripheral exists
	XASSERT_NONVOID(sec_timer_p != XNULL);
	sec_timer = (Xuint32) sec_timer_p;
	
	

	//----------------------------------------------------
    // REGISTER INTERRUPT HANDLER
    //----------------------------------------------------
	xil_printf("  - Registering interrupt handlers\r\n");
	
	// register sec_timer interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_SEC_TIMER_0_IP2INTC_IRPT_INTR,
						SEC_TIMER_Intr_Handler,
						(void *)XPAR_SEC_TIMER_0_BASEADDR);
						
	// register pushbutton_west interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_DEBOUNCER_0_DB_PB_WEST_INTR,
						PB_West_Intr_Handler,
						(void *) 0);
						
	// register pushbutton_east interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_DEBOUNCER_0_DB_PB_EAST_INTR,
						PB_East_Intr_Handler,
						(void *) 0);
	
	// register pushbutton_center interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_DEBOUNCER_0_DB_PB_CENTER_INTR,
						PB_Center_Intr_Handler,
						(void *) 0);
	
	// register pushbutton_south interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_DEBOUNCER_0_DB_PB_SOUTH_INTR,
						PB_South_Intr_Handler,
						(void *) 0);
						
	// register pushbutton_north interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_DEBOUNCER_0_DB_PB_NORTH_INTR,
						PB_North_Intr_Handler,
						(void *) 0);
	
	// register millisec_timer interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_MILISEC_TIMER_0_IP2INTC_IRPT_INTR,
						MS_TIMER_Intr_Handler,
						(void *)XPAR_MILISEC_TIMER_0_BASEADDR);
					
	// register wd_timer interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_XPS_TIMEBASE_WDT_0_WDT_INTERRUPT_INTR,
						WD_TIMER_Intr_Handler,
						(void *)XPAR_XPS_TIMEBASE_WDT_0_BASEADDR);
					
	// register gated_sec_timer interrupt handler
	XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
						XPAR_XPS_INTC_0_SEC_TIMER_GATED_0_IP2INTC_IRPT_INTR,
						GATED_SEC_TIMER_Intr_Handler,
						(void *)XPAR_SEC_TIMER_GATED_0_BASEADDR);			
	
	//----------------------------------------------------
    // ENABLE INTERRUPTS
    //----------------------------------------------------
    xil_printf("  - Enabling interrupts\r\n");
    // Enable INTC interrupts
    XIntc_mMasterEnable(XPAR_XPS_INTC_0_BASEADDR);
    XIntc_mEnableIntr( XPAR_XPS_INTC_0_BASEADDR,
  							XPAR_SEC_TIMER_0_IP2INTC_IRPT_MASK |
							XPAR_DEBOUNCER_0_DB_PB_WEST_MASK |
							XPAR_DEBOUNCER_0_DB_PB_EAST_MASK |
							XPAR_DEBOUNCER_0_DB_PB_SOUTH_MASK |
							XPAR_DEBOUNCER_0_DB_PB_NORTH_MASK |
							XPAR_DEBOUNCER_0_DB_PB_CENTER_MASK |
							XPAR_MILISEC_TIMER_0_IP2INTC_IRPT_MASK |
							XPAR_XPS_TIMEBASE_WDT_0_WDT_INTERRUPT_MASK |
							XPAR_SEC_TIMER_GATED_0_IP2INTC_IRPT_MASK);
	
	// start the watch dog timer
	XWdtTb_Start(&wd_timer);  					
		
    // Enable sec_timer interrupts
    SEC_TIMER_EnableInterrupt(sec_timer_p);
	
	// Enable ms_timer interrupts
	MILISEC_TIMER_EnableInterrupt(ms_timer_p);
	// but dont start it (counter_enable = 0)
	// write to slave reg1(which is counter_enable), the value zero
	xil_printf("  - Stopping MsTimer\r\n");
	MILISEC_TIMER_mWriteSlaveReg1(ms_timer,0, MS_TIMER_STOP);
	
	// enable gated sec timer interrupts
	SEC_TIMER_GATED_EnableInterrupt(sec_gated_timer_p);	
	
	// disable the gated seconds timer counter_enable - write to slave reg1
	SEC_TIMER_GATED_mWriteSlaveReg1(sec_gated_timer,0,TIMERINT_STOP);	
	
    // Enable Microblaze interrupts
    microblaze_enable_interrupts();	
		
	// Reset write FIFO to initial state
	LCD_mResetWriteFIFO(baseaddr);
	
	// Push data to write FIFO
	lcd_init();
	
	//----------------------------------------------------
	// INITIALIZE USER APPS DCLOCK S/W COMPONENTS
    //----------------------------------------------------	
	MENU_Initialise();
	ALARM_Initialise();
	STOPWATCH_Initialise();
	CLOCK_Initialise();
	TIMER_Initialise();
		
	
	//----------------------------------------------------
	// MAIN LOOP:
	// Stay in an infinite loop. Alot of the screen 
	// updating and status checking is done in the 
	// main loop to avoid delaying the interrupt handlers
    //----------------------------------------------------	

	while(1){		
		
		// status checking
		// read dip switch and obtain value
		DipSwitchDataRead = XGpio_DiscreteRead(&GpioInputDIPSW, 1);

		/** NB: CLOCK + ALARM need only a (sec) accuracy, hence 
		 ** status checking can go in the main loop
		
		/***************************************************************/
		/*** Handle Clock Screen Related functions *********************/
		
		// check the dip switch and decide 24/12hr format
		CLOCK_SetTimeFormat(DipSwitchDataRead);	
		
		
		/****************************************************************/
		/*** Handle Alarm Notification **********************************/
		// set/unset the alarm status - switches off LEDs as well
		ALARM_SetAlarm(DipSwitchDataRead);
		
		// this will inform the user visibly + audibly about alarm		
		ALARM_SwitchONOFFAlarmNotification(GpioOutputLEDs, buzzer);
		
		
		/***************************************************************/
		/*** Handle Timer Screen Related functions *********************/
		DipSwitchDataRead = XGpio_DiscreteRead(&GpioInputDIPSW, 1);
		
		TIMER_SetStatus(DipSwitchDataRead, GpioOutputLEDs, buzzer, sec_gated_timer);
		
		// this will inform the user visibly + audibly about alarm
		TIMER_ShowExpiryNotification(GpioOutputLEDs, buzzer, sec_gated_timer);	
				
		
		// ----- DEBUG OUTPUT : Show all screens on terminal -----		
		ALARM_GetDisplayString(strDEBUGAlarmBuff);
		STOPWATCH_GetDisplayString(strDEBUGStopWatchBuff);
		CLOCK_GetDisplayString(strDEBUGClockBuff);
		TIMER_GetDisplayString(strDEBUGTimerBuff);
		
		
		xil_printf( "\r CLOCK: %s,  ALARM: %s,  STOPWATCH: %s,  TIMER: %s,  dipSW: 0x%2X",\
		strDEBUGClockBuff,strDEBUGAlarmBuff,strDEBUGStopWatchBuff,strDEBUGTimerBuff, DipSwitchDataRead );
		//xil_printf("  - display screen\r\n");
		DCLOCK_DisplayScreen();
		
	}
}

