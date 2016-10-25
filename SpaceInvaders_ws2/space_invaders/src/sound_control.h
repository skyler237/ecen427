/*
 * sound_control.h
 *
 *  Created on: Oct 21, 2016
 *      Author: superman
 */

#ifndef SOUND_CONTROL_H_
#define SOUND_CONTROL_H_
#include <stdint.h>

extern const int32_t tankFireSoundRate;
extern const int32_t tankFireSoundFrames;
extern const int32_t tankFireSound[];
extern const int32_t alienMarchingSoundRate;
extern const int32_t alienMarchingSoundFrames;
extern const int32_t alienMarchingSound[];
extern const int32_t saucerFlyingSoundRate;
extern const int32_t saucerFlyingSoundFrames;
extern const int32_t saucerFlyingSound[];
extern const int32_t tankExplosionSoundRate;
extern const int32_t tankExplosionSoundFrames;
extern const int32_t tankExplosionSound[];
extern const int32_t saucerExplosionSoundRate;
extern const int32_t saucerExplosionSoundFrames;
extern const int32_t saucerExplosionSound[];
extern const int32_t alienExplosionSoundRate;
extern const int32_t alienExplosionSoundFrames;
extern const int32_t alienExplosionSound[];

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



#endif /* SOUND_CONTROL_H_ */
