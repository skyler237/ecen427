/*
 * globals.c
 *
 *  Created on: Sep 16, 2016
 *      Author: superman
 */

// Demonstrates one way to handle globals safely in C.
#include "globals.h"

#define BULLET_COUNT 4
#define BULLET_TYPES 6
// Here are the globals.
static point_t tankPosition;
static point_t tankBulletPosition;
static point_t alienBlockPosition;
static bullet_t alienBullets[BULLET_COUNT];


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

void setAlienBlockPosition(uint16_t x, uint16_t y){
	alienBlockPosition.x = x;
	alienBlockPosition.y = y;
}
point_t getAlienBlockPosition(){
	return alienBlockPosition;
}

void createAlienBullet(uint16_t x, uint16_t y){
	// Initializes a new alien bullet
	int i = 0;
	bool created = false;
	while (!created && i < BULLET_COUNT) {
		if (alienBullets[i].position.y == OFF_SCREEN) {
			alienBullets[i].position.x = x;
			alienBullets[i].position.y = y;
			alienBullets[i].type = x % BULLET_TYPES;
			created = true;
		}
		i++;
	}
}


// TODO: Andrew
void updateAlienBullet(uint8_t index, uint16_t x, uint16_t y); // Updates an existing alien bullet's position
bullet_t getAlienBullet(uint8_t index);
point_t getAlienBulletPosition(uint8_t index);

void erodeBunkerBlock(uint8_t bunker_index, uint8_t block_index);
erosionState_t getBlockState(uint8_t bunker_index, uint8_t block_index);
point_t getBlockPosition(uint8_t bunker_index, uint8_t block_index);

void killAlien(uint8_t index);
bool alienIsAlive(uint8_t index);

void setLives(uint8_t lives);
uint8_t getLives();

void setScore(uint16_t score);
uint16_t getScore();
