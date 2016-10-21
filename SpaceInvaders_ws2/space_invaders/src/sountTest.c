/*
 * sountTest.c
 *
 *  Created on: Oct 21, 2016
 *      Author: superman
 */
#include "sound_control.h"
#include "xparameters.h"
#include "xac97_l.h"
#include <stdio.h>

int main() {
	sound_control_init();
	while(1) {
		if (XAC97_isInFIFOEmpty(XPAR_AXI_AC97_0_BASEADDR)) {
			load_sound_buffer();
			sound_control_load_sound();
		}
	}
}
