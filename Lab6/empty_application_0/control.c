/*
 * control.c
 *
 *  Created on: Sep 26, 2016
 *      Author: superman
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "render.h"

#define SCREEN_RIGHT_EDGE SCREEN_WIDTH
#define SCREEN_LEFT_EDGE 0

#define ALIEN_MOVE_X 4
#define ALIEN_MOVE_Y 6
#define EDGE_2_ALIEN_GAP 10

#define BULLET_SPEED 5

#define TANK_BULLET_INDEX -1

// Initialize the control
void control_init(){
	// Might need later...
}

// Updates the tank (move left or right and shoot)
void control_updateTank() {
	// Check to see which direction we need to move
	tankMotion_t tankDirection = global_getTankDirection();

	// Move the tank
	global_moveTank(tankDirection*TANK_SPEED, 0);

}

/**
* Checks for a collision between a specific alien and any of the existing bunker blocks
*/
void checkAlienBlockCollision(uint8_t alienRow, uint8_t alienCol) {
	uint8_t bunker_index;
	point_t alien_pos = global_getAlienPosition(alienRow, alienCol); // Get the alien position

	// Extract the correct bunker index
	if((alien_pos.x >= BUNKER_0_X && alien_pos.x <= BUNKER_0_X + BUNKER_WIDTH) ||  // Left edge of alien
			(alien_pos.x + ALIEN_WIDTH >= BUNKER_0_X && alien_pos.x + ALIEN_WIDTH <= BUNKER_0_X + BUNKER_WIDTH)){ // Right edge of alien
		bunker_index = BUNKER_0;
	}
	else if((alien_pos.x >= BUNKER_1_X && alien_pos.x <= BUNKER_1_X + BUNKER_WIDTH) ||  // Left edge of alien
			(alien_pos.x + ALIEN_WIDTH >= BUNKER_1_X && alien_pos.x + ALIEN_WIDTH <= BUNKER_1_X + BUNKER_WIDTH)){ // Right edge of alien
		bunker_index = BUNKER_1;
	}
	else if((alien_pos.x >= BUNKER_2_X && alien_pos.x <= BUNKER_2_X + BUNKER_WIDTH) ||  // Left edge of alien
			(alien_pos.x + ALIEN_WIDTH >= BUNKER_2_X && alien_pos.x + ALIEN_WIDTH <= BUNKER_2_X + BUNKER_WIDTH)){ // Right edge of alien
		bunker_index = BUNKER_2;
	}
	else if((alien_pos.x >= BUNKER_3_X && alien_pos.x <= BUNKER_3_X + BUNKER_WIDTH) ||  // Left edge of alien
			(alien_pos.x + ALIEN_WIDTH >= BUNKER_3_X && alien_pos.x + ALIEN_WIDTH <= BUNKER_3_X + BUNKER_WIDTH)){ // Right edge of alien
		bunker_index = BUNKER_3;
	}
	else {
		return;
	}

	// Loop through all the blocks
	uint8_t block_index;	
	for (block_index = 0; block_index < BUNKER_BLOCK_CNT; block_index++) {
		// Get the block position
		point_t block_pos = global_getBlockPosition(bunker_index, block_index);
		// Check x-bounds
		if((block_pos.x >= alien_pos.x && block_pos.x <= alien_pos.x + ALIEN_WIDTH) ||
				((block_pos.x + BUNKER_BLOCK_SIZE) >= alien_pos.x && (block_pos.x + BUNKER_BLOCK_SIZE) <= (alien_pos.x + ALIEN_WIDTH)) ) {
			// Check y-bounds
			if(block_pos.y <= alien_pos.y + ALIEN_HEIGHT) {
				// If we overlap and the block isn't already dead...
				if(global_getBlockState(bunker_index, block_index) != DEAD) {

					// Set block to dead
					global_setBlockState(bunker_index, block_index, DEAD);

					// Erase block
					render_eraseBlock(bunker_index, block_index);
				}
				// If we overlap and we are at the bottom row, it's game over!
				else if (block_index >= BOTTOM_BUNKER_ROW) {
					global_endGame();
				}
			}
		}
	}
}

/**
 *  Handles collisions between the alien block and the bunkers
 */
void control_checkAlienBunkerCollision() {
	// Get the alien block position
	point_t alienBlockPos = global_getAlienBlockPosition();
	bool foundBottomAliens = false;

	// Check if we are low enough for a collision
	int8_t row;
	uint8_t col;
	for(row = ALIEN_ROWS - 1; row >= 0; row--) { // Start at the bottom row and work our way up
		// Check if we are low enough to touch the bunkers
		if((alienBlockPos.y + row*ALIEN_Y_SPACING + ALIEN_HEIGHT) >= BUNKER_Y ) {
			// Loop through each alien on the row
			for(col = 0; col < ALIEN_COLS; col++) {
				// If the alien is alive,
				if(global_isAlienAlive(row, col)) {
					// Check to see if there is a collision with one of the blocks
					checkAlienBlockCollision(row, col);
					foundBottomAliens = true; // Once we find an alien low enough to touch the bunkers, we don't need to keep loopin up the rows
				}
			}
			if(foundBottomAliens) { // We only need to check the bottom most row
				return;
			}
		}
	}

}


