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

// Helps provide a buffer to ensure tank safety
#define SAFETY_MARGIN 2*BULLET_UPDATE_TIMER_MAX

// Gives us a buffer in case our alien prediction is a little off
#define HIT_BUFFER 2

// Denotes the spot type
typedef enum { SAFE, UNSAFE, SHOOT_ALIEN, SHOOT_SAUCER }SpotType_t;
// Each pixel represents a spot where the tank can be - each spot is marked by its type
SpotType_t tankSpots[SCREEN_WIDTH];

uint8_t alien_col;
int8_t alien_row;

/**
 * Sets all the spots to SAFE
 */
void setAllSpotsSafe() {
	// Loop through all spots
	uint16_t i;
	for(i = 0; i < SCREEN_WIDTH; i++) {
		// Mark each spot as safe
		tankSpots[i] = SAFE;
	}
}


/**
 * Checks to see if tank turret is under a bunker
 */
bool AI_checkBunkers(int16_t bullet_pos){
	uint8_t x_offset; // Offset from left edge of bunker
	uint8_t bunker_index; // The index of the bunker
	uint8_t block_index = 0; // Index of the block

	// Check to see which bunker the bullet is under, if any
	if (bullet_pos >= BUNKER_0_X && bullet_pos < BUNKER_0_X + BUNKER_WIDTH) { // Check for bunker 0
		x_offset = bullet_pos - BUNKER_0_X; // Get x offset from bunker position
		bunker_index = BUNKER_0;
	}
	else if (bullet_pos >= BUNKER_1_X && bullet_pos < BUNKER_1_X + BUNKER_WIDTH) { // Check for bunker 1
		x_offset = bullet_pos - BUNKER_1_X; // Get x offset from bunker position
		bunker_index = BUNKER_1;
	}
	else if (bullet_pos >= BUNKER_2_X && bullet_pos < BUNKER_2_X + BUNKER_WIDTH) { // Check for bunker 2
		x_offset = bullet_pos - BUNKER_2_X; // Get x offset from bunker position
		bunker_index = BUNKER_2;
	}
	else if (bullet_pos >= BUNKER_3_X && bullet_pos < BUNKER_3_X + BUNKER_WIDTH) { // Check for bunker 3
		x_offset = bullet_pos - BUNKER_3_X; // Get x offset from bunker position
		bunker_index = BUNKER_3;
	}
	else {
		return true; // won't hit a bunker - no bunker
	}
	block_index = (x_offset/BUNKER_BLOCK_SIZE);
	bool no_bunker = true;
	while (block_index < BUNKER_BLOCK_CNT) {
		if(global_getBlockState(bunker_index, block_index) != DEAD) {
			// If we are inside the arch of the bunker, return
			no_bunker = false;
		}
		block_index += BUNKER_BLOCK_COLS;
	}
	return no_bunker;
}


/**
 * Finds all the spots that are safe for the tank to be
 */
void AI_findSafeSpots() {
	// Find unsafe spots and mark them
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

		// Check if the bullet would hit the tank predicted position
		if((tankPredictedRight - 1 <= bullet_pos.x && tankPredictedRight + TANK_WIDTH + 1 >= bullet_pos.x) ||
					(tankPredictedLeft - 1 <= bullet_pos.x && tankPredictedLeft + TANK_WIDTH + 1 >= bullet_pos.x)) {
				int16_t j;

				// Loop through the tank width and mark all dangerous positions as unsafe
				for(j = bullet_pos.x - TANK_WIDTH - 1; j <= bullet_pos.x + 2; j++) {
					if(j >= 0 && j < SCREEN_WIDTH) {
						// Mark the unsafe tank_x positions
						tankSpots[j] = UNSAFE;
					}
				}
			}
	  }
	}
}

/**
 * Determines if a given spot will allow us to kill an alien by the time the tank gets there
 * @param curr_tank_x = the current tank position
 * @param tank_offset = the offset from the tank we are checking
 * 											(determines how far the tank will move to get to the spot in question)
 */
