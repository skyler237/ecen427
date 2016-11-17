/*
 * AI.c
 *
 *  Created on: Nov 9, 2016
 *      Author: superman
 */

#include "globals.h"
#include "control.h"
#include <stdbool.h>
#include <stdio.h>

#define SAFETY_MARGIN 2*BULLET_UPDATE_TIMER_MAX

// Space Invaders AI Pseudo-code
typedef enum { SAFE, UNSAFE, SHOOT_ALIEN, SHOOT_SAUCER }SpotType_t;
SpotType_t tankSpots[SCREEN_WIDTH];

uint8_t alien_col;
int8_t alien_row;

void setAllSpotsSafe() {
	uint16_t i;
	for(i = 0; i < SCREEN_WIDTH; i++) {
		tankSpots[i] = SAFE;
//		xil_printf("set spot %d\n\r", i);
	}
}

bool AI_checkBunkers(int16_t bullet_pos){
	//xil_printf("checkBunkers\n\r");
	uint8_t x_offset;
	uint8_t bunker_index;
	uint8_t block_index = 0;

	// Check to see which bunker was hit, if any
	if (bullet_pos >= BUNKER_0_X && bullet_pos < BUNKER_0_X + BUNKER_WIDTH) { // Check for bunker 0 collision
		x_offset = bullet_pos - BUNKER_0_X; // Get x offset from bunker position
		bunker_index = BUNKER_0;
	}
	else if (bullet_pos >= BUNKER_1_X && bullet_pos < BUNKER_1_X + BUNKER_WIDTH) { // Check for bunker 1 collision
		x_offset = bullet_pos - BUNKER_1_X; // Get x offset from bunker position
		bunker_index = BUNKER_1;
	}
	else if (bullet_pos >= BUNKER_2_X && bullet_pos < BUNKER_2_X + BUNKER_WIDTH) { // Check for bunker 2 collision
		x_offset = bullet_pos - BUNKER_2_X; // Get x offset from bunker position
		bunker_index = BUNKER_2;
	}
	else if (bullet_pos >= BUNKER_3_X && bullet_pos < BUNKER_3_X + BUNKER_WIDTH) { // Check for bunker 3 collision
		x_offset = bullet_pos - BUNKER_3_X; // Get x offset from bunker position
		bunker_index = BUNKER_3;
	}
	else {
		return true; // Didn't hit a bunker
	}
	block_index = (x_offset/BUNKER_BLOCK_SIZE);
	while (block_index < BUNKER_BLOCK_CNT) {
		if(global_getBlockState(bunker_index, block_index) == DEAD) {
			// If we are inside the arch of the bunker, return
			return false;
		}
		block_index += BUNKER_BLOCK_COLS;
	}
	return true;
}

void AI_findSafeSpots() {
	//xil_printf("safeSpots\n\r");
	// Find unsafe spots
	point_t tank_pos = global_getTankPosition();
	uint8_t i;
	for (i = 0; i < BULLET_COUNT; i++) {
	  // Check if the bullet is currently on screen
	  point_t bullet_pos = global_getAlienBulletPosition(i);
	  if(bullet_pos.x != OFF_SCREEN) {
		// Calculate how long it will take the bullet to hit the tank
		int32_t bulletHitTime = ((BULLET_UPDATE_TIMER_MAX - 1) * (TANK_INIT_Y - (bullet_pos.y + BULLET_HEIGHT)) / BULLET_SPEED) - SAFETY_MARGIN + global_getBulletUpdateTimer();

		// Calculate how far the tank can move in one direction during "bulletHitTime"
		uint16_t tankMoveDistance = TANK_SPEED * bulletHitTime / TANK_MOVE_TIMER_MAX;

		// Test the tank predicted position for bullets
		uint16_t tankPredictedRight = tank_pos.x + tankMoveDistance;
		uint16_t tankPredictedLeft = tank_pos.x - tankMoveDistance;
//		xil_printf("tankRight: %d\n\r", tankPredictedRight);
//		xil_printf("bullet: %d\n\r", bullet_pos.x);
		if((tankPredictedRight - 1 <= bullet_pos.x && tankPredictedRight + TANK_WIDTH + 1 >= bullet_pos.x) ||
				(tankPredictedLeft - 1 <= bullet_pos.x && tankPredictedLeft + TANK_WIDTH + 1 >= bullet_pos.x)) {
			int16_t j;
			// Mark the unsafe tank_x positions
			for(j = bullet_pos.x - TANK_WIDTH - 1; j <= bullet_pos.x + 2; j++) {
				if(j >= 0 && j < SCREEN_WIDTH) {
					tankSpots[j] = UNSAFE;
//					xil_printf("spot unsafe: %d\n\r", j);
				}
			}
		}
	  }
	}
}