/**
 * Check for collision with aliens and kill if needed
 */
bool control_checkTankBulletAlienCollision(point_t bulletPoint) {
	point_t alienBlockPos = global_getAlienBlockPosition();

	// Check if the bullet is within the alien block at all
	if((bulletPoint.y < alienBlockPos.y || bulletPoint.y > alienBlockPos.y + ALIEN_BLOCK_HEIGHT ||
			bulletPoint.x < alienBlockPos.x || bulletPoint.x > alienBlockPos.x + ALIEN_BLOCK_WIDTH) &&
			// Also checks the previous positions in case the aliens have moved on top of the bullet since we last checked
			(bulletPoint.prev_y < alienBlockPos.y || bulletPoint.prev_y > alienBlockPos.y + ALIEN_BLOCK_HEIGHT ||
			bulletPoint.prev_x < alienBlockPos.x || bulletPoint.prev_x > alienBlockPos.x + ALIEN_BLOCK_WIDTH)) {
		return false;
	}
	else {
		// Calculate the x and y offsets for the bullet's current position
		uint16_t x_offset = bulletPoint.x - alienBlockPos.x;
		uint16_t y_offset = bulletPoint.y - alienBlockPos.y;
		// Calculate the x and y offsets for the bullet's previous position
		uint16_t x_offset_prev = bulletPoint.prev_x - alienBlockPos.x ;
		uint16_t y_offset_prev = bulletPoint.prev_y - alienBlockPos.y + BULLET_HEIGHT; // Use the bottom of the previous (helps eliminate artifacts)


		// Check to see if we hit the actual guise or a gap between them
		if(!(x_offset_prev % ALIEN_X_SPACING > ALIEN_WIDTH || y_offset_prev % ALIEN_Y_SPACING > ALIEN_HEIGHT)){
			// We know there is a collision with the previous position
			// Get the column
			uint8_t alien_col =  x_offset_prev / ALIEN_X_SPACING;
			uint8_t alien_row = y_offset_prev / ALIEN_Y_SPACING;

			// Kill the alien if it's not already dead
			if(global_isAlienAlive(alien_row, alien_col)) {
				global_killAlien(alien_row, alien_col); // Kill the alien
				global_collideTankBullet(); // Move the bullet off screen

				// Add points to the current score
				global_incrementScore(alien_row);
				return true;
			}
		}
		else if(!(x_offset % ALIEN_X_SPACING > ALIEN_WIDTH || y_offset % ALIEN_Y_SPACING > ALIEN_HEIGHT)) {
			// We know there is a collision with the current position
			// Get the column
			uint8_t alien_col =  x_offset / ALIEN_X_SPACING;
			uint8_t alien_row = y_offset / ALIEN_Y_SPACING;

			// Kill the alien if it's not already dead
			if(global_isAlienAlive(alien_row, alien_col)) {
				global_killAlien(alien_row, alien_col); // Kill the alien
				global_collideTankBullet(); // Move the bullet off screen

				// Add points to the current score
				global_incrementScore(alien_row);
				return true;
			}
		}
		else {
			return false;
		}
		return false;
	}
}

