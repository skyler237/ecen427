/*
 * globals.c
 *
 *  Created on: Sep 16, 2016
 *      Author: superman
 */

// Demonstrates one way to handle globals safely in C.
#include "globals.h"
#include "render.h"
#include <stdio.h>


// Tank constants
#define TANK_MIN_X 2
#define TANK_MAX_X (SCREEN_WIDTH - TANK_WIDTH - 2)
#define TANK_SPEED 4

// Alien constants
#define ALIEN_BLOCK_INIT_X (SCREEN_WIDTH/2 - ALIEN_BLOCK_WIDTH/2)	// Alien block initial x position
#define ALIEN_BLOCK_INIT_Y (SCORE_BAR_HEIGHT + 10)	// Alien block initial y position
#define ALIEN_CENTER (ALIEN_WIDTH / 2)	// Center of an alien (used for firing bullets)
#define ALIEN_INIT_ROW 0x7FF // Initial value for the alienPositions array (indicates all aliens are alive)
#define ALIEN_ROW_MSB 0x400  // Most significant byte of the alien row (used for a mask)
#define KILL_ALIEN_MASK (0xFFFFFBFF) // Mask used to clear alien-alive bits

// Bullet constants
#define BULLET_TYPES 6	// Number of different bullet types/states
#define TANK_BULLET_X_OFFSET 7	// X-offset to make bullet fire from tank turret


// Status bar constants
#define SCORE_BAR_HEIGHT 15
#define LIVES_INIT_VALUE 3

// A block stores a position and an erosion state
typedef struct {erosionState_t erosion_state; point_t position;} block_t;

// A bunker stores a position and an array of blocks
typedef struct {block_t blocks[BUNKER_BLOCK_CNT]; point_t position;} bunker_t;

// Keeps track of all bullet information
typedef struct {
	point_t position; // The position of the bullet
	bulletType_t type; // The bullet image type and state
} bullet_t;


// Global variables
static point_t tankPosition; // Position of the tank
static point_t tankBulletPosition; // Position of the single tank bullet
static point_t alienBlockPosition; // Upper left corner of the full alien block
static uint16_t alienPositions[ALIEN_ROWS]; // low 11 bits of each word represent each alien in a row -- 1 = alive, 0 = dead
static bullet_t alienBullets[BULLET_COUNT]; // Array of 4 alien bullets
static bool alienPosIn; // Keeps track of whether the aliens are "in" or "out" -- 1 = in, 0 = out
static bunker_t bunkers[BUNKER_COUNT]; // Array of 4 bunkers
static uint8_t current_lives; // Lives left
static uint16_t current_score; // Accumulated score



/*
	Initializes the positions of each of the individual bunker blocks
*/
void init_bunker_blocks(uint8_t bunker_index){
	// Get the bunker base position (upper left)
	point_t bunker_pos = bunkers[bunker_index].position;

	// Loop through the different blocks
	uint8_t block_index;
	for(block_index = 0; block_index < BUNKER_BLOCK_CNT; block_index++){
		// Calculate the block offset based on the index
		bunkers[bunker_index].blocks[block_index].position.x = bunker_pos.x + BUNKER_BLOCK_SIZE*(block_index % BUNKER_BLOCK_COLS); // Get the x offset (col)
		bunkers[bunker_index].blocks[block_index].position.y = bunker_pos.y + BUNKER_BLOCK_SIZE*(block_index / BUNKER_BLOCK_COLS); // Get the y offset (row)

		// If the block index is 9 or 10, we just treat it as dead (the blocks under the arch)
		if (bunker_index == INVISIBLE_BLOCK_1 || bunker_index == INVISIBLE_BLOCK_2){
			// Initialize "invisible" blocks to DEAD
			bunkers[bunker_index].blocks[block_index].erosion_state = DEAD;
		}
		else {
			// Initialize other blocks to WHOLE
			bunkers[bunker_index].blocks[block_index].erosion_state = WHOLE;
		}
	}
}

