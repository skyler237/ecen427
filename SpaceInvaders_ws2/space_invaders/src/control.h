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

// Updates the tank (move left or right and shoot)
void control_updateTank();

// Update the alien block one frame
// -- Changes position and toggles guises
void control_updateAlienBlock();

// Update bullet positions
// -- Also manages collisions
void control_manageBullets();

// Fires a random alien bullet from the bottom-most row
void control_fireAlienBullet();

//Returns true if we are moving right, false otherwise
bool control_aliensMovingRight();

bool control_checkTankBulletAlienCollision(point_t bulletPoint, point_t alienBlockPos);


#endif /* CONTROL_H_ */
