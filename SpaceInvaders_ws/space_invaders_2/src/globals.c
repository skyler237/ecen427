/*
 * globals.c
 *
 *  Created on: Sep 16, 2016
 *      Author: superman
 */

// Demonstrates one way to handle globals safely in C.
#include "globals.h"
#include <stdio.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define TANK_WIDTH 15
#define TANK_HEIGHT 8
#define BUNKER_WIDTH 24
#define BUNKER_HEIGHT 18
#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define ALIEN_BLOCK_WIDTH (ALIEN_WIDTH*11 + 30)
#define SCORE_BAR_HEIGHT 15

#define TANK_INIT_X ((SCREEN_WIDTH - TANK_WIDTH)/2)
#define TANK_INIT_Y (SCREEN_HEIGHT -20)

#define BUNKER_0_INIT_X (SCREEN_WIDTH/4*0.5 - BUNKER_WIDTH/2)
#define BUNKER_1_INIT_X (SCREEN_WIDTH/4*1.5 - BUNKER_WIDTH/2)
#define BUNKER_2_INIT_X (SCREEN_WIDTH/4*2.5 - BUNKER_WIDTH/2)
#define BUNKER_3_INIT_X (SCREEN_WIDTH/4*3.5 - BUNKER_WIDTH/2)
#define BUNKER_INIT_Y (TANK_INIT_Y - 40)

#define ALIEN_BLOCK_INIT_X (SCREEN_WIDTH/2 - ALIEN_BLOCK_WIDTH/2)
#define ALIEN_BLOCK_INIT_Y (SCORE_BAR_HEIGHT + 10)


#define BULLET_COUNT 4
#define BULLET_TYPES 6

#define ALIEN_ROWS 5

#define BUNKER_COUNT 4
#define BUNKER_0 0
#define BUNKER_1 1
#define BUNKER_2 2
#define BUNKER_3 3
#define BUNKER_BLOCK_CNT 12 // Really only 10, but blocks 9 and 10 (0 indexed) are initialized to dead
#define BUNKER_BLOCK_ROWS 3
#define BUNKER_BLOCK_COLS 4
#define BUNKER_BLOCK_WIDTH 6

#define INVISIBLE_BLOCK_1 9
#define INVISIBLE_BLOCK_2 10

#define ALIEN_INIT_ROW 0x7FF
#define ALIEN_ROW_MSB 0x400
#define KILL_ALIEN_MASK (0xFFFFFBFF)

#define LIVES_INIT_VALUE 3

typedef struct {erosionState_t erosion_state; point_t position;} block_t;
typedef struct {block_t blocks[BUNKER_BLOCK_CNT]; point_t position;} bunker_t;

typedef struct {
	point_t position;
	bulletType_t type;
} bullet_t;


// Here are the globals.
static point_t tankPosition;
static point_t tankBulletPosition;
static point_t alienBlockPosition;
static uint16_t alienPositions[ALIEN_ROWS]; // low 11 bits of each word represent each alien in a row -- 1 = alive, 0 = dead
static bullet_t alienBullets[BULLET_COUNT];
static bool alienPosIn; // Keeps track of whether the aliens are "in" or "out" -- 1 = in, 0 = out
static bunker_t bunkers[BUNKER_COUNT];
static uint8_t current_lives;
static uint16_t current_score;


void globals_init() {
	tankPosition.x = TANK_INIT_X;
	tankPosition.y = TANK_INIT_Y;
	tankPosition.prev_x = TANK_INIT_X;
	tankPosition.prev_y = TANK_INIT_Y;

	alienBlockPosition.x = ALIEN_BLOCK_INIT_X;
	alienBlockPosition.y = ALIEN_BLOCK_INIT_Y;
	alienBlockPosition.prev_x = ALIEN_BLOCK_INIT_X;
	alienBlockPosition.prev_y = ALIEN_BLOCK_INIT_Y;
	int i;
	for(i=0; i < ALIEN_ROWS; i++) {
		alienPositions[i] = ALIEN_INIT_ROW;
	}

	bunkers[BUNKER_0].position.x = BUNKER_0_INIT_X;
	bunkers[BUNKER_0].position.y = BUNKER_INIT_Y;
	bunkers[BUNKER_1].position.x = BUNKER_1_INIT_X;
	bunkers[BUNKER_1].position.y = BUNKER_INIT_Y;
	bunkers[BUNKER_2].position.x = BUNKER_2_INIT_X;
	bunkers[BUNKER_2].position.y = BUNKER_INIT_Y;
	bunkers[BUNKER_3].position.x = BUNKER_3_INIT_X;
	bunkers[BUNKER_3].position.y = BUNKER_INIT_Y;

	current_lives = LIVES_INIT_VALUE;

}