/*
	Initializes all positions and other relevant game data
*/
void globals_init() {
	// Initialize Tank position
	tankPosition.x = TANK_INIT_X;
	tankPosition.y = TANK_INIT_Y;
	// Initialize Tank previous position
	tankPosition.prev_x = TANK_INIT_X; // Same as initial position
	tankPosition.prev_y = TANK_INIT_Y;

	// Initialize tank bullet position
	tankBulletPosition.y = OFF_SCREEN; // Start it "off scree"
	tankBulletPosition.x = OFF_SCREEN;

	// Initialize the alien block position
	alienBlockPosition.x = ALIEN_BLOCK_INIT_X;
	alienBlockPosition.y = ALIEN_BLOCK_INIT_Y;
	// Initialize the alien block previous position
	alienBlockPosition.prev_x = ALIEN_BLOCK_INIT_X; // Same as initial position
	alienBlockPosition.prev_y = ALIEN_BLOCK_INIT_Y;

	// Iterate through alien rows to mark all aliens as alive initially
	uint8_t i;
	for(i=0; i < ALIEN_ROWS; i++) {
		// Mark each row as having all aliens alive
		alienPositions[i] = ALIEN_INIT_ROW;
	}

	// Iterate through the alien bullets
	for (i = 0; i < BULLET_COUNT; i++){
		// Mark each bullet as "off screen"
		alienBullets[i].position.x = OFF_SCREEN;
		alienBullets[i].position.y = OFF_SCREEN;
	}

	// Initialize the bunker positions
	bunkers[BUNKER_0].position.x = BUNKER_0_X; // Initialize bunker 0
	bunkers[BUNKER_0].position.y = BUNKER_Y;
	bunkers[BUNKER_1].position.x = BUNKER_1_X; // Initialize bunker 1
	bunkers[BUNKER_1].position.y = BUNKER_Y;
	bunkers[BUNKER_2].position.x = BUNKER_2_X; // Initialize bunker 2
	bunkers[BUNKER_2].position.y = BUNKER_Y;
	bunkers[BUNKER_3].position.x = BUNKER_3_X; // Initialize bunker 3
	bunkers[BUNKER_3].position.y = BUNKER_Y;

	// Iterate through bunkers
	for(i=0; i < BUNKER_COUNT; i++) {
		// For each bunker, initialize the positions of each of its blocks
		init_bunker_blocks(i);
	}

	// Set the initial value of the lives
	current_lives = LIVES_INIT_VALUE;

}



// Global Functions and Accessors

/*
	Sets the tank position and updates previous position
*/
void global_setTankPositionGlobal(int16_t x, int16_t y) {
	// Update the previous position
	tankPosition.prev_x = tankPosition.x;
	tankPosition.prev_y = tankPosition.y;

	// Set the current position
	if(x - TANK_SPEED < TANK_MIN_X) {
		// Don't let the tank pass the left wall
		tankPosition.x = TANK_MIN_X;
	}
	else if(x + TANK_SPEED > TANK_MAX_X) {
		// Don't let the tank pass the right wall
		tankPosition.x = TANK_MAX_X;
	}
	else {
		// Otherwise, just move the tank
		tankPosition.x = x;
	}

	tankPosition.y = y;
}

/*
	Moves the tank by dx and dy
*/
void global_moveTank(int8_t dx, int8_t dy) {
	// Call the set tank position function, but add dx and dy
	global_setTankPositionGlobal(tankPosition.x + dx, tankPosition.y + dy);
}

/*
	Returns the tank position point
*/
point_t global_getTankPositionGlobal() {
	return tankPosition;
}

/*
	Sets the tank bullet position
*/
void global_setTankBulletPosition(uint16_t x, uint16_t y) {
	// Update the previous position
	tankBulletPosition.prev_x = tankBulletPosition.x;
	tankBulletPosition.prev_y = tankBulletPosition.y;

	// Set the current position
	tankBulletPosition.x = x;
	tankBulletPosition.y = y;
}

/*
	Moves the tank bullet by dx and dy
*/
void global_moveTankBullet(int8_t dx, int8_t dy){
	// Check the position of the bullet to see if it is going to hit the status bar
	if(tankBulletPosition.y+dy < STATUS_BAR_Y+TEXT_HEIGHT || tankBulletPosition.y == OFF_SCREEN){
		// If so, remove it from the screen
		global_setTankBulletPosition(OFF_SCREEN, OFF_SCREEN);
	}
	else { // If it's not at the top of the screen,
		// Update the position by moving it by dx and dy
		global_setTankBulletPosition(tankBulletPosition.x+dx, tankBulletPosition.y+dy);
	}
}

/*
	Fires a tank bullet
*/
void global_fireTankBullet() {
	// Check to see if the bullet is available
	if(tankBulletPosition.y == OFF_SCREEN){ // If it is not on the screen...
		// Set the bullet to appear just above the tank turret
		tankBulletPosition.x = tankPosition.x + TANK_BULLET_X_OFFSET;
		tankBulletPosition.y = tankPosition.y - BULLET_HEIGHT;
	}
}

