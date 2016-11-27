//////////////////////////////////////////////////////////////////////////////
// Filename:          D:\Xtemp\hrm506_EMB\Lab56/drivers/lcd_v1_00_a/src/lcd_selftest.c
// Version:           1.00.a
// Description:       Contains a diagnostic self-test function for the lcd driver
// Date:              Tue Mar 01 12:16:53 2011 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "lcd.h"

/************************** Constant Definitions ***************************/


/************************** Variable Definitions ****************************/


/************************** Function Definitions ***************************/

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the LCD instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus LCD_SelfTest(void * baseaddr_p)
{
  int     Index;
  Xuint32 baseaddr;
  Xuint8  Reg8Value;
  Xuint16 Reg16Value;
  Xuint32 Reg32Value;
  
  /*
   * Check and get the device address
   */
  XASSERT_NONVOID(baseaddr_p != XNULL);
  baseaddr = (Xuint32) baseaddr_p;

  xil_printf("******************************\n\r");
  xil_printf("* User Peripheral Self Test\n\r");
  xil_printf("******************************\n\n\r");

  /*
   * Write to the Write Packet FIFO and read back from the Read Packet FIFO
   */
  xil_printf("Packet FIFO test...\n\r");
  xil_printf("   - reset write packet FIFO to initial state\n\r");
  LCD_mResetWriteFIFO(baseaddr);
  xil_printf("   - reset read packet FIFO to initial state \n\r");
  LCD_mResetReadFIFO(baseaddr);
  xil_printf("   - push data to write packet FIFO\n\r");
  for ( Index = 0; Index < 4; Index++ )
  {
    xil_printf("     0x%08x", Index*1+1);
    LCD_mWriteToFIFO(baseaddr, 0, Index*1+1);
    xil_printf("\n\r");
  }
  xil_printf("   - write packet FIFO is %s\n\r", ( LCD_mWriteFIFOFull(baseaddr) ? "full" : "not full" ));
  /* 
   * FIFO example in user logic will loop WrFIFO data back to RdFIFO,
   * so we get number of entries from RdFIFO instead of WrFIFO.
   * Reg32Value = LCD_mWriteFIFOVacancy(baseaddr);
   */
  Reg32Value = LCD_mReadFIFOOccupancy(baseaddr);
  xil_printf("   - number of entries is %s %d\n\r", ( Reg32Value == (Xuint32) 4 ? "expected" : "unexpected" ), Reg32Value);
  xil_printf("   - pop data out from read packet FIFO\n\r");
  for ( Index = 0; Index < 4; Index++ )
  {
    Reg32Value = LCD_mReadFromFIFO(baseaddr, 0);
    xil_printf("     0x%08x", Reg32Value);
    if ( Reg32Value != (Xuint32) Index*1+1 )
    {
      xil_printf("\n\r");
      xil_printf("   - unexpected value read from read packet FIFO\n\r");
      xil_printf("   - write/read packet FIFO failed\n\r");
      LCD_mResetWriteFIFO(baseaddr);
      LCD_mResetReadFIFO(baseaddr);
      return XST_FAILURE;
    }
    xil_printf("\n\r");
  }
  xil_printf("   - read packet FIFO is %s\n\r", ( LCD_mReadFIFOEmpty(baseaddr) ? "empty" : "not empty" ));
  Reg32Value = LCD_mReadFIFOOccupancy(baseaddr);
  xil_printf("   - number of entries is %s %d \n\r", ( Reg32Value == (Xuint32) 0 ? "expected" : "unexpected" ), Reg32Value);
  LCD_mResetWriteFIFO(baseaddr);
  LCD_mResetReadFIFO(baseaddr);
  xil_printf("   - write/read packet FIFO passed \n\n\r");

  return XST_SUCCESS;
}
