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
static uint16_t tankFireSoundIndex;
static uint16_t alienMarchingSoundIndex;
static uint16_t saucerFlyingSoundIndex;
static uint8_t playingSaucerSound;
static uint16_t tankExplosionSoundIndex;
static uint16_t saucerExplosionSoundIndex;
static uint16_t alienExplosionSoundIndex;
static uint32_t soundBuffer[SOUND_FIFO_ADD];

void sound_control_playTankFireSound(){
	tankFireSoundIndex = 0;
}

void sound_control_playAlienMarchingSound(){
	alienMarchingSoundIndex = 0;
}

void sound_control_playSaucerFlyingSound(){
	playingSaucerSound = 1;
}

void sound_control_stopSaucerFlyingSound(){
	playingSaucerSound = 0;
	saucerFlyingSoundIndex = 0;
}

void sound_control_playTankExplosionSound(){
	tankExplosionSoundIndex = 0;
}

void sound_control_playSaucerExplosionSound(){
	saucerExplosionSoundIndex = 0;
}

void sound_control_playAlienExplosionSound(){
	alienExplosionSoundIndex = 0;
}

void sound_control_init(){
	// Hard reset
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);

    // Wait until we receive the ready signal
    XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);

	// Enable VRA Mode
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, 1);

	// Set frequency
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankFireSoundRate);

	// Set volume
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, AC97_VOL_MID);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, AC97_VOL_MAX);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVolMono, AC97_VOL_MAX);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCMOutVol, AC97_VOL_MAX);

	// Clear FIFO
	XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);

	// Enable interrupts
	XAC97_mSetControl(XPAR_AXI_AC97_0_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);

    // Initialize sound data indices
	tankFireSoundIndex = tankFireSoundFrames;
}

void sound_control_simple_test() {
	uint32_t i;
	for(i = 0; i < tankFireSoundFrames; i++){
		uint32_t dual_channel = (tankFireSound[i] << 16) + tankFireSound[i];
		XAC97_WriteFifo(XPAR_AXI_AC97_0_BASEADDR, dual_channel);
	}
}

int32_t getSoundValue(int32_t sound[], uint16_t* index, uint16_t max_index, uint8_t* sound_count){
	if(*index < max_index){
		int32_t ret = sound[*index];
		(*index)++;
//			xil_printf("index: %d\n\r", tankFireSoundIndex);
		(*sound_count)++;
		return ret;
	}
	return 0;
}

//Loads all of the sounds that we want to play into a buffer that will be loaded into the sound chip
void load_sound_buffer(){
	uint16_t i;
	for(i = 0; i < SOUND_FIFO_ADD; i++){
		uint8_t sound_count = 0;
		soundBuffer[i] = 0;
		/*if(tankFireSoundIndex < tankFireSoundFrames){
			soundBuffer[i] += tankFireSound[tankFireSoundIndex];
			tankFireSoundIndex++;
//			xil_printf("index: %d\n\r", tankFireSoundIndex);
			sound_count++;
		}*/
		soundBuffer[i] += getSoundValue(tankFireSound, &tankFireSoundIndex, tankFireSoundFrames, &sound_count);
		soundBuffer[i] += getSoundValue(alienMarchingSound, &alienMarchingSoundIndex, alienMarchingSoundFrames, &sound_count);
		if(playingSaucerSound){
			soundBuffer[i] += saucerFlyingSound[saucerFlyingSoundIndex++];
			if(saucerFlyingSoundIndex >= saucerFlyingSoundFrames){
				saucerFlyingSoundIndex = 0;
			}
		}
		soundBuffer[i] += getSoundValue(tankExplosionSound, &tankExplosionSoundIndex, tankExplosionSoundFrames, &sound_count);
		soundBuffer[i] += getSoundValue(saucerExplosionSound, &saucerExplosionSoundIndex, saucerExplosionSoundFrames, &sound_count);
		soundBuffer[i] += getSoundvalue(alienExplosionSound, &alienExplosionSoundIndex, alienExplosionSoundFrames, &sound_count);
		if(sound_count == 0) {
			soundBuffer[i] = 0;
		}
		else {
			soundBuffer[i] = soundBuffer[i] / sound_count;
//			xil_printf("%d\n\r",soundBuffer[i]);
		}
	}
}

//Loads the current contents of the buffer into the FIFO on the sound chip
void sound_control_load_sound() {
	uint16_t i;
	for(i = 0; i < SOUND_FIFO_ADD; i++){
		//xil_printf("Loading: %d\n\r", soundBuffer[i]);
		uint32_t dual_channel = (soundBuffer[i] << 16) + soundBuffer[i];
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, dual_channel);
	}
}
