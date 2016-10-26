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
#include "globals.h"
#define SOUND_FIFO_ADD 128
#define ALIEN_SOUND_COUNT 4
#define CHANGE_VOLUME_INCREMENT (AC97_VOL_MIN / 10)


// Keep track of current indexes for each sound
static uint16_t tankFireSoundIndex;
static uint16_t alienMarchingSoundIndex;
static uint16_t saucerFlyingSoundIndex;
static uint8_t playingSaucerSound;
static uint16_t tankExplosionSoundIndex;
static uint16_t saucerExplosionSoundIndex;
static uint16_t alienExplosionSoundIndex;
static uint32_t soundBuffer[SOUND_FIFO_ADD];
static uint8_t whichAlienSound;

void sound_control_playTankFireSound(){
	tankFireSoundIndex = 0;
}

void sound_control_playAlienMarchingSound(){
	alienMarchingSoundIndex = 0;
	if(++whichAlienSound > ALIEN_SOUND_COUNT){
		whichAlienSound = 1;
	}
}

void sound_control_playSaucerFlyingSound(){
	if(!global_isGameOver()){
		playingSaucerSound = 1;
	}
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
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, tankFire_numberOfSamples);

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
	tankFireSoundIndex = tankFire_numberOfSamples;
	alienMarchingSoundIndex = alien1_numberOfSamples;
	saucerFlyingSoundIndex = saucerFlying_numberOfSamples;
	playingSaucerSound = 0;
	tankExplosionSoundIndex = tankExplosion_numberOfSamples;
	saucerExplosionSoundIndex = saucerExplosion_numberOfSamples;
	alienExplosionSoundIndex = alien_explosion_numberOfSamples;
	whichAlienSound = ALIEN_SOUND_COUNT;
}

void sound_control_simple_test() {
	uint32_t i;
	for(i = 0; i < tankFire_numberOfSamples; i++){
		uint32_t dual_channel = (tankFire_soundData[i] << 16) + tankFire_soundData[i];
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
	int32_t* alienSound;
	switch(whichAlienSound){
	case 1:
		alienSound = alien1_soundData;
		break;
	case 2:
		alienSound = alien2_soundData;
		break;
	case 3:
		alienSound = alien3_soundData;
		break;
	case 4:
		alienSound = alien4_soundData;
		break;
	}
	for(i = 0; i < SOUND_FIFO_ADD; i++){
		uint8_t sound_count = 0;
		soundBuffer[i] = 0;
		/*if(tankFireSoundIndex < tankFireSoundFrames){
			soundBuffer[i] += tankFireSound[tankFireSoundIndex];
			tankFireSoundIndex++;
//			xil_printf("index: %d\n\r", tankFireSoundIndex);
			sound_count++;
		}*/
		soundBuffer[i] += getSoundValue(tankFire_soundData, &tankFireSoundIndex, tankFire_numberOfSamples, &sound_count);
		soundBuffer[i] += getSoundValue(alienSound, &alienMarchingSoundIndex, alien1_numberOfSamples, &sound_count);
		if(playingSaucerSound){
			soundBuffer[i] += saucerFlying_soundData[saucerFlyingSoundIndex++];
			if(saucerFlyingSoundIndex >= saucerFlying_numberOfSamples){
				saucerFlyingSoundIndex = 0;
			}
			sound_count++;
		}
		soundBuffer[i] += getSoundValue(tankExplosion_soundData, &tankExplosionSoundIndex, tankExplosion_numberOfSamples, &sound_count);
		soundBuffer[i] += getSoundValue(saucerExplosion_soundData, &saucerExplosionSoundIndex, saucerExplosion_numberOfSamples, &sound_count);
		soundBuffer[i] += getSoundValue(alien_explosion_soundData, &alienExplosionSoundIndex, alien_explosion_numberOfSamples, &sound_count);
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

void sound_control_increaseVolume() {
	int32_t currVol = XAC97_ReadReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol);
	currVol -= CHANGE_VOLUME_INCREMENT;
	if(currVol < AC97_VOL_MAX){
		currVol = AC97_VOL_MAX;
	}
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, currVol);
}

void sound_control_decreaseVolume() {
	int32_t currVol = XAC97_ReadReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol);
	currVol += CHANGE_VOLUME_INCREMENT;
	if(currVol > AC97_VOL_MIN){
		currVol = AC97_VOL_MIN;
	}
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, currVol);
}