/*
	Returns the tank bullet position
*/
point_t global_getTankBulletPosition() {
  return tankBulletPosition;
}

/*
	Sets the alien block position (upper left corner)
*/
void global_setAlienBlockPosition(uint16_t x, uint16_t y) {
	// Update the previous position
	alienBlockPosition.prev_x = alienBlockPosition.x;
	alienBlockPosition.prev_y = alienBlockPosition.y;

	// Set the current position
	alienBlockPosition.x = x;
	alienBlockPosition.y = y;
}

/*
	Moves the alien block position by dx and dy
*/
void global_moveAlienBlock(uint16_t dx, uint16_t dy){
	// Just increases the position by dx and dy
	// --NOTE: bounds checking is done by the controller
	global_setAlienBlockPosition(alienBlockPosition.x + dx, alienBlockPosition.y + dy);
}

/*
	Returns the alien block position
*/
point_t global_getAlienBlockPosition(){
	return alienBlockPosition;
}

/*
	Creates a new alien bullet, but only if one is available
	@param row, col: the row and column of the alien from which the bullet will fire
*/
void global_createAlienBullet(uint8_t row, uint8_t col){
	// Initializes a new alien bullet
	uint8_t i = 0;
	bool created = false;
	// Iterate through the bullets
	while (!created && i < BULLET_COUNT) { // Keep looping until we have created, or until we have tried all bullets
		if (alienBullets[i].position.y == OFF_SCREEN) { // Only fire a bullet if it is off the screen

			// Calculate the proper offset for the given alien row & col
			alienBullets[i].position.x = alienBlockPosition.x + col*ALIEN_X_SPACING + ALIEN_CENTER;
			alienBullets[i].position.y = alienBlockPosition.y + row*ALIEN_Y_SPACING + ALIEN_HEIGHT;

			// Set the bullet type to a pseudo-random value
			alienBullets[i].type = alienBullets[i].position.x % BULLET_TYPES;

			created = true; // Indicate that we have successfully created a bullet
		}
		i++;
	}
}

/*
	Updates an existing alien bullet's position
	@param index: the index of the bullet we want to update
*/
void global_updateAlienBullet(uint8_t index, uint16_t x, uint16_t y) {
	// Updates the previous position
	alienBullets[index].position.prev_x = alienBullets[index].position.x;
	alienBullets[index].position.prev_y = alienBullets[index].position.y;
	// Update the new position
	alienBullets[index].position.x = x;
	alienBullets[index].position.y = y;

	// Rotate through guises on each move of the bullet to make it animated
	switch(alienBullets[index].type){
			case CROSS_UP: // If we were in the CROSS_UP position,
				alienBullets[index].type = CROSS_MID2DOWN; // change to CROSS_MID2DOWN
				break;
			case CROSS_DOWN: // If we were in the CROSS_DOWN position,
				alienBullets[index].type = CROSS_MID2UP; // change to CROSS_MID2UP
				break;
			case CROSS_MID2UP: // If we were in the CROSS_MID2UP position,
				alienBullets[index].type = CROSS_UP; // change to CROSS_UP
				break;
			case CROSS_MID2DOWN: // If we were in the CROSS_MID2DOWN position,
				alienBullets[index].type = CROSS_DOWN; // change to CROSS_DOWN
				break;
			case LIGHTNING1: // If we were in the LIGHTNING1 position,
				alienBullets[index].type = LIGHTNING2; // change to LIGHTNING2
				break;
			case LIGHTNING2: // If we were in the LIGHTNING2 position,
				alienBullets[index].type = LIGHTNING1; // change to LIGHTNING1
				break;
			}
}

/*
	Moves an alien bullet by dx and dy
	@param index: the index of the bullet we want to move
*/
void global_moveAlienBullet(uint8_t index, int8_t dx, int8_t dy){
	// Check to see if we are at the bottom of the screen
	if(alienBullets[index].position.y+dy > BASE_LINE_Y-BULLET_HEIGHT){
		// If we are at the bottom, remove the bullet from the screen
		global_updateAlienBullet(index, OFF_SCREEN, OFF_SCREEN);
	}
	else { // If we aren't at the bottom...
		// Update the bullet position, moving by dx and dy
		global_updateAlienBullet(index, alienBullets[index].position.x + dx, alienBullets[index].position.y + dy);
	}
}

