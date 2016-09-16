/*
 * globals.h
 *
 *  Created on: Sep 16, 2016
 *      Author: superman
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

enum bool {FALSE, TRUE};

#define OFF_SCREEN 700 //Denotes an off-screen point
typedef struct {unsigned short x; unsigned short y;} point_t;

enum bulletType_t {CROSS_UP, CROSS_DOWN, CROSS_MID2UP, CROSS_MID2DOWN, LIGHTNING1, LIGHTNING2};

typedef struct {
	point_t position;
	bulletType_t type;
} bullet_t;

enum erosionState_t {WHOLE, HIT1, HIT2, HIT3, DEAD};
typedef struct {erosionState_t erosion_state; point_t position;} block_t;
typedef struct {block_t blocks[10];} bunker_t;

void setTankPositionGlobal(point_t val);
point_t getTankPositionGlobal();

void setTankBulletPosition(point_t val);
point_t getTankBulletPosition();

void setAlienBlockPosition(point_t val);
point_t getAlienBlockPosition();

void createAlienBullet(point_t val, bulletType_t type); // Initializes a new alien bullet
void updateAlienBullet(uint8_t index, point_t val); // Updates an existing alien bullet's position
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