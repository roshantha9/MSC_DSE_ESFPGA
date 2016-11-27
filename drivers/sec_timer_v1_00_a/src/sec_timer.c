//////////////////////////////////////////////////////////////////////////////
// Filename:          D:\Xtemp\hrm506_EMB\Lab56/drivers/sec_timer_v1_00_a/src/sec_timer.c
// Version:           1.00.a
// Description:       sec_timer Driver Source File
// Date:              Tue Mar 01 14:55:52 2011 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "sec_timer.h"

/************************** Function Definitions ***************************/

/**
 *
 * Enable all possible interrupts from SEC_TIMER device.
 *
 * @param   baseaddr_p is the base address of the SEC_TIMER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void SEC_TIMER_EnableInterrupt(void * baseaddr_p)
{
  Xuint32 baseaddr;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Enable all interrupt source from user logic.
   */
  SEC_TIMER_mWriteReg(baseaddr, SEC_TIMER_INTR_IPIER_OFFSET, 0x00000001);

  /*
   * Enable all possible interrupt sources from device.
   */
  SEC_TIMER_mWriteReg(baseaddr, SEC_TIMER_INTR_DIER_OFFSET,
    INTR_TERR_MASK
    | INTR_DPTO_MASK
    | INTR_IPIR_MASK
    );

  /*
   * Set global interrupt enable.
   */
  SEC_TIMER_mWriteReg(baseaddr, SEC_TIMER_INTR_DGIER_OFFSET, INTR_GIE_MASK);
}

/**
 *
 * Example interrupt controller handler for SEC_TIMER device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the SEC_TIMER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void SEC_TIMER_Intr_DefaultHandler(void * baseaddr_p)
{
  Xuint32 baseaddr;
  Xuint32 IntrStatus;
Xuint32 IpStatus;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Get status from Device Interrupt Status Register.
   */
  IntrStatus = SEC_TIMER_mReadReg(baseaddr, SEC_TIMER_INTR_DISR_OFFSET);

  xil_printf("Device Interrupt! DISR value : 0x%08x \n\r", IntrStatus);

  /*
   * Verify the source of the interrupt is the user logic and clear the interrupt
   * source by toggle write baca to the IP ISR register.
   */
  if ( (IntrStatus & INTR_IPIR_MASK) == INTR_IPIR_MASK )
  {
    xil_printf("User logic interrupt! \n\r");
    IpStatus = SEC_TIMER_mReadReg(baseaddr, SEC_TIMER_INTR_IPISR_OFFSET);
    SEC_TIMER_mWriteReg(baseaddr, SEC_TIMER_INTR_IPISR_OFFSET, IpStatus);
  }

}

