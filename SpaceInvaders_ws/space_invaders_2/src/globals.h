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

//===================================================
//				Global constants
//===================================================

// Screen constants
#define SCREEN_WIDTH 320	// Screen width in game pixels
#define SCREEN_HEIGHT 240	// Screen height in game pixels

// Alien constants
#define ALIEN_ROWS 5		// Number of alien rows
#define ALIEN_COLS 11		// Number of alien constants
#define ALIEN_ROW_MAX (ALIEN_ROWS - 1)	// Max row index (0 indexed)
#define ALIEN_COL_MAX (ALIEN_COLS - 1)	// Max col index (0 indexed)
#define ALIEN_WIDTH 12		// Alien sprite width
#define ALIEN_HEIGHT 8		// Alien sprite height
#define ALIEN_X_SPACING (ALIEN_WIDTH + 4)	// Difference in x from one alien position to the next
#define ALIEN_Y_SPACING (ALIEN_HEIGHT + 6)	// Difference in y from one alien position to the next

// Tank constants
#define TANK_WIDTH 15		// Tank sprite width
#define TANK_HEIGHT 8		// Tank sprite height

// Bunker constants
#define BUNKER_WIDTH 24		// Full bunker sprite width
#define BUNKER_HEIGHT 18	// Full bunker sprite height
#define BUNKER_COUNT 4		// Number of bunkers in the game
#define BUNKER_BLOCK_CNT 12 // Really only 10, but blocks 9 and 10 (0 indexed) are initialized to dead
#define BUNKER_BLOCK_SIZE 6	// Size of a bunker block (in pixels) - width = height = size

// Bullet constants
#define BULLET_HEIGHT 5		// Height of bullet sprite
#define BULLET_COUNT 4		// Number of alien bullets

// Status bar constants
#define STATUS_BAR_Y 10		// Y value of the status bar (top)
#define TEXT_HEIGHT 5		// General height of text for the status bar text sprites

#define BASE_LINE_Y (SCREEN_HEIGHT - 5)	// Y value of the base line



// A point will keep track of an x,y position and a previous x,y position
typedef struct {int16_t x; int16_t y; int16_t prev_x; int16_t prev_y;} point_t;

// Keeps track of the guise types for alien bullets (also accounts for state)
typedef enum { CROSS_UP, CROSS_DOWN, CROSS_MID2UP, CROSS_MID2DOWN, LIGHTNING1, LIGHTNING2 } bulletType_t;

// Keeps track of the erosion status of a bunker block
typedef enum  {WHOLE, HIT1, HIT2, HIT3, DEAD} erosionState_t;

// Initializes all positions and other relevant game data
void globals_init();

// Sets the tank position and updates previous position
void global_setTankPositionGlobal(uint16_t x, uint16_t y);
// Moves the tank by dx and dy
void global_moveTank(int8_t dx, int8_t dy);
// Returns the tank position point
point_t global_getTankPositionGlobal();

// Sets the tank bullet position
void global_setTankBulletPosition(uint16_t x, uint16_t y);
// Moves the tank bullet by dx and dy
void global_moveTankBullet(int8_t dx, int8_t dy);
// Fires a tank bullet
void global_fireTankBullet();
// Returns the tank bullet position
point_t global_getTankBulletPosition();

// Sets the alien block position (upper left corner)
void global_setAlienBlockPosition(uint16_t x, uint16_t y);
// Moves the alien block position by dx and dy
void global_moveAlienBlock(uint16_t dx, uint16_t dy);
// Returns the alien block position
point_t global_getAlienBlockPosition();

// Creates a new alien bullet, but only if one is available
void global_createAlienBullet(uint8_t row, uint8_t col);
// Updates an existing alien bullet's position
void global_moveAlienBullet(uint8_t index,int8_t dx, int8_t dy);
// Return the type of an alien bullet
bulletType_t global_getAlienBulletType(uint8_t index);
// Return the position of an alien bullet
point_t global_getAlienBulletPosition(uint8_t index);

// Increases the erosion state of a given block
void global_erodeBunkerBlock(uint8_t bunker_index, uint8_t block_index);
// Returns the erosion state of the given block 
erosionState_t global_getBlockState(uint8_t bunker_index, uint8_t block_index);
// Returns the bunker block position of the given block
point_t global_getBlockPosition(uint8_t bunker_index, uint8_t block_index);
// Returns the position of a given bunker
point_t global_getBunkerPosition(uint8_t bunker_index);
// Sets a bunker position
void global_setBunkerPosition(uint8_t bunker_index, uint8_t x, uint8_t y);

// Kills a given alien
void global_killAlien(uint8_t row, uint8_t col);
// Checks to see if the give alien is alive
bool global_isAlienAlive(uint8_t row, uint8_t col);
// Returns whether or not the alien guises are the "inward" or "outward" type
bool global_isAlienPoseIn();
// Toggles the alien position between in and out
void global_toggleAlienPose();

// Sets the current life count
void global_setLives(uint8_t lives);
// Returns the current life count
uint8_t global_getLives();

// Sets the current score
void global_setScore(uint16_t score);
// Increments the current score by the given amount
void global_addToScore(uint16_t score);
// Returns the current score
uint16_t global_getScore();


#endif /* GLOBALS_H_ */
