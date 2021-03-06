/*
 * sountTest.c
 *
 *  Created on: Oct 21, 2016
 *      Author: superman
 */
#include "sound_control.h"
#include "xparameters.h"
#include "xac97_l.h"
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "platform.h"
#include <stdio.h>

void sound_interrupt_handler2() {
	load_sound_buffer();
	sound_control_load_sound();
}

void interrupt_handler_dispatcher2(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the sound controller.
	if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
		sound_interrupt_handler();
	}
}

int main2() {
	init_platform();                   // Necessary for all programs.
	sound_control_init();

	// Set up interrupts
	 microblaze_register_handler(interrupt_handler_dispatcher2, NULL);
	 XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_AXI_AC97_0_INTERRUPT_MASK));
	 XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	 xil_printf("start\n\r");
	 microblaze_enable_interrupts();

	while(1) {
//		sound_control_simple_test();
//		if (XAC97_isInFIFOEmpty(XPAR_AXI_AC97_0_BASEADDR)) {
//			load_sound_buffer();
//			sound_control_load_sound();
//		}
	}
}