bool isKillSpot(uint16_t curr_tank_x, int16_t tank_offset) {
	point_t alienBlock_pos = global_getAlienBlockPosition();
	point_t alienPredicted_pos = alienBlock_pos;
	int16_t bullet_pos = curr_tank_x + BULLET_ORIGIN + tank_offset;
	if(tankSpots[curr_tank_x + tank_offset] == UNSAFE){
		return false;
	}
	// Find time for tank bullet to reach alien block
	int32_t bulletHitTime = ((BULLET_UPDATE_TIMER_MAX - 1) * (TANK_INIT_Y - (alienBlock_pos.y + ALIEN_BLOCK_HEIGHT)) / BULLET_SPEED) + global_getBulletUpdateTimer();
	if(tank_offset >= 0){
		bulletHitTime += ((TANK_MOVE_TIMER_MAX - 1) * tank_offset / TANK_SPEED) + global_getTankMoveTimer();
	}
	else {
		bulletHitTime -= ((TANK_MOVE_TIMER_MAX - 1) * tank_offset / TANK_SPEED) - global_getTankMoveTimer();
	}
	uint8_t row = 0;
		uint8_t col = 0;
		// Get left-most alive alien position
		uint8_t left_most_col = 0;
		while(!global_isAlienAlive(row, col)) { // Keep looking until we find an alien that's alive
			if(row >= ALIEN_ROW_MAX) { // If we are at the last row...
				if(col >= ALIEN_COL_MAX) {
					// We have cycled through all the aliens and they are all dead
					return false; // Stop looping
				}
				// move to the top of the next column to the right
				row = 0;
				col++;
			}
			else { // Otherwise, just go down the column
				row++;
			}
		}
		// We have found an alien that's alive -- this is the left-most column
		left_most_col = col;

		// Get right-most alive alien position
		row = 0;
		col = ALIEN_COL_MAX; // Start at the far right column
	uint8_t right_most_col;
	while(!global_isAlienAlive(row, col)) { // Keep looking until we find an alien that's alive
		if(row >= ALIEN_ROW_MAX) {// If we are at the last row...
		// move to the top of the next column to the left
			row = 0;
			col--;
		}
		else { // Otherwise, just go down the column
			row++;
		}
	}
	// We have found an alien that's alive -- this is the right-most column
	right_most_col = col;
	// Check if an alien will be there at that time
	for(alien_row = ALIEN_ROW_MAX; alien_row >= 0; alien_row--) {
		// Calculate number of alien updates
		uint16_t alienUpdates = (bulletHitTime) / ALIEN_MOVE_TIMER_MAX;
		bool isMovingRight = control_aliensMovingRight();

		// Predict alien pos
		uint8_t i;

		for(i = 0; i < alienUpdates; i++) {
			// If we are moving right and are at the right edge...
			if(isMovingRight && (alienPredicted_pos.x + ALIEN_X_SPACING*(right_most_col) + ALIEN_WIDTH + EDGE_2_ALIEN_GAP) >= SCREEN_RIGHT_EDGE) {
				// Move down
				alienPredicted_pos.y -= ALIEN_MOVE_Y;
				// Start moving left
				isMovingRight = false;
			}
			// If we are moving left and are at the left edge...
			else if(!isMovingRight && (alienPredicted_pos.x + ALIEN_X_SPACING*left_most_col - EDGE_2_ALIEN_GAP) <= SCREEN_LEFT_EDGE) {
				// Move down
				alienPredicted_pos.y -= ALIEN_MOVE_Y;
				// Start moving right
				isMovingRight = true;
			}
			// If we are moving right, but not at an edge,
			else if(isMovingRight) {
				// Move right
				alienPredicted_pos.x += ALIEN_MOVE_X;
			}
			// If we are moving left, but not at an edge,
			else {
				// Move left
				alienPredicted_pos.x -= ALIEN_MOVE_X;
			}
		}
		point_t bulletPoint;
		bulletPoint.x = bullet_pos;
		bulletPoint.y = alienPredicted_pos.y + ALIEN_Y_SPACING * alien_row + ALIEN_HEIGHT;
		uint16_t x_offset = bulletPoint.x - alienPredicted_pos.x;
		uint16_t y_offset = bulletPoint.y - alienPredicted_pos.y;
		if(!(bulletPoint.y < alienPredicted_pos.y || bulletPoint.y > alienPredicted_pos.y + ALIEN_BLOCK_HEIGHT ||
				bulletPoint.x < alienPredicted_pos.x || bulletPoint.x > alienPredicted_pos.x + ALIEN_BLOCK_WIDTH)) {
			if (!(x_offset % ALIEN_X_SPACING > ALIEN_WIDTH || y_offset % ALIEN_Y_SPACING > ALIEN_HEIGHT)){
				alien_col =  x_offset / ALIEN_X_SPACING;
				alien_row = y_offset / ALIEN_Y_SPACING;
				if(global_isAlienAlive(alien_row, alien_col) && AI_checkBunkers(bulletPoint.x)) {
					//xil_printf("KillSpot: %d,%d\n\r", bulletPoint.x, bulletPoint.y);
					return true;
				}
			}
		}
		bulletHitTime += (BULLET_UPDATE_TIMER_MAX * (ALIEN_Y_SPACING) / BULLET_SPEED);
	}
	return false;
}

