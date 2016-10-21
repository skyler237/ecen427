/*
 * sound_control.c
 *
 *  Created on: Oct 21, 2016
 *      Author: superman
 */
#include "sound_control.h"
#include "xac97_l.h"
#include <xbasic_types.h>
#include <xio.h>
#include <stdio.h>
#include "xparameters.h"
#define SOUND_FIFO_ADD 128


// Keep track of current indexes for each sound
uint16_t tankFireSoundIndex;
uint32_t soundBuffer[SOUND_FIFO_ADD];

void sound_control_playTankFireSound(){
	tankFireSoundIndex = 0;
}

void sound_control_init(){
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);


    /** Wait until we receive the ready signal **/
    XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);

	/** Enable VRA Mode **/
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, 1);
	XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);

    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankFireSoundRate);
    XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_ADC_Rate, tankFireSoundRate);

    XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);

	tankFireSoundIndex = tankFireSoundFrames;
}

//Loads all of the sounds that we want to play into a buffer that will be loaded into the sound chip
void load_sound_buffer(){
	uint16_t i;
	for(i = 0; i < SOUND_FIFO_ADD; i++){
		uint8_t sound_count;
		if(tankFireSoundIndex < tankFireSoundFrames){
			soundBuffer[i] += tankFireSound[tankFireSoundIndex];
			tankFireSoundIndex++;
			sound_count++;
		}
		else {
			//tankFireSoundIndex = 0;
		}

		if(sound_count == 0) {
			soundBuffer[i] = 0;
		}
		else {
			soundBuffer[i] = soundBuffer[i] / sound_count;
		}
	}
}

//Loads the current contents of the buffer into the FIFO on the sound chip
void sound_control_load_sound() {
	uint16_t i;
	for(i = 0; i < SOUND_FIFO_ADD; i++){
//		xil_printf("Loading: %d\n\r", soundBuffer[i]);
		uint32_t dual_channel = (soundBuffer[i] << 16) + soundBuffer[i];
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, dual_channel);
	}
}

