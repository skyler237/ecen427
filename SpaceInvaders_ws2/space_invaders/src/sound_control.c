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

// Definitions
#define SOUND_FIFO_ADD 128 // Size of our local sound buffer
#define ALIEN_SOUND_COUNT 4 // Number of alien sounds
#define ALIEN_SOUND_1 1 // First marching alien sound index
#define ALIEN_SOUND_2 2 // Second marching alien sound index
#define ALIEN_SOUND_3 3 // Third marching alien sound index
#define ALIEN_SOUND_4 4 // Fourth marching alien sound index
#define ONE_CHANNEL_MIN_VOLUME 0x1F // Minimum volume for one channel
#define CHANGE_VOLUME_INCREMENT (ONE_CHANNEL_MIN_VOLUME / 10) // Volume step size


// Keep track of current indexes for each sound
static uint32_t tankFireSoundIndex;						// Index for the tank fire sound
static uint32_t alienMarchingSoundIndex;				// Index for the alien marching sound
static uint32_t saucerFlyingSoundIndex;					// Index for the saucer flying sound		
static uint8_t playingSaucerSound;							// Boolean for if the saucer flying sound should be playing
static uint32_t tankExplosionSoundIndex;				// Index for the tank explosion sound
static uint32_t saucerExplosionSoundIndex;			// Index for the saucer explosion sound
static uint32_t alienExplosionSoundIndex;				// Index for the alien explosion sound

static int32_t soundBuffer[SOUND_FIFO_ADD];		// Our sound buffer used to store the sounds we want to load into the FIFO
static uint8_t whichAlienSound;								// incidates which of the four alien sounds should be playing

/**
 * Starts the tank fire sound by resetting it's index back to the beginning
 */
void sound_control_playTankFireSound(){
	tankFireSoundIndex = 0; // Set the index to zero
}

/**
 * Starts the alien marching sound by resetting it's index back to the beginning
 */
void sound_control_playAlienMarchingSound(){
	alienMarchingSoundIndex = 0; // Set the index to zero
	// Loop over the alien sounds
	if(++whichAlienSound > ALIEN_SOUND_COUNT){
		whichAlienSound = 1;
	}
}

/**
 * Starts the saucer flying sound by setting the flag to 1
 */
void sound_control_playSaucerFlyingSound(){
	if(!global_isGameOver()){ // Don't allow the saucer sound to keep playing on game over
		playingSaucerSound = 1; // Set flag to true
	}
}

/**
 * Stops the saucer flying sound by setting the flag to zero
 */
void sound_control_stopSaucerFlyingSound(){
	playingSaucerSound = 0; // Set flag to false
	saucerFlyingSoundIndex = 0; // Set the index to zero
}

/**
 * Starts the tank explosion sound by resetting it's index back to the beginning
 */
void sound_control_playTankExplosionSound(){
	tankExplosionSoundIndex = 0; // Set the index to zero
}

/**
 * Starts the saucer explosion sound by resetting it's index back to the beginning
 */
void sound_control_playSaucerExplosionSound(){
	saucerExplosionSoundIndex = 0; // Set the index to zero
}

/**
 * Starts the alien explosion sound by resetting it's index back to the beginning
 */
void sound_control_playAlienExplosionSound(){
	alienExplosionSoundIndex = 0; // Set the index to zero
}

/**
 * Initializes the sound control
 */
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
	playingSaucerSound = 0; 	// Initialize the saucer sound boolean to false
	tankExplosionSoundIndex = tankExplosion_numberOfSamples;
	saucerExplosionSoundIndex = saucerExplosion_numberOfSamples;
	alienExplosionSoundIndex = alien_explosion_numberOfSamples;
	whichAlienSound = ALIEN_SOUND_COUNT; // Initialize which alien sound will play first
}

// Simple function just to test a single sound
void sound_control_simple_test() {
	uint32_t i;
	for(i = 0; i < tankFire_numberOfSamples; i++){
		uint32_t dual_channel = (tankFire_soundData[i] << 16) + tankFire_soundData[i];
		XAC97_WriteFifo(XPAR_AXI_AC97_0_BASEADDR, dual_channel);
	}
}

// Helper function to get the sound data from its data array
int32_t getSoundValue(const int32_t sound[], uint32_t* index, uint32_t max_index, uint8_t* sound_count){
	// Check to see if the sound is still playing
	if(*index < max_index){
		int32_t ret = sound[*index]; // Extract the sound data
		(*index)++; // increment index to next value
		(*sound_count)++; // Increment the sound count (used to mix sounds)
		return ret;
	}
	return 0;
}