void init_bunker_blocks(uint8_t bunker_index){
	point_t bunker_pos = bunkers[bunker_index].position;
	int block_index;
	for(block_index = 0; block_index < BUNKER_BLOCK_CNT; block_index++){
		bunkers[bunker_index].blocks[block_index].position.x = bunker_pos.x + BUNKER_BLOCK_WIDTH*(block_index % BUNKER_BLOCK_COLS);
		bunkers[bunker_index].blocks[block_index].position.y = bunker_pos.y + BUNKER_BLOCK_WIDTH*(block_index / BUNKER_BLOCK_COLS);
		if (bunker_index == INVISIBLE_BLOCK_1 || bunker_index == INVISIBLE_BLOCK_2){
			bunkers[bunker_index].blocks[block_index].erosion_state = DEAD;
		}
		else {
			bunkers[bunker_index].blocks[block_index].erosion_state = WHOLE;
		}
	}
}

// Here are the accessors.
void global_setTankPositionGlobal(uint16_t x, uint16_t y) {
	tankPosition.prev_x = tankPosition.x;
	tankPosition.prev_y = tankPosition.y;
	tankPosition.x = x;
	tankPosition.y = y;
}

void global_moveTank(int8_t dx, int8_t dy) {
	global_setTankPositionGlobal(tankPosition.x + dx, tankPosition.y + dy);
}

point_t global_getTankPositionGlobal() {
  return tankPosition;
}

void global_setTankBulletPosition(uint16_t x, uint16_t y) {
	tankBulletPosition.prev_x = tankBulletPosition.x;
		tankBulletPosition.prev_y = tankBulletPosition.y;
  tankBulletPosition.x = x;
  tankBulletPosition.y = y;
}

point_t global_getTankBulletPosition() {
  return tankBulletPosition;
}

void global_setAlienBlockPosition(uint16_t x, uint16_t y){
	alienBlockPosition.prev_x = alienBlockPosition.x;
	alienBlockPosition.prev_y = alienBlockPosition.y;
	alienBlockPosition.x = x;
	alienBlockPosition.y = y;
}

void global_moveAlienBlock(uint16_t dx, uint16_t dy){
	global_setAlienBlockPosition(alienBlockPosition.x + dx, alienBlockPosition.y + dy);
}

point_t global_getAlienBlockPosition(){
	return alienBlockPosition;
}

void global_createAlienBullet(uint16_t x, uint16_t y){
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

void global_updateAlienBullet(uint8_t index, uint16_t x, uint16_t y) {
	// Updates an existing alien bullet's position
	alienBullets[index].position.prev_x = alienBullets[index].position.x;
	alienBullets[index].position.prev_y = alienBullets[index].position.y;
	alienBullets[index].position.x = x;
	alienBullets[index].position.y = y;
}
bulletType_t global_getAlienBulletType(uint8_t index){
	return alienBullets[index].type;
}
point_t global_getAlienBulletPosition(uint8_t index){
	return alienBullets[index].position;
}

void global_erodeBunkerBlock(uint8_t bunker_index, uint8_t block_index){
	if(global_getBlockState(bunker_index, block_index) != DEAD) {
		bunkers[bunker_index].blocks[block_index].erosion_state++;
	}
}
void global_initBlockState(uint8_t bunker_index, uint8_t block_index){
	bunkers[bunker_index].blocks[block_index].erosion_state = WHOLE;
}
erosionState_t global_getBlockState(uint8_t bunker_index, uint8_t block_index){
	return bunkers[bunker_index].blocks[block_index].erosion_state;
}
void global_setBlockPosition(uint8_t bunker_index, uint8_t block_index, uint8_t x, uint8_t y){
	bunkers[bunker_index].blocks[block_index].position.x = x;
	bunkers[bunker_index].blocks[block_index].position.y = y;
}
point_t global_getBlockPosition(uint8_t bunker_index, uint8_t block_index){
	return bunkers[bunker_index].blocks[block_index].position;
}

point_t global_getBunkerPosition(uint8_t bunker_index){
	return bunkers[bunker_index].position;
}

void global_setBunkerPosition(uint8_t bunker_index, uint8_t x, uint8_t y){
	bunkers[bunker_index].position.x = x;
	bunkers[bunker_index].position.y = y;
}

void global_killAlien(uint8_t row, uint8_t col){
	alienPositions[row] = alienPositions[row] & (KILL_ALIEN_MASK >> col);
}

bool global_isAlienAlive(uint8_t row, uint8_t col) {
	uint16_t alien_row = alienPositions[row];
	return alien_row & (ALIEN_ROW_MSB >> col);
}

bool global_isAlienPoseIn() {
	return alienPosIn;
}
void global_toggleAlienPose() {
	alienPosIn = !alienPosIn;
}

void global_setLives(uint8_t lives){
	current_lives = lives;
}
uint8_t global_getLives(){
	return current_lives;
}

void global_setScore(uint16_t score){
	current_score = score;
}
void global_addToScore(uint16_t score){
	current_score += score;
}
uint16_t global_getScore(){
	return current_score;
}
