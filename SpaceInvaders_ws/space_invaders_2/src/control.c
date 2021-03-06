/*
 * control.c
 *
 *  Created on: Sep 26, 2016
 *      Author: superman
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "globals.h"
#include "render.h"

#define SCREEN_RIGHT_EDGE SCREEN_WIDTH
#define SCREEN_LEFT_EDGE 0

#define ALIEN_MOVE_X 4
#define ALIEN_MOVE_Y 6
#define EDGE_2_ALIEN_GAP 10

#define BULLET_SPEED 6

// Initialize the control
void control_init(){
	// Might need later...
}

/**
* Updates the alien block one frame
*/
void control_updateAlienBlock(uint32_t* framePtr) {
	static bool isMovingRight = true;
	// Toggle the in/out position
	global_toggleAlienPose();

	// Check the bounds
	uint8_t row = 0;
	uint8_t col = 0;
	// Get left-most alive alien position
	uint8_t left_most_col = 0;
	while(!global_isAlienAlive(row, col)) { // Keep looking until we find an alien that's alive
		if(row >= ALIEN_ROW_MAX) { // If we are at the last row...
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

	// Make the appropriate move
	point_t blockPosition = global_getAlienBlockPosition(); // Get the current alien block position
	// If we are moving right and are at the right edge...
	if(isMovingRight && (blockPosition.x + ALIEN_X_SPACING*(right_most_col) + ALIEN_WIDTH + EDGE_2_ALIEN_GAP) >= SCREEN_RIGHT_EDGE) {
		// Move down
		global_moveAlienBlock(0, ALIEN_MOVE_Y);
		// Start moving left
		isMovingRight = false;
	}
	// If we are moving left and are at the left edge...
	else if(!isMovingRight && (blockPosition.x + ALIEN_X_SPACING*left_most_col - EDGE_2_ALIEN_GAP) <= SCREEN_LEFT_EDGE) {
		// Move down
		global_moveAlienBlock(0, ALIEN_MOVE_Y);
		// Start moving right
		isMovingRight = true;
	}
	// If we are moving right, but not at an edge,
	else if(isMovingRight) {
		// Move right
		global_moveAlienBlock(ALIEN_MOVE_X, 0);
	}
	// If we are moving left, but not at an edge,
	else {
		// Move left
		global_moveAlienBlock(-ALIEN_MOVE_X, 0);
	}

	// TODO: Kill bunker blocks if colliding

	// Refresh all the aliens
	render_refreshAliens(framePtr);
}

/**
 * Check for collision with aliens and kill if needed
 */
void control_checkTankBulletAlienCollision(point_t bulletPoint) {
	point_t alienBlockPos = global_getAlienBlockPosition();

	if(bulletPoint.y < alienBlockPos.y || bulletPoint.y > alienBlockPos.y + ALIEN_BLOCK_HEIGHT ||
			bulletPoint.x < alienBlockPos.x || bulletPoint.x > alienBlockPos.x + ALIEN_BLOCK_WIDTH) {
		return;
	}
	else {
		uint16_t x_offset = bulletPoint.x - alienBlockPos.x;
		uint16_t y_offset = bulletPoint.y - alienBlockPos.y;

		// Check to see if we hit the actual guise or a gap between them
		if(x_offset % ALIEN_X_SPACING > ALIEN_WIDTH || y_offset % ALIEN_Y_SPACING > ALIEN_HEIGHT) {
			return;
		}
		else { // We know there is a collision
			// Get the column
			uint8_t alien_col =  x_offset / ALIEN_X_SPACING;
			uint8_t alien_row = y_offset / ALIEN_Y_SPACING;

			// Kill the alien if it's not already dead
			if(global_isAlienAlive(alien_row, alien_col)) {
				global_killAlien(alien_row, alien_col); // Kill the alien
				global_setTankBulletPosition(OFF_SCREEN, OFF_SCREEN); // Move the bullet off screen
			}
		}



	}
}

/**
 * Check for collision with bunkers and erode if needed
 */
void control_checkTankBulletBunkerCollision(point_t bulletPoint) {
	uint8_t bunker_index;
	uint8_t block_index;
	// Check the y range
	if(bulletPoint.y < BUNKER_Y || bulletPoint.y > BUNKER_Y + BUNKER_HEIGHT) {
		// If we aren't in the right y range, return
		return;
	}
	else {
		uint8_t x_offset;
		uint8_t y_offset;

		// Check to see which bunker was hit, if any
		if (bulletPoint.x >= BUNKER_0_X && bulletPoint.y <= BUNKER_0_X + BUNKER_WIDTH) { // Check for bunker 0 collision
			x_offset = bulletPoint.x - BUNKER_0_X; // Get x offset from bunker position
			y_offset = bulletPoint.y - BUNKER_Y; // Get y offset from bunker position
			bunker_index = BUNKER_0;
		}
		else if (bulletPoint.x >= BUNKER_1_X && bulletPoint.y <= BUNKER_1_X + BUNKER_WIDTH) { // Check for bunker 1 collision
			x_offset = bulletPoint.x - BUNKER_1_X; // Get x offset from bunker position
			y_offset = bulletPoint.y - BUNKER_Y; // Get y offset from bunker position
			bunker_index = BUNKER_1;
		}
		else if (bulletPoint.x >= BUNKER_2_X && bulletPoint.y <= BUNKER_2_X + BUNKER_WIDTH) { // Check for bunker 2 collision
			x_offset = bulletPoint.x - BUNKER_2_X; // Get x offset from bunker position
			y_offset = bulletPoint.y - BUNKER_Y; // Get y offset from bunker position
			bunker_index = BUNKER_2;
		}
		else if (bulletPoint.x >= BUNKER_3_X && bulletPoint.y <= BUNKER_3_X + BUNKER_WIDTH) { // Check for bunker 3 collision
			x_offset = bulletPoint.x - BUNKER_3_X; // Get x offset from bunker position
			y_offset = bulletPoint.y - BUNKER_Y; // Get y offset from bunker position
			bunker_index = BUNKER_3;
		}
		else {
			return; // Didn't hit a bunker
		}
		// Calculate which block was hit
		block_index = (x_offset/BUNKER_BLOCK_SIZE) + (y_offset/BUNKER_BLOCK_SIZE)*BUNKER_BLOCK_COLS;
		// Check to see if we are inside the arch (the "invisible blocks")
		if(block_index == INVISIBLE_BLOCK_1 || block_index == INVISIBLE_BLOCK_2) {
			// If we are inside the arch of the bunker, return
			return;
		}
		else {
			// Otherwise, erode the block
			global_erodeBunkerBlock(bunker_index, block_index);

			// And set the bullet off screen
			global_setTankBulletPosition(OFF_SCREEN, OFF_SCREEN); // Move the bullet off screen
		}

	}
}

/**
* Update all the bullets
*/
void control_manageBullets(uint32_t* framePtr) {
	// Move the tank bullet upward
	global_moveTankBullet(0, -BULLET_SPEED);
	point_t bulletPoint = global_getTankBulletPosition();
	// Check for collision with aliens and kill if needed
	control_checkTankBulletAlienCollision(bulletPoint);
	// Check for collision with bunker and erode if needed
	control_checkTankBulletBunkerCollision(bulletPoint);

	// Iterate through all alien bullets
	uint8_t i;
	for(i = 0; i < BULLET_COUNT; i++){
		// Move each alien bullet downward
		global_moveAlienBullet(i, 0, BULLET_SPEED);
	}
}

/**
* Fire a random alien bullet at a random position
*/
void control_fireAlienBullet(uint32_t* framePtr) {

	int8_t row, col;
	// Assume that the current column is empty
	bool column_empty = true;
	while(column_empty) { // Keep looping until we find a non-empty column
		col = rand() % ALIEN_COLS; // Choose a random column
		row = ALIEN_ROW_MAX; // Start at the bottom row of aliens

		while(row >= 0 && column_empty) { // Look through all possible rows until we find an existing alien
			// Check to see if the alien is alive
			if (global_isAlienAlive(row, col)){
				// If so, exit both loops
				column_empty = false;
			}
			else { // If not, move up one row
				row--;
			}
		}
	}

	// Create a bullet at the position calculated above
	global_createAlienBullet(row, col);
}
