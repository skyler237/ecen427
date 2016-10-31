##############################################################################
## Filename:          C:\Users\superman\Documents\AndSky\ecen427\SpaceInvaderTestHW2/drivers/pitimer_v1_00_a/data/pitimer_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Mon Oct 31 15:12:24 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "pitimer" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
