//////////////////////////////////////////////////////////////////////////////
// Filename:          D:\Xtemp\hrm506_EMB\Lab56/drivers/pbuzzer_v1_00_a/src/pbuzzer.c
// Version:           1.00.a
// Description:       pbuzzer Driver Source File
// Date:              Mon Mar 21 17:26:21 2011 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "pbuzzer.h"

/************************** Function Definitions ***************************/

/**
 *
 * Enable all possible interrupts from PBUZZER device.
 *
 * @param   baseaddr_p is the base address of the PBUZZER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void PBUZZER_EnableInterrupt(void * baseaddr_p)
{
  Xuint32 baseaddr;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Enable all interrupt source from user logic.
   */
  PBUZZER_mWriteReg(baseaddr, PBUZZER_INTR_IPIER_OFFSET, 0x00000001);

  /*
   * Enable all possible interrupt sources from device.
   */
  PBUZZER_mWriteReg(baseaddr, PBUZZER_INTR_DIER_OFFSET,
    INTR_TERR_MASK
    | INTR_DPTO_MASK
    | INTR_IPIR_MASK
    );

  /*
   * Set global interrupt enable.
   */
  PBUZZER_mWriteReg(baseaddr, PBUZZER_INTR_DGIER_OFFSET, INTR_GIE_MASK);
}

/**
 *
 * Example interrupt controller handler for PBUZZER device.
 * This is to show example of how to toggle write back ISR to clear interrupts.
 *
 * @param   baseaddr_p is the base address of the PBUZZER device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void PBUZZER_Intr_DefaultHandler(void * baseaddr_p)
{
  Xuint32 baseaddr;
  Xuint32 IntrStatus;
Xuint32 IpStatus;
  baseaddr = (Xuint32) baseaddr_p;

  /*
   * Get status from Device Interrupt Status Register.
   */
  IntrStatus = PBUZZER_mReadReg(baseaddr, PBUZZER_INTR_DISR_OFFSET);

  xil_printf("Device Interrupt! DISR value : 0x%08x \n\r", IntrStatus);

  /*
   * Verify the source of the interrupt is the user logic and clear the interrupt
   * source by toggle write baca to the IP ISR register.
   */
  if ( (IntrStatus & INTR_IPIR_MASK) == INTR_IPIR_MASK )
  {
    xil_printf("User logic interrupt! \n\r");
    IpStatus = PBUZZER_mReadReg(baseaddr, PBUZZER_INTR_IPISR_OFFSET);
    PBUZZER_mWriteReg(baseaddr, PBUZZER_INTR_IPISR_OFFSET, IpStatus);
  }

}

