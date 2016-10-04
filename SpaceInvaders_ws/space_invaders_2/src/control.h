/*
 * control.h
 *
 *  Created on: Sep 26, 2016
 *      Author: superman
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#include "globals.h"

// Initialize the control
void control_init();

// Move the tank left
void control_moveTankLeft();

// Move the tank right
void control_moveTankRight();

// Update the alien block one frame
// -- Changes position and toggles guises
void control_updateAlienBlock(uint32_t* framePtr);

// Update bullet positions
// -- Also manages collisions
void control_manageBullets(uint32_t* framePtr);

// Fires a random alien bullet from the bottom-most row
void control_fireAlienBullet(uint32_t* framePtr);


#endif /* CONTROL_H_ */
