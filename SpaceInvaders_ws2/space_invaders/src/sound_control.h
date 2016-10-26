/*
 * sound_control.h
 *
 *  Created on: Oct 21, 2016
 *      Author: superman
 */

#ifndef SOUND_CONTROL_H_
#define SOUND_CONTROL_H_
#include <stdint.h>

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

void sound_control_init();
void load_sound_buffer();
void sound_control_load_sound();
void sound_control_playTankFireSound();
void sound_control_simple_test();
void sound_control_playTankFireSound();
void sound_control_playAlienMarchingSound();
void sound_control_playSaucerFlyingSound();
void sound_control_stopSaucerFlyingSound();
void sound_control_playTankExplosionSound();
void sound_control_playSaucerExplosionSound();
void sound_control_playAlienExplosionSound();
void sound_control_increaseVolume();
void sound_control_decreaseVolume();



#endif /* SOUND_CONTROL_H_ */
