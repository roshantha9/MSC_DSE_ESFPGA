/************************************************************************
 * @file	lcd_helper.h
 * @author  106033467
 * @version 1.0
 *
 * @brief 	Contains function prototype defines and constant defines
 *		  	related to lcd_helper.c
 *
 ************************************************************************/
//----------------------------------------------------------------------
// SYSTEM INCLUDES
//----------------------------------------------------------------------
#include "xbasic_types.h"

//----------------------------------------------------------------------
// USER PERIPHERAL INCLUDES
//----------------------------------------------------------------------
#include "lcd.h"


//----------------------------------------------------------------------
// CONSTANT DEFINES
//----------------------------------------------------------------------

// Display feature flags
#define LCD_DISPLAY_ON 0x4
#define LCD_DISPLAY_OFF 0x0
#define LCD_CURSOR_ON 0x2
#define LCD_CURSOR_OFF 0x0
#define LCD_CURSOR_BLINK 0x1
#define LCD_CURSOR_NO_BLINK 0x0

// Entry modes
#define LCD_ENTRY_CURSOR_RIGHT 0x2
#define LCD_ENTRY_CURSOR_LEFT 0x0
#define LCD_ENTRY_SCROLL_RIGHT 0x1
#define LCD_ENTRY_SCROLL_LEFT 0x3

// Cursor nudge
#define LCD_NUDGE_RIGHT 0x4
#define LCD_NUDGE_LEFT 0x0

// Scroll display
#define LCD_SCROLL_RIGHT 0x4
#define LCD_SCROLL_LEFT 0x0

Xuint32 baseaddr;	// base address

//----------------------------------------------------------------------
// GLOBAL FUNCTIONS - EXTERNS
//----------------------------------------------------------------------

// These functions are helper functions to I/O to the LCD driver
extern void lcd_clear();
extern void lcd_home();
extern void lcd_entry_mode(Xuint32 command);
extern void lcd_display_ctrl(Xuint32 command);
extern void lcd_nudge_cursor(Xuint32 command);
extern void lcd_scroll(Xuint32 command);
extern void lcd_cgram_set(Xuint32 address);
extern void lcd_ddram_set(Xuint32 address);
extern void lcd_data_write(Xuint32 data);

//Convenience functions 
extern void lcd_init();
extern void lcd_move(Xuint32 line, Xuint32 pos);
extern void lcd_print(char *text);
