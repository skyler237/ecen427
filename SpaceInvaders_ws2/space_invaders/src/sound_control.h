/*
 * sound_control.h
 *
 *  Created on: Oct 21, 2016
 *      Author: superman
 */

#ifndef SOUND_CONTROL_H_
#define SOUND_CONTROL_H_
#include <stdint.h>

// Imported values from sound source files
extern const int32_t tankFire_sampleRate;
extern const int32_t tankFire_numberOfSamples;
extern const int32_t tankFire_soundData[];
extern const int32_t alien1_sampleRate;
extern const int32_t alien1_numberOfSamples;
extern const int32_t alien1_soundData[];
extern const int32_t alien2_sampleRate;
extern const int32_t alien2_numberOfSamples;
extern const int32_t alien2_soundData[];
extern const int32_t alien3_sampleRate;
extern const int32_t alien3_numberOfSamples;
extern const int32_t alien3_soundData[];
extern const int32_t alien4_sampleRate;
extern const int32_t alien4_numberOfSamples;
extern const int32_t alien4_soundData[];
extern const int32_t alien_explosion_sampleRate;
extern const int32_t alien_explosion_numberOfSamples;
extern const int32_t alien_explosion_soundData[];
extern const int32_t saucerFlying_sampleRate;
extern const int32_t saucerFlying_numberOfSamples;
extern const int32_t saucerFlying_soundData[];
extern const int32_t saucerExplosion_sampleRate;
extern const int32_t saucerExplosion_numberOfSamples;
extern const int32_t saucerExplosion_soundData[];
extern const int32_t tankExplosion_sampleRate;
extern const int32_t tankExplosion_numberOfSamples;
extern const int32_t tankExplosion_soundData[];

// Initialize the sound controller
void sound_control_init();
// Load sound into our local sound buffer
void load_sound_buffer();
// Load the sound buffer into the sound FIFO
void sound_control_load_sound();
// Play the tank fire sound
void sound_control_playTankFireSound();
// Play the alien marching sound
void sound_control_playAlienMarchingSound();
// Play the saucer flying sound
void sound_control_playSaucerFlyingSound();
void sound_control_stopSaucerFlyingSound();
// Play the tank explosion sound
void sound_control_playTankExplosionSound();
// Play the saucer explosion sound
void sound_control_playSaucerExplosionSound();
// Play the alien explosion sound
void sound_control_playAlienExplosionSound();
// Increase the volume incrementally
void sound_control_increaseVolume();
// Decrease the volume incrementally
void sound_control_decreaseVolume();
// Simple test for playing a single sound
void sound_control_simple_test();



#endif /* SOUND_CONTROL_H_ */
