/*
 * control.c
 *
 *  Created on: Sep 26, 2016
 *      Author: superman
 */

#include <stdbool.h>
#include <stdint.h>
#include "globals.h"
#include "render.h"

#define SCREEN_RIGHT_EDGE 320
#define SCREEN_LEFT_EDGE 0

#define ALIEN_MOVE_X 4
#define ALIEN_MOVE_Y 6
#define EDGE_2_ALIEN_GAP 10
#define ALIEN_ROW_MAX 4
#define ALIEN_COL_MAX 10

#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define ALIEN_X_SPACING (ALIEN_WIDTH + 4)
#define ALIEN_Y_SPACING (ALIEN_HEIGHT + 6)

void control_updateAlienBlock(uint32_t* framePtr) {
	static bool isMovingRight = true;
	// Toggle the in/out position
	global_toggleAlienPose();

	// Check the bounds
	uint8_t row = 0;
	uint8_t col = 0;
	// Get left-most alive alien position
	uint8_t left_most_col = 0;
	while(!global_isAlienAlive(row, col)) {
		if(row >= ALIEN_ROW_MAX) {
			row = 0;
			col++;
		}
		else {
			row++;
		}
	}
	left_most_col = col;

	// Get right-most alive alien position
	row = 0;
	col = ALIEN_COL_MAX;
	uint8_t right_most_col;
	while(!global_isAlienAlive(row, col)) {
			if(row >= ALIEN_ROW_MAX) {
				row = 0;
				col--;
			}
			else {
				row++;
			}
		}
		right_most_col = col;

	// Make the appropriate move
	point_t blockPosition = global_getAlienBlockPosition();
	if(isMovingRight && (blockPosition.x + ALIEN_X_SPACING*right_most_col) >= SCREEN_RIGHT_EDGE) {
		// Move down
		global_moveAlienBlock(0, ALIEN_MOVE_Y);

		// Start moving left
		isMovingRight = false;
	}
	else if(!isMovingRight && (blockPosition.x + ALIEN_X_SPACING*left_most_col) <= SCREEN_LEFT_EDGE) {
		// Move down
		global_moveAlienBlock(0, ALIEN_MOVE_Y);

		// Start moving right
		isMovingRight = true;
	}
	else if(isMovingRight) {
		// Move right
		global_moveAlienBlock(ALIEN_MOVE_X, 0);
	}
	else {
		// Move right
		global_moveAlienBlock(-ALIEN_MOVE_X, 0);
	}

	// Kill bunker blocks if colliding

	// Render
	render_refreshAliens(framePtr);
}
