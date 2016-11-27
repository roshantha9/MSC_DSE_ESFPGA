##############################################################################
## Filename:          D:\Xtemp\hrm506_EMB\Lab56/drivers/sec_timer_gated_v1_00_a/data/sec_timer_gated_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Fri Mar 18 16:29:04 2011 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "sec_timer_gated" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
