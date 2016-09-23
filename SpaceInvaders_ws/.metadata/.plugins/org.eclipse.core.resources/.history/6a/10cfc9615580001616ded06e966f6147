/*
 * globals.c
 *
 *  Created on: Sep 16, 2016
 *      Author: superman
 */

// Demonstrates one way to handle globals safely in C.
#include "globals.h"

// Here are the globals.
static point_t tankPosition;
static point_t tankBulletPosition;
static point_t alienBlockPosition;

// Here are the accessors.
void setTankPositionGlobal(uint16_t x, uint16_t y) {
  tankPosition.x = x;
  tankPosition.y = y;
}

point_t getTankPositionGlobal() {
  return tankPosition;
}

void setTankBulletPosition(uint16_t x, uint16_t y) {
  tankBulletPosition.x = x;
  tankBulletPosition.y = y;
}

point_t getTankBulletPosition() {
  return tankBulletPosition;
}
