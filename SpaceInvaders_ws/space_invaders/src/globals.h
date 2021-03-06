/*
 * globals.h
 *
 *  Created on: Sep 16, 2016
 *      Author: superman
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdint.h>
#include <stdbool.h>

#define OFF_SCREEN 700 //Denotes an off-screen point
typedef struct {unsigned short x; unsigned short y;} point_t;

typedef enum { CROSS_UP, CROSS_DOWN, CROSS_MID2UP, CROSS_MID2DOWN, LIGHTNING1, LIGHTNING2 } bulletType_t;

bulletType_t global_test;

typedef struct {
	point_t position;
	bulletType_t type;
} bullet_t;

typedef enum  {WHOLE, HIT1, HIT2, HIT3, DEAD} erosionState_t;
typedef struct {erosionState_t erosion_state; point_t position;} block_t;
typedef struct {block_t blocks[10];} bunker_t;

void setTankPositionGlobal(uint16_t x, uint16_t y);
point_t getTankPositionGlobal();

void setTankBulletPosition(uint16_t x, uint16_t y);
point_t getTankBulletPosition();

void setAlienBlockPosition(uint16_t x, uint16_t y);
point_t getAlienBlockPosition();

void createAlienBullet(uint16_t x, uint16_t y); // Initializes a new alien bullet
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


#endif /* GLOBALS_H_ */
