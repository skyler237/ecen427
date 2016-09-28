/*
 * control.h
 *
 *  Created on: Sep 26, 2016
 *      Author: superman
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "globals.h"

void control_init();

void control_moveTankLeft();

void control_moveTankRight();

void control_updateAlienBlock(uint32_t* framePtr);

void control_manageBullets(uint32_t* framePtr);

void control_fireAlienBullet(uint32_t* framePtr);


#endif /* CONTROL_H_ */
