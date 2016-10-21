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

void sound_control_init();
void load_sound_buffer();
void sound_control_load_sound();
void sound_control_playTankFireSound();


#endif /* SOUND_CONTROL_H_ */
