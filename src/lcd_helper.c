/************************************************************************
 * @file	lcd_helper.c
 * @author  106033467
 * @version 1.0
 *
 * @brief   This file implements the interface with the LCD driver.
 *
 *  Functionality:
 *  	- Clear, Goto Home, SetEntry mode, Display Control, Nudge Cursor
 *		  Scroll, set CGRAM, set DDRAM, write out Data
 *		- Convenience Functions: Initialise, MoveCursor, Print Out String
 ************************************************************************/
 
//----------------------------------------------------------------------
// USER HEADER INCLUDES
//----------------------------------------------------------------------
#include "lcd_helper.h"
//----------------------------------------------------------------------
// MAIN INTERFACING FUNCTIONS
//----------------------------------------------------------------------

/***********************************************************
 *  @fn  	lcd_clear
 *  @brief 	Clears the screen and sets cursor to (0,0)
 *  @param 	None
 *  @return void
 ***********************************************************/ 
void lcd_clear() {
	
	//xil_printf("\nmain::baseaddr=0x%x",baseaddr);

	// Clears the screen and sets cursor to (0,0)
	LCD_mWriteToFIFO(baseaddr, 0, 0x01);
}

/***********************************************************
 *  @fn  	lcd_home
 *  @brief 	Sets cursor to (0,0)
 *  @param 	None
 *  @return void
 ***********************************************************/ 
void lcd_home() {
	// Sets cursor to (0,0)
	LCD_mWriteToFIFO(baseaddr, 0, 0x00);
}

/***********************************************************
 *  @fn  	lcd_entry_mode
 *  @brief 	Sets the entry mode (see LCD_ENTRY_* constants)
 *  @param 	Xuint32 command
 *  @return void
 ***********************************************************/ 
void lcd_entry_mode(Xuint32 command) {
	// Sets the entry mode (see LCD_ENTRY_* constants)
	LCD_mWriteToFIFO(baseaddr, 0, 0x04 | (command & 0x03));
}

/***********************************************************
 *  @fn  	lcd_display_ctrl
 *  @brief 	Controls display and cursor 
 *			(see LCD_DISPLAY_* and LCD_CURSOR_* constants)
 *  @param 	Xuint32 command
 *  @return void
 ***********************************************************/
void lcd_display_ctrl(Xuint32 command) {
	// Controls display and cursor (see LCD_DISPLAY_* and LCD_CURSOR_* constants)
	LCD_mWriteToFIFO(baseaddr, 0, 0x08 | (command & 0x07));
}

/***********************************************************
 *  @fn  	lcd_nudge_cursor
 *  @brief 	Nudges cursor right or left 
 *			(see LCD_NUDGE_* constants)
 *  @param 	Xuint32 command
 *  @return void
 ***********************************************************/
void lcd_nudge_cursor(Xuint32 command) {
	// Nudges cursor right or left (see LCD_NUDGE_* constants)
	LCD_mWriteToFIFO(baseaddr, 0, 0x10 | (command & 0x04));
}

/***********************************************************
 *  @fn  	lcd_scroll
 *  @brief 	Scrolls entire display 
 *			(see LCD_SCROLL_* constants)
 *  @param 	Xuint32 command
 *  @return void
 ***********************************************************/
void lcd_scroll(Xuint32 command) {
	// Scrolls entire display (see LCD_SCROLL_* constants)
	LCD_mWriteToFIFO(baseaddr, 0, 0x18 | (command & 0x04));
}

/***********************************************************
 *  @fn  	lcd_cgram_set
 *  @brief 	Sets entry point to a CGRAM location, allowing 
 *			custom character generation
 *  @param 	Xuint32 address
 *  @return void
 ***********************************************************/
void lcd_cgram_set(Xuint32 address) {
	// Sets entry point to a CGRAM location, allowing custom character generation
	LCD_mWriteToFIFO(baseaddr, 0, 0x40 | (address & 0x3F));
}

/***********************************************************
 *  @fn  	lcd_ddram_set
 *  @brief 	Sets entry point to a DDRAM location, allowing 
 *			text entry.
 *  @param 	Xuint32 address
 *  @return void
 ***********************************************************/
void lcd_ddram_set(Xuint32 address) {
	// Sets entry point to a DDRAM location, allowing text entry
	LCD_mWriteToFIFO(baseaddr, 0, 0x80 | (address & 0x7F));
}

/***********************************************************
 *  @fn  	lcd_data_write
 *  @brief 	Writes a byte to the current address 
 *			(CGRAM or DDRAM)
 *  @param 	Xuint32 data
 *  @return void
 ***********************************************************/
void lcd_data_write(Xuint32 data) {
	// Writes a byte to the current address (CGRAM or DDRAM)
	LCD_mWriteToFIFO(baseaddr, 0, 0x100 | (data & 0xFF));
}


//----------------------------------------------------------------------
// CONVENIENCE FUNCTIONS
//----------------------------------------------------------------------

/***********************************************************
 *  @fn  	lcd_init
 *  @brief 	Initialise the LCD screen, set to 4-bit, 2 line
 *			mode, switch display on and clear screen.
 *  @param 	None
 *  @return void
 ***********************************************************/
void lcd_init() {
	
	// 4-bit mode, 2 line display, etc.
	LCD_mWriteToFIFO(baseaddr, 0, 0x2C);
	
	// Display on, no cursor
	lcd_display_ctrl(LCD_DISPLAY_ON);

	// Sensible default entry mode
	lcd_entry_mode(LCD_ENTRY_CURSOR_RIGHT);
	
	// Clear the screen
	lcd_clear();
}

/***********************************************************
 *  @fn  	lcd_move
 *  @brief 	Moves the cursor to a new position, line and 
 *			pos both 0-indexed. Line may be 0 or 1, pos 
 *			may be 0 to 39.
 *  @param 	Xuint32 line
 *  @param 	Xuint32 pos
 *  @return void
 ***********************************************************/
void lcd_move(Xuint32 line, Xuint32 pos) {
	
	// Moves the cursor to a new position, line and pos both 0-indexed
	// Line may be 0 or 1, pos may be 0 to 39
	lcd_ddram_set((0x40 * (line%2)) + (pos%40));
}

/***********************************************************
 *  @fn  	lcd_move
 *  @brief 	Prints a string to the screen (if the current 
			address is in DDRAM). Loops through each
			character in the input string argument. Uses
			main function lcd_data_write to output
 *  @param 	char *text	(output data string)
 *  @return void
 ***********************************************************/
void lcd_print(char *text) {
	// Prints a string to the screen (if the current address is in DDRAM)
	while (*text != 0) {
		lcd_data_write(*text);
		++text;
	}
}