bool isSafe(uint16_t curr_tank_x, int16_t tank_offset){
	return !(tankSpots[curr_tank_x + tank_offset] == UNSAFE);
}

void AI_evadeBullets() {
	xil_printf("evadeBullets\n\r");
	point_t tank_pos = global_getTankPosition();
	// Move towards nearest safe spot
	if(tankSpots[tank_pos.x] == UNSAFE) {
		bool safeSpotFound = false;
		uint16_t offset = TANK_SPEED;
		while(!safeSpotFound && offset < SCREEN_WIDTH) {
			// Do bounds checking here

			if((tank_pos.x + offset + TANK_WIDTH) < SCREEN_WIDTH && tankSpots[tank_pos.x + offset] != UNSAFE) {
				// Move right
				global_setTankDirection(RIGHT);
				safeSpotFound = true;
				break;
			}
			else if((tank_pos.x - offset) >= 0 && tankSpots[tank_pos.x - offset] != UNSAFE) {
				// Move left
				global_setTankDirection(LEFT);
				safeSpotFound = true;
				break;
			}
			offset += TANK_SPEED;
		}
	}
	else {
		// Only move if the tank is unsafe
		global_setTankDirection(STOPPED);
	}
}



void AI_findClosestKillSpot() {
	//xil_printf("ClosestKill\n\r");
	//xil_printf("killSpot\n\r");
	point_t tank_pos = global_getTankPosition();
	bool killSpotFound = false;
	int16_t offset = 0;
	while(!killSpotFound) {
		// Right offset
		if(isKillSpot(tank_pos.x, offset)) {
			// Move right
			global_setTankDirection(RIGHT);
			killSpotFound = true;
			break;
		}
		// Left offset
		if(isKillSpot(tank_pos.x, -offset)) {
			// Move left
			global_setTankDirection(LEFT);
			killSpotFound = true;
			break;
		}
		offset += TANK_SPEED;
		if(offset > SCREEN_WIDTH){
			AI_evadeBullets();
			return;
		}
	}
	if (offset == 0){
		global_setTankDirection(STOPPED);
		if (global_getTankBulletPosition().x == OFF_SCREEN) {
			//xil_printf("Firing Bullet at %d\n\r", tank_pos.x);
			xil_printf("Firing at row: %d, col %d\n\r", alien_row, alien_col);
			global_fireTankBullet();
		}
	}
}


void AI_update() {
	setAllSpotsSafe();
	AI_findSafeSpots();
	AI_findClosestKillSpot();
}
