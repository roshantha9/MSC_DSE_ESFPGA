//////////////////////////////////////////////////////////////////////////////
// Filename:          D:\Xtemp\hrm506_EMB\Lab56/drivers/sec_timer_gated_v1_00_a/src/sec_timer_gated.h
// Version:           1.00.a
// Description:       sec_timer_gated Driver Header File
// Date:              Fri Mar 18 16:29:04 2011 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////

#ifndef SEC_TIMER_GATED_H
#define SEC_TIMER_GATED_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 * -- SLV_REG1 : user logic slave module register 1
 */
#define SEC_TIMER_GATED_USER_SLV_SPACE_OFFSET (0x00000000)
#define SEC_TIMER_GATED_SLV_REG0_OFFSET (SEC_TIMER_GATED_USER_SLV_SPACE_OFFSET + 0x00000000)
#define SEC_TIMER_GATED_SLV_REG1_OFFSET (SEC_TIMER_GATED_USER_SLV_SPACE_OFFSET + 0x00000004)

/**
 * Interrupt Controller Space Offsets
 * -- INTR_DISR  : device (peripheral) interrupt status register
 * -- INTR_DIPR  : device (peripheral) interrupt pending register
 * -- INTR_DIER  : device (peripheral) interrupt enable register
 * -- INTR_DIIR  : device (peripheral) interrupt id (priority encoder) register
 * -- INTR_DGIER : device (peripheral) global interrupt enable register
 * -- INTR_ISR   : ip (user logic) interrupt status register
 * -- INTR_IER   : ip (user logic) interrupt enable register
 */
#define SEC_TIMER_GATED_INTR_CNTRL_SPACE_OFFSET (0x00000100)
#define SEC_TIMER_GATED_INTR_DISR_OFFSET (SEC_TIMER_GATED_INTR_CNTRL_SPACE_OFFSET + 0x00000000)
#define SEC_TIMER_GATED_INTR_DIPR_OFFSET (SEC_TIMER_GATED_INTR_CNTRL_SPACE_OFFSET + 0x00000004)
#define SEC_TIMER_GATED_INTR_DIER_OFFSET (SEC_TIMER_GATED_INTR_CNTRL_SPACE_OFFSET + 0x00000008)
#define SEC_TIMER_GATED_INTR_DIIR_OFFSET (SEC_TIMER_GATED_INTR_CNTRL_SPACE_OFFSET + 0x00000018)
#define SEC_TIMER_GATED_INTR_DGIER_OFFSET (SEC_TIMER_GATED_INTR_CNTRL_SPACE_OFFSET + 0x0000001C)
#define SEC_TIMER_GATED_INTR_IPISR_OFFSET (SEC_TIMER_GATED_INTR_CNTRL_SPACE_OFFSET + 0x00000020)
#define SEC_TIMER_GATED_INTR_IPIER_OFFSET (SEC_TIMER_GATED_INTR_CNTRL_SPACE_OFFSET + 0x00000028)

/**
 * Interrupt Controller Masks
 * -- INTR_TERR_MASK : transaction error
 * -- INTR_DPTO_MASK : data phase time-out
 * -- INTR_IPIR_MASK : ip interrupt requeset
 * -- INTR_RFDL_MASK : read packet fifo deadlock interrupt request
 * -- INTR_WFDL_MASK : write packet fifo deadlock interrupt request
 * -- INTR_IID_MASK  : interrupt id
 * -- INTR_GIE_MASK  : global interrupt enable
 * -- INTR_NOPEND    : the DIPR has no pending interrupts
 */
#define INTR_TERR_MASK (0x00000001UL)
#define INTR_DPTO_MASK (0x00000002UL)
#define INTR_IPIR_MASK (0x00000004UL)
#define INTR_RFDL_MASK (0x00000020UL)
#define INTR_WFDL_MASK (0x00000040UL)
#define INTR_IID_MASK (0x000000FFUL)
#define INTR_GIE_MASK (0x80000000UL)
#define INTR_NOPEND (0x80)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a SEC_TIMER_GATED register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the SEC_TIMER_GATED device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void SEC_TIMER_GATED_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define SEC_TIMER_GATED_mWriteReg(BaseAddress, RegOffset, Data) \
 	XIo_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a SEC_TIMER_GATED register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the SEC_TIMER_GATED device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 SEC_TIMER_GATED_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define SEC_TIMER_GATED_mReadReg(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from SEC_TIMER_GATED user logic slave registers.
 *
 * @param   BaseAddress is the base address of the SEC_TIMER_GATED device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void SEC_TIMER_GATED_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 SEC_TIMER_GATED_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define SEC_TIMER_GATED_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (SEC_TIMER_GATED_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))
#define SEC_TIMER_GATED_mWriteSlaveReg1(BaseAddress, RegOffset, Value) \
 	XIo_Out32((BaseAddress) + (SEC_TIMER_GATED_SLV_REG1_OFFSET) + (RegOffset), (Xuint32)(Value))

#define SEC_TIMER_GATED_mReadSlaveReg0(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (SEC_TIMER_GATED_SLV_REG0_OFFSET) + (RegOffset))
#define SEC_TIMER_GATED_mReadSlaveReg1(BaseAddress, RegOffset) \
 	XIo_In32((BaseAddress) + (SEC_TIMER_GATED_SLV_REG1_OFFSET) + (RegOffset))

/************************** Function Prototypes ****************************/


/**
 *
 * Enable all possible interrupts from SEC_TIMER_GATED device.
 *
 * @param   baseaddr_p is the base address of the SEC_TIMER_GATED device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void SEC_TIMER_GATED_EnableInterrupt(void * baseaddr_p);

/**
 *
 * Example interrupt controller handler.
 *
 * @param   baseaddr_p is the base address of the SEC_TIMER_GATED device.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void SEC_TIMER_GATED_Intr_DefaultHandler(void * baseaddr_p);

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the SEC_TIMER_GATED instance to be worked on.
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
XStatus SEC_TIMER_GATED_SelfTest(void * baseaddr_p);

#endif // SEC_TIMER_GATED_H