bool isKillSpot(uint16_t curr_tank_x, int16_t tank_offset) {
	// Get the current alien block position
	point_t alienBlock_pos = global_getAlienBlockPosition();
	// Initialize the predicted position
	point_t alienPredicted_pos = alienBlock_pos;
	// Get the bullet position
	int16_t bullet_pos = curr_tank_x + BULLET_ORIGIN + tank_offset;
	// We only want safe spots
	if(tankSpots[curr_tank_x + tank_offset] == UNSAFE){
		// If the spot in question is unsafe, return false
		return false;
	}
	// Find time for tank bullet to reach alien block
	int32_t bulletHitTime = (BULLET_UPDATE_TIMER_MAX * ((TANK_INIT_Y - (alienBlock_pos.y + ALIEN_BLOCK_HEIGHT)) / BULLET_SPEED)) + (BULLET_UPDATE_TIMER_MAX - global_getBulletUpdateTimer());

	// Find the time it will take the tank to move to that position
	if(tank_offset >= 0){
		bulletHitTime += ((TANK_MOVE_TIMER_MAX) * (tank_offset / TANK_SPEED)) + (TANK_MOVE_TIMER_MAX -global_getTankMoveTimer());
	}
	else {
		bulletHitTime -= ((TANK_MOVE_TIMER_MAX ) * (tank_offset / TANK_SPEED)) - (TANK_MOVE_TIMER_MAX - global_getTankMoveTimer());
	}

	// ===== Predict the alien block position =====
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
		int16_t alienUpdates = (bulletHitTime) / ALIEN_MOVE_TIMER_MAX - 1;
		if(alienUpdates < 0) {
			alienUpdates = 0;
		}
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

		// Determine where the alien will hit for the alien row we are checking
		point_t bulletPoint;
		bulletPoint.x = bullet_pos;
		bulletPoint.y = alienPredicted_pos.y + ALIEN_Y_SPACING * alien_row + ALIEN_HEIGHT;

		// === Check to see if that alien is still alive ===
		// Get the offset into the alien block
		uint16_t x_offset = bulletPoint.x - alienPredicted_pos.x;
		uint16_t y_offset = bulletPoint.y - alienPredicted_pos.y;
		// Check the bounds - is the bullet even in the right range?
		if(!(bulletPoint.y < alienPredicted_pos.y || bulletPoint.y > alienPredicted_pos.y + ALIEN_BLOCK_HEIGHT ||
				bulletPoint.x < alienPredicted_pos.x || bulletPoint.x > alienPredicted_pos.x + ALIEN_BLOCK_WIDTH)) {
			// Check to see if the bullet will hit the middle few pixels of the alien
			if (!((x_offset % ALIEN_X_SPACING > ALIEN_WIDTH - HIT_BUFFER) || (x_offset % ALIEN_X_SPACING < HIT_BUFFER) || y_offset % ALIEN_Y_SPACING > ALIEN_HEIGHT)) {
				// Calculate the correct alien row and column
				alien_col = x_offset / ALIEN_X_SPACING;
				alien_row = y_offset / ALIEN_Y_SPACING;
				// Check to see if that particular alien is alive
				if(global_isAlienAlive(alien_row, alien_col) && AI_checkBunkers(bulletPoint.x)) {
					// If so, we have found a kill spot!
					return true;
				}
			}
		}
		// As we loop through the rows, update the bullet hit time to take longer (in the y direction)
		bulletHitTime += (BULLET_UPDATE_TIMER_MAX * (ALIEN_Y_SPACING) / BULLET_SPEED);
	}
	return false;
}

/**
 * Determine if a given spot is safe or not
 */
bool isSafe(uint16_t curr_tank_x, int16_t tank_offset){
	// Return true as long as the spot is not unsafe (a.k.a. is safe)
	return !(tankSpots[curr_tank_x + tank_offset] == UNSAFE);
}

/**
 * Doge all the bullets!
 */
void AI_evadeBullets() {
	// Get the current tank position
	point_t tank_pos = global_getTankPosition();
	// Move towards nearest safe spot
	if(tankSpots[tank_pos.x] == UNSAFE) {
		bool safeSpotFound = false;
		uint16_t offset = TANK_SPEED;
		// Loop through nearest spots, starting at the tank position and moving outward
		while(!safeSpotFound && offset < SCREEN_WIDTH) {
			// Check to see if a spot to the right is safe
			if((tank_pos.x + offset + TANK_WIDTH) < SCREEN_WIDTH && tankSpots[tank_pos.x + offset] != UNSAFE) {
				// If so, move right
				global_setTankDirection(RIGHT);
				safeSpotFound = true;
				break;
			}
			// Check to see if a spot to the left is safe
			else if((tank_pos.x - offset) >= 0 && tankSpots[tank_pos.x - offset] != UNSAFE) {
				// If so, mMove left
				global_setTankDirection(LEFT);
				safeSpotFound = true;
				break;
			}
			// increase the offset by one tank movement
			offset += TANK_SPEED;
		}
	}
	else {
		// Only move if the tank is unsafe
		global_setTankDirection(STOPPED);
	}
}

/**
 * Find and move toward the closest kill spot that is safe
 */
void AI_findClosestKillSpot() {
	// Get the current tank position
	point_t tank_pos = global_getTankPosition();
	bool killSpotFound = false;
	int16_t offset = 0;
	// Keep looping until we find a kill spot
	while(!killSpotFound) {
		// Check right offset
		if(isKillSpot(tank_pos.x, offset)) {
			// If it is a kill spot, move right
			global_setTankDirection(RIGHT);
			killSpotFound = true;
			break;
		}
		// Check left offset
		if(isKillSpot(tank_pos.x, -offset)) {
			// If it is a kill spot, move left
			global_setTankDirection(LEFT);
			killSpotFound = true;
			break;
		}
		// increment offset by one tank movement
		offset += TANK_SPEED;
		// If the offset is past the screen, just be sure to keep dodging bullets
		if(offset > SCREEN_WIDTH){
			AI_evadeBullets();
			return;
		}
	}
	// If a kill spot is found where we are currently...
	if (offset == 0){
		// Stop moving
		global_setTankDirection(STOPPED);
		// Check for an available bullet
		if (global_getTankBulletPosition().x == OFF_SCREEN) {
			// If there is an available bullet, fire it!
			global_fireTankBullet();
		}
	}
}

/**
 * General update function - encapsulates all AI functionality
 */
 void AI_update() {
	// Mark all spots as safe initially
	setAllSpotsSafe();
	// Mark all the unsafe spots as unsafe
	AI_findSafeSpots();
	// Find and move towards closest kill spot
	AI_findClosestKillSpot();
}
