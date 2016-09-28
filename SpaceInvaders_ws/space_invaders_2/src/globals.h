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
typedef struct {int16_t x; int16_t y; int16_t prev_x; int16_t prev_y;} point_t;

typedef enum { CROSS_UP, CROSS_DOWN, CROSS_MID2UP, CROSS_MID2DOWN, LIGHTNING1, LIGHTNING2 } bulletType_t;

typedef enum  {WHOLE, HIT1, HIT2, HIT3, DEAD} erosionState_t;

void globals_init();

void global_setTankPositionGlobal(uint16_t x, uint16_t y);
void global_moveTank(int8_t dx, int8_t dy);
point_t global_getTankPositionGlobal();

void global_setTankBulletPosition(uint16_t x, uint16_t y);
void global_moveTankBullet(int8_t dx, int8_t dy);
void global_fireTankBullet();
point_t global_getTankBulletPosition();

void global_setAlienBlockPosition(uint16_t x, uint16_t y);
void global_moveAlienBlock(uint16_t dx, uint16_t dy);
point_t global_getAlienBlockPosition();

void global_createAlienBullet(uint8_t row, uint8_t col); // Initializes a new alien bullet
void global_updateAlienBullet(uint8_t index, uint16_t x, uint16_t y); // Updates an existing alien bullet's position
void global_moveAlienBullet(uint8_t index,int8_t dx, int8_t dy);
bulletType_t global_getAlienBulletType(uint8_t index);
point_t global_getAlienBulletPosition(uint8_t index);

void global_erodeBunkerBlock(uint8_t bunker_index, uint8_t block_index);
erosionState_t global_getBlockState(uint8_t bunker_index, uint8_t block_index);
point_t global_getBlockPosition(uint8_t bunker_index, uint8_t block_index);
point_t global_getBunkerPosition(uint8_t bunker_index);
void global_setBunkerPosition(uint8_t bunker_index, uint8_t x, uint8_t y);

void global_killAlien(uint8_t row, uint8_t col);
bool global_isAlienAlive(uint8_t row, uint8_t col);
bool global_isAlienPoseIn();
void global_toggleAlienPose();

void global_setLives(uint8_t lives);
uint8_t global_getLives();

void global_setScore(uint16_t score);
uint16_t global_getScore();


#endif /* GLOBALS_H_ */