/**
* Updates the alien block one frame
*/
void control_updateAlienBlock() {
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
			if(col >= ALIEN_COL_MAX) {
				// We have cycled through all the aliens and they are all dead
				return; // Stop looping
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

	// Kill bunker blocks if colliding
	control_checkAlienBunkerCollision();
	// Refresh all the aliens
	render_refreshAliens();
}

/**
 * Kills the tank (temporarily)
 */
void control_killTank(){
	point_t tankPos = global_getTankPosition();
	global_setTankPosition(tankPos.x, tankPos.y);//Update prev_x and prev_y so that they are the same
												//That lets the render work
	// Set the state of the tank to dead
	global_killTank();
	// Refresh the sprite to show the dead guise
	render_refreshTank();
}

/**
 * Check for collision with aliens and kill if needed
 * @return: true if a collision occured
 */
bool control_checkAlienBulletTankCollision(point_t bulletPoint, uint8_t index) {
	// Get the tank position
	point_t tankPos = global_getTankPosition();

	// Check to see if the bullet point hit the tank 
	if(bulletPoint.y >= tankPos.y && bulletPoint.y <= (tankPos.y + TANK_HEIGHT) && // Check y bounds
			bulletPoint.x >= tankPos.x && bulletPoint.x <= (tankPos.x + TANK_WIDTH) && // Check x bounds
			!global_isTankDead()) { // Can't hit the tank again if it's already dead
		// Set tank state to dead
		control_killTank(); 
		global_collideAlienBullet(index); // Causes the bullet to disappear
		return true;
	}
	else {
		return false;
	}
}


/**
 * Check for collision with bunkers and erode if needed
 * True if a collision occured
 */
bool control_checkBulletBunkerCollision(point_t bulletPoint, int8_t bullet_index) {
	uint8_t bunker_index;
	uint8_t block_index;
	// Check the y range
	if(bulletPoint.y < BUNKER_Y || bulletPoint.y >= BUNKER_Y + BUNKER_HEIGHT) {
		// If we aren't in the right y range, return
		return false;
	}
	else {
		uint8_t x_offset;
		uint8_t y_offset;

		// Check to see which bunker was hit, if any
		if (bulletPoint.x >= BUNKER_0_X && bulletPoint.x <= BUNKER_0_X + BUNKER_WIDTH) { // Check for bunker 0 collision
			x_offset = bulletPoint.x - BUNKER_0_X; // Get x offset from bunker position
			y_offset = bulletPoint.y - BUNKER_Y; // Get y offset from bunker position
			bunker_index = BUNKER_0;
		}
		else if (bulletPoint.x >= BUNKER_1_X && bulletPoint.x <= BUNKER_1_X + BUNKER_WIDTH) { // Check for bunker 1 collision
			x_offset = bulletPoint.x - BUNKER_1_X; // Get x offset from bunker position
			y_offset = bulletPoint.y - BUNKER_Y; // Get y offset from bunker position
			bunker_index = BUNKER_1;
		}
		else if (bulletPoint.x >= BUNKER_2_X && bulletPoint.x <= BUNKER_2_X + BUNKER_WIDTH) { // Check for bunker 2 collision
			x_offset = bulletPoint.x - BUNKER_2_X; // Get x offset from bunker position
			y_offset = bulletPoint.y - BUNKER_Y; // Get y offset from bunker position
			bunker_index = BUNKER_2;
		}
		else if (bulletPoint.x >= BUNKER_3_X && bulletPoint.x <= BUNKER_3_X + BUNKER_WIDTH) { // Check for bunker 3 collision
			x_offset = bulletPoint.x - BUNKER_3_X; // Get x offset from bunker position
			y_offset = bulletPoint.y - BUNKER_Y; // Get y offset from bunker position
			bunker_index = BUNKER_3;
		}
		else {
			return false; // Didn't hit a bunker
		}
		// Calculate which block was hit
		block_index = (x_offset/BUNKER_BLOCK_SIZE) + (y_offset/BUNKER_BLOCK_SIZE)*BUNKER_BLOCK_COLS;
		// Check to see if we are inside the arch (the "invisible blocks")
		if(global_getBlockState(bunker_index, block_index) == DEAD) {
			// If we are inside the arch of the bunker, return
			return false;
		}
		else {
			// Otherwise, erode the block
			global_erodeBunkerBlock(bunker_index, block_index);
			render_erodeBlock(bunker_index, block_index);


			// And set the bullet off screen
			if (bullet_index == TANK_BULLET_INDEX){
				global_collideTankBullet(); // Move the bullet off screen
			}
			else {
				global_collideAlienBullet(bullet_index); // Move the bullet off screen
			}
			return true;
		}

	}
}

/**
 * Handles collisions with tank bullets and the UFO
 */
void control_checkUFOCollision(point_t bulletPoint){
	if(bulletPoint.y >= UFO_Y && bulletPoint.y < UFO_Y + UFO_HEIGHT){ // Check y bounds
		if(bulletPoint.x >= global_getUFOPosition().x && bulletPoint.x < global_getUFOPosition().x + UFO_WIDTH){ // Check x bounds
			// If there is a collision, kill UFO and erase bullet
			global_killUFO();
			global_collideTankBullet();
		}
	}
}

/**
 * Update all the bullets
 */
void control_manageBullets() {
	// Move the tank bullet upward
	global_moveTankBullet(0, -BULLET_SPEED);
	point_t bulletPoint = global_getTankBulletPosition();
	// Check for collision with aliens and kill if needed
	control_checkTankBulletAlienCollision(bulletPoint);
	// Check for collision with bunker and erode if needed
	control_checkBulletBunkerCollision(bulletPoint, TANK_BULLET_INDEX);
	control_checkUFOCollision(bulletPoint);

	// Iterate through all alien bullets
	uint8_t i;
	for(i = 0; i < BULLET_COUNT; i++){
		// Move the alien bullet downward
		global_moveAlienBullet(i, 0, BULLET_SPEED);
		// Get it's new position (top of the bullet)
		point_t location = global_getAlienBulletPosition(i);
		// Get the location of the bottom of the bullet
		point_t bot_location = location;
		bot_location.y += BULLET_HEIGHT;
		uint8_t j;
		for(j = 0; j < ALIEN_BULLET_WIDTH; j++){ // Check for collisions with any part of the bullet
			// Check for bunker collisions with both the top and bottom of the bullet, then check for a collision with the tank
			if (control_checkBulletBunkerCollision(location, i) || control_checkBulletBunkerCollision(bot_location, i) || control_checkAlienBulletTankCollision(location, i)) {
				break; // If a collision is detected, no need to keep looping
			}
			location.x++;
		}
	}
}

/**
* Fire a random alien bullet at a random position
*/
void control_fireAlienBullet() {

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