//Loads all of the sounds that we want to play into a buffer that will be loaded into the sound chip
void load_sound_buffer(){
	uint16_t i;
	const int32_t* alienSound;
	// Get the appropriate alien sound
	switch(whichAlienSound){
	case ALIEN_SOUND_1:
		alienSound = alien1_soundData;
		break;
	case ALIEN_SOUND_2:
		alienSound = alien2_soundData;
		break;
	case ALIEN_SOUND_3:
		alienSound = alien3_soundData;
		break;
	case ALIEN_SOUND_4:
		alienSound = alien4_soundData;
		break;
	}
	
	// Fill up our buffer of sounds we want to put in the FIFO
	for(i = 0; i < SOUND_FIFO_ADD; i++){
		uint8_t sound_count = 0; // Initialize sound count to zero
		soundBuffer[i] = 0; // Clear the buffer entry back to zero
		
		// Check for and extract tank sound
		soundBuffer[i] += getSoundValue(tankFire_soundData, &tankFireSoundIndex, tankFire_numberOfSamples, &sound_count);
		// Check for and extract alien sound
		soundBuffer[i] += getSoundValue(alienSound, &alienMarchingSoundIndex, alien1_numberOfSamples, &sound_count);
		
		// If the saucer is on the screen
		if(playingSaucerSound){
			// Extract saucer flying sound
			soundBuffer[i] += saucerFlying_soundData[saucerFlyingSoundIndex++];
			
			// Loop throug the sound
			if(saucerFlyingSoundIndex >= saucerFlying_numberOfSamples){
				saucerFlyingSoundIndex = 0;
			}
			sound_count++; // Increment the sound count (used for mixing)
		}
		// Check for and extract tank explosion sound
		soundBuffer[i] += getSoundValue(tankExplosion_soundData, &tankExplosionSoundIndex, tankExplosion_numberOfSamples, &sound_count);
		// Check for and extract saucer explosion sound
		soundBuffer[i] += getSoundValue(saucerExplosion_soundData, &saucerExplosionSoundIndex, saucerExplosion_numberOfSamples, &sound_count);
		// Check for and extract alien explosion sound
		soundBuffer[i] += getSoundValue(alien_explosion_soundData, &alienExplosionSoundIndex, alien_explosion_numberOfSamples, &sound_count);
		
		if(sound_count == 0) { // If no sounds are playing, insert silence into the buffer
			soundBuffer[i] = 0;
		}
		else {
			// Mix the sounds 
			soundBuffer[i] = soundBuffer[i] / sound_count;
		}
	}
}

//Loads the current contents of the buffer into the FIFO on the sound chip
void sound_control_load_sound() {
	uint16_t i;
	// Loop through our sound buffer
	for(i = 0; i < SOUND_FIFO_ADD; i++){
		// Convert single channel sound to dual channel
		uint32_t dual_channel = (soundBuffer[i] << 16) + soundBuffer[i];
		// Turn of all sounds if the game is over
		if(global_isGameOver()){
			dual_channel = 0;
		}
		// Load the sound data into the FIFO
		XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, dual_channel);
	}
}

// Increases the volume level
void sound_control_increaseVolume() {
	// Get the current volume level
	int8_t currVol = XAC97_ReadReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol);
	// Make the appropriate increment (decrement attenuation = increment sound)
	currVol -= CHANGE_VOLUME_INCREMENT;
	// Saturate at the max value
	if(currVol < AC97_VOL_MAX){
		currVol = AC97_VOL_MAX;
	}
	// Control both channels simultaneously - duplicate the volume level
	int16_t twoChannel = (currVol << 8) | currVol;
	// Write back the appropriate volume level
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, twoChannel);
}

// Decreases the volume level
void sound_control_decreaseVolume() {
	// Get the current volume level
	int8_t currVol = XAC97_ReadReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol);
	// Make the appropriate decrement (increment attenuation = decrement sound)
	currVol += CHANGE_VOLUME_INCREMENT;
	// Saturate at the min value
	if(currVol > ONE_CHANNEL_MIN_VOLUME){
		currVol = ONE_CHANNEL_MIN_VOLUME;
	}
	// Control both channels simultaneously - duplicate the volume level
	int16_t twoChannel = (currVol << 8) | currVol;
	// Write back the appropriate volume level
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, twoChannel);
}