/*
	Return the type of an alien bullet
	@param index: the index of the bullet we want information about
*/
bulletType_t global_getAlienBulletType(uint8_t index){
	return alienBullets[index].type;
}
/*
	Return the position of an alien bullet
	@param index: the index of the bullet we want information about
*/
point_t global_getAlienBulletPosition(uint8_t index){
	return alienBullets[index].position;
}

/*
	Increases the erosion state of a given block
	@param bunker_index: indicates the bunker (0-4)
	@param block_index: indicates which block within the given bunker (0-11)
*/
void global_erodeBunkerBlock(uint8_t bunker_index, uint8_t block_index){
	// If the block isn't already dead,
	if(global_getBlockState(bunker_index, block_index) != DEAD) {
		// Increase the erosion state by one
		bunkers[bunker_index].blocks[block_index].erosion_state++;
	}
}

/*
	Initialize the block state of a given bunker block
	@param bunker_index: indicates the bunker (0-4)
	@param block_index: indicates which block within the given bunker (0-11)
*/
void global_initBlockState(uint8_t bunker_index, uint8_t block_index){
	// Initialize the state to be whole
	bunkers[bunker_index].blocks[block_index].erosion_state = WHOLE;
}

/*
	Returns the erosion state of the given block
	@param bunker_index: indicates the bunker (0-4)
	@param block_index: indicates which block within the given bunker (0-11)
*/
erosionState_t global_getBlockState(uint8_t bunker_index, uint8_t block_index){
	return bunkers[bunker_index].blocks[block_index].erosion_state;
}

/*
	Sets the block position of a given bunker block
	@param bunker_index: indicates the bunker (0-4)
	@param block_index: indicates which block within the given bunker (0-11)
*/
void global_setBlockPosition(uint8_t bunker_index, uint8_t block_index, uint8_t x, uint8_t y){
	bunkers[bunker_index].blocks[block_index].position.x = x;
	bunkers[bunker_index].blocks[block_index].position.y = y;
}

/*
	Returns the bunker block position of the given block
	@param bunker_index: indicates the bunker (0-4)
	@param block_index: indicates which block within the given bunker (0-11)
*/
point_t global_getBlockPosition(uint8_t bunker_index, uint8_t block_index){
	return bunkers[bunker_index].blocks[block_index].position;
}

/*
	Returns the position of a given bunker
	@param bunker_index: indicates the bunker (0-4)
*/
point_t global_getBunkerPosition(uint8_t bunker_index){
	return bunkers[bunker_index].position;
}

/*
	Sets a bunker position
	@param bunker_index: indicates the bunker (0-4)
*/
void global_setBunkerPosition(uint8_t bunker_index, uint8_t x, uint8_t y){
	bunkers[bunker_index].position.x = x;
	bunkers[bunker_index].position.y = y;
}

/*
	Kills a given alien
	-- NOTE: only sets the state to dead, but doesn't erase here
	@param row, col: the row and column within the alien block of the alien to kill
*/
void global_killAlien(uint8_t row, uint8_t col){
	// Clears the corresponding bit of the given alien
	alienPositions[row] = alienPositions[row] & (KILL_ALIEN_MASK >> col);

	//Draws the explosion sprite in place of the alien sprite
	render_killAlien(row, col);
}

/*
	Checks to see if the give alien is alive
	@param row, col: the row and column within the alien block of the alien to kill
*/
bool global_isAlienAlive(uint8_t row, uint8_t col) {
	// Extract the given row from the alien position array
	uint16_t alien_row = alienPositions[row];
	// Check to see if the bit in the given column is set (a.k.a. is alive)
	return alien_row & (ALIEN_ROW_MSB >> col);
}

/*
	Returns whether or not the alien guises are the "inward" or "outward" type
	@return alienPosIn: if true, alien guises are "inward"; if false, then they are "outward"
*/
bool global_isAlienPoseIn() {
	return alienPosIn;
}
/*
	Toggles the alien position between in and out
*/
void global_toggleAlienPose() {
	// Set the guise type to the complement of the current type
	alienPosIn = !alienPosIn;
}

/*
	Sets the current life count
*/
void global_setLives(uint8_t lives){
	current_lives = lives;
}
/*
	Returns the current life count
*/
uint8_t global_getLives(){
	return current_lives;
}

/*
	Sets the current score
*/
void global_setScore(uint16_t score){
	current_score = score;
}
/*
	Increments the current score by the given amount
	@param score: this is the number of points we will add to the current score
*/
void global_addToScore(uint16_t score){
	current_score += score;
}
/*
	Returns the current score
*/
uint16_t global_getScore(){
	return current_score;
}
