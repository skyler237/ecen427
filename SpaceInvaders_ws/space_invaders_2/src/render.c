/*
 * render.c
 *
 *  Created on: Sep 21, 2016
 *      Author: superman
 */

#include "globals.h"
#include "render.h"
#include <stdio.h>

//================================================================================================================
// 								Bitmap Declarations
//================================================================================================================

// Packs each horizontal line of the figures into a single 32 bit word.
#define packword32(b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b31 << 31) | (b30 << 30) | (b29 << 29) | (b28 << 28) | (b27 << 27) | (b26 << 26) | (b25 << 25) | (b24 << 24) |						  \
 (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 16 bit word.
#define packword16(b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 12 bit word.
#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | \
 (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 15 bit word.
#define packword15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 24 bit word.
#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 15 bit word.
#define packword6(b5,b4,b3,b2,b1,b0) \
((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define packword3(b2,b1,b0) \
((b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Must define packword for each of the different bit-widths.
static const  uint32_t saucer_16x7[] =
{
packword16(0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0),
packword16(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0),
packword16(0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword16(0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0),
packword16(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword16(0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0),
packword16(0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0)
};

static const  uint32_t alien_explosion_12x10[] =
{
packword12(0,0,0,0,0,0,1,0,0,0,0,0),
packword12(0,0,0,1,0,0,1,0,0,0,1,0),
packword12(1,0,0,1,0,0,0,0,0,1,0,0),
packword12(0,1,0,0,1,0,0,0,1,0,0,0),
packword12(0,0,0,0,0,0,0,0,0,0,1,1),
packword12(1,1,0,0,0,0,0,0,0,0,0,0),
packword12(0,0,0,1,0,0,0,1,0,0,1,0),
packword12(0,0,1,0,0,0,0,0,1,0,0,1),
packword12(0,1,0,0,0,1,0,0,1,0,0,0),
packword12(0,0,0,0,0,1,0,0,0,0,0,0)
};

static const  uint32_t alien_top_in_12x8[] =
{
packword12(0,0,0,0,0,1,1,0,0,0,0,0),
packword12(0,0,0,0,1,1,1,1,0,0,0,0),
packword12(0,0,0,1,1,1,1,1,1,0,0,0),
packword12(0,0,1,1,0,1,1,0,1,1,0,0),
packword12(0,0,1,1,1,1,1,1,1,1,0,0),
packword12(0,0,0,1,0,1,1,0,1,0,0,0),
packword12(0,0,1,0,0,0,0,0,0,1,0,0),
packword12(0,0,0,1,0,0,0,0,1,0,0,0)
};

static const  uint32_t alien_top_out_12x8[] =
{
packword12(0,0,0,0,0,1,1,0,0,0,0,0),
packword12(0,0,0,0,1,1,1,1,0,0,0,0),
packword12(0,0,0,1,1,1,1,1,1,0,0,0),
packword12(0,0,1,1,0,1,1,0,1,1,0,0),
packword12(0,0,1,1,1,1,1,1,1,1,0,0),
packword12(0,0,0,0,1,0,0,1,0,0,0,0),
packword12(0,0,0,1,0,1,1,0,1,0,0,0),
packword12(0,0,1,0,1,0,0,1,0,1,0,0)
};

static const  uint32_t alien_middle_in_12x8[] =
{
packword12(0,0,0,1,0,0,0,0,0,1,0,0),
packword12(0,0,0,0,1,0,0,0,1,0,0,0),
packword12(0,0,0,1,1,1,1,1,1,1,0,0),
packword12(0,0,1,1,0,1,1,1,0,1,1,0),
packword12(0,1,1,1,1,1,1,1,1,1,1,1),
packword12(0,1,1,1,1,1,1,1,1,1,1,1),
packword12(0,1,0,1,0,0,0,0,0,1,0,1),
packword12(0,0,0,0,1,1,0,1,1,0,0,0)
};

static const  uint32_t alien_middle_out_12x8[] =
{
packword12(0,0,0,1,0,0,0,0,0,1,0,0),
packword12(0,1,0,0,1,0,0,0,1,0,0,1),
packword12(0,1,0,1,1,1,1,1,1,1,0,1),
packword12(0,1,1,1,0,1,1,1,0,1,1,1),
packword12(0,1,1,1,1,1,1,1,1,1,1,1),
packword12(0,0,1,1,1,1,1,1,1,1,1,0),
packword12(0,0,0,1,0,0,0,0,0,1,0,0),
packword12(0,0,1,0,0,0,0,0,0,0,1,0)
};

static const  uint32_t alien_bottom_in_12x8[] =
{
packword12(0,0,0,0,1,1,1,1,0,0,0,0),
packword12(0,1,1,1,1,1,1,1,1,1,1,0),
packword12(1,1,1,1,1,1,1,1,1,1,1,1),
packword12(1,1,1,0,0,1,1,0,0,1,1,1),
packword12(1,1,1,1,1,1,1,1,1,1,1,1),
packword12(0,0,1,1,1,0,0,1,1,1,0,0),
packword12(0,1,1,0,0,1,1,0,0,1,1,0),
packword12(0,0,1,1,0,0,0,0,1,1,0,0)
};

static const  uint32_t alien_bottom_out_12x8[] =
{
packword12(0,0,0,0,1,1,1,1,0,0,0,0),
packword12(0,1,1,1,1,1,1,1,1,1,1,0),
packword12(1,1,1,1,1,1,1,1,1,1,1,1),
packword12(1,1,1,0,0,1,1,0,0,1,1,1),
packword12(1,1,1,1,1,1,1,1,1,1,1,1),
packword12(0,0,0,1,1,0,0,1,1,0,0,0),
packword12(0,0,1,1,0,1,1,0,1,1,0,0),
packword12(1,1,0,0,0,0,0,0,0,0,1,1)
};

static const  uint32_t tank_15x8[] =
{
packword15(0,0,0,0,0,0,0,1,0,0,0,0,0,0,0),
packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
packword15(0,0,0,0,0,0,1,1,1,0,0,0,0,0,0),
packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword15(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)
};


// Shape of the entire bunker.
static const  uint32_t bunker_24x18[] =
{
packword24(0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0),
packword24(0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0),
packword24(0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1),
packword24(1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1)
};

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
static const  uint32_t bunkerDead_6x6[] = {
packword6(0,0,0,0,0,0),
packword6(0,0,0,0,0,0),
packword6(0,0,0,0,0,0),
packword6(0,0,0,0,0,0),
packword6(0,0,0,0,0,0),
packword6(0,0,0,0,0,0)
};

static const  uint32_t bunkerDamage0_6x6[] = {
packword6(0,1,1,0,0,0),
packword6(0,0,0,0,0,1),
packword6(1,1,0,1,0,0),
packword6(1,0,0,0,0,0),
packword6(0,0,1,1,0,0),
packword6(0,0,0,0,1,0)
};

static const  uint32_t bunkerDamage1_6x6[] = {
packword6(1,1,1,0,1,0),
packword6(1,0,1,0,0,1),
packword6(1,1,0,1,1,1),
packword6(1,0,0,0,0,0),
packword6(0,1,1,1,0,1),
packword6(0,1,1,0,1,0)
};

static const  uint32_t bunkerDamage2_6x6[] = {
packword6(1,1,1,1,1,1),
packword6(1,0,1,1,0,1),
packword6(1,1,0,1,1,1),
packword6(1,1,0,1,1,0),
packword6(0,1,1,1,0,1),
packword6(1,1,1,1,1,1)
};

static const  uint32_t bunkerDamage3_6x6[] = {
packword6(1,1,1,1,1,1),
packword6(1,1,1,1,1,1),
packword6(1,1,1,1,1,1),
packword6(1,1,1,1,1,1),
packword6(1,1,1,1,1,1),
packword6(1,1,1,1,1,1)
};

static const uint32_t livesText_24x5[] =
{
		packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,1),
		packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0),
		packword24(1,0,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,0,0,0,1,1,1,0),
		packword24(1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1),
		packword24(1,1,1,1,0,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,1,1,1,0)
};

static const uint32_t scoreText_32x5[] =
{
		packword32(0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,1,1),
		packword32(0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0),
		packword32(0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0),
		packword32(0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0),
		packword32(0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1)
};

static const uint32_t tankBullet_1x5[] =
{
		1,
		1,
		1,
		1,
		1
};

static const uint32_t bulletCrossUp_3x5[] =
{
		packword3(0,1,0),
		packword3(1,1,1),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0)
};

static const uint32_t bulletCrossMid_3x5[] =
{
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(1,1,1),
		packword3(0,1,0),
		packword3(0,1,0)
};

static const uint32_t bulletCrossDown_3x5[] =
{
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(0,1,0),
		packword3(1,1,1),
		packword3(0,1,0)
};

static const uint32_t bulletLightningOne_3x5[] =
{
		packword3(0,1,0),
		packword3(1,0,0),
		packword3(0,1,0),
		packword3(0,0,1),
		packword3(0,1,0)
};

static const uint32_t bulletLightningTwo_3x5[] =
{
		packword3(0,1,0),
		packword3(0,0,1),
		packword3(0,1,0),
		packword3(1,0,0),
		packword3(0,1,0)
};

#define FRAME_BUFFER_0_ADDR 0xC1000000
#define EXPLOSION_DEFAULT -1

static point_t explosion_location;
static uint32_t * framePtr = (uint32_t *) FRAME_BUFFER_0_ADDR;;

// Screen constants
#define SCALING_CONST 2 // Scaling constant between game pixels and actual screen density
#define LINE_WIDTH 640	// Number of screen pixels in one line
#define NEXT_COL 1		// Denotes the next column over
#define BACKGROUND_COLOR 0x00000000	// Background color for the screen

#define TANK_MSB 0x8000			// Most significant bit of the tank sprite rows
#define TANK_COLOR 0x0000FF00	// Tank color

#define BUNKER_COLOR 0x0000FF00 // Bunker color

#define TOP_ALIEN_ROW 0			// Top alien row index
#define MID_ALIEN_ROW 1			// Middle two rows starting index
#define BOTTOM_ALIEN_ROW 3		// Bottow two rows starting index
#define ALIEN_COLOR 0xFFFFFFFF	// Alien color
#define ALIEN_EXPLOSION_HEIGHT 10 //Height in pixels of the explosion sprite

#define TEXT_COLOR 0xFFFFFFFF	// General text color
#define SCORE_TEXT_X 15			// Score text sprite x offset
#define SCORE_TEXT_WIDTH 32		// Score text sprite width
#define SCORE_TEXT_HEIGHT TEXT_HEIGHT	// Score text sprite height
#define LIVES_TEXT_X (SCREEN_WIDTH*2/3)	// Lives text x offset
#define LIVES_TEXT_WIDTH 24		// Lives text sprite width
#define LIVES_TEXT_HEIGHT TEXT_HEIGHT	// Lives text sprite height
#define TEXT_2_LIFE_SPACING 10	// Spacing from lives text to the first life sprite (tank)
#define LIFE_2_LIFE_SPACING 5	// Spacing between life sprites (tanks)
#define LIFE_TANK_OFFSET (TANK_HEIGHT - LIVES_TEXT_HEIGHT)	// Offset to help align life sprites and lives text

#define BASE_LINE_COLOR 0x0000FF00	// Base line color

#define BULLET_COLOR 0xFFFFFFFF		// Bullet color
#define TANK_BULLET_WIDTH 1			// Tank bullet sprite height
#define ALIEN_BULLET_WIDTH 3		// Width of alien bullet sprites
//=======================================================================================
// 							Private Helper Functions
//=======================================================================================

/**
 * Writes a particular color to a given pixel
 * Also provides an abstraction from the actual resolution (640x480) to our game resolution (320x240)
 * @param row, col: the row and column in GAME PIXELS - NOT SCREEN PIXELS (e.g. 320,240 would be bottom right 2x2 block)
 */
void writePixel(uint16_t row, uint16_t col, uint32_t color) {
	// Write a 2x2 pixel block
	framePtr[row*SCALING_CONST*LINE_WIDTH + col*SCALING_CONST] = color; //Top left pixel
	framePtr[row*SCALING_CONST*LINE_WIDTH + (col*SCALING_CONST + NEXT_COL)] = color;	// Top right pixel
	framePtr[(row*SCALING_CONST*LINE_WIDTH + LINE_WIDTH) + col*SCALING_CONST] = color; // Bottom left pixel
	framePtr[(row*SCALING_CONST*LINE_WIDTH + LINE_WIDTH) + (col*SCALING_CONST + NEXT_COL)] = color; // Bottom right pixel
}

/**
 * Iterates through the entire screen, making all the pixels black
 */
void render_blankScreen() {
	uint16_t row, col;
	for(row=0; row < SCREEN_HEIGHT; row++) { // Iterate through rows
		for(col=0; col < SCREEN_WIDTH; col++) { // Iterate through columns
			writePixel(row, col, BACKGROUND_COLOR); // Write black to the whole screen
		}
	}
}

/**
 * Generic draw sprite function
 * @param spriteArray: a pointer to the sprite bitmap
 */
void drawSprite(const uint32_t* spriteArray, point_t sprite_pos, uint8_t spriteWidth, uint8_t spriteHeight, uint32_t sprite_color) {
	// Iterate through all rows of the sprite
	uint8_t row, col;
	for(row=0; row < spriteHeight; row++) {
		// Extract a row from the bitmap
		uint32_t sprite_row = spriteArray[row];
		// Iterate through each column
		for(col=0; col < spriteWidth; col++) {
			// Calculate the most significant bit of (based on sprite width)
			uint32_t msb = 0x01 << (spriteWidth - 1);
			// Check if the particular column is a 1
			if(sprite_row & (msb >> col)) {
				// If there is a 1, draw a pixel
				writePixel(row+sprite_pos.y, col+sprite_pos.x, sprite_color);
			}
		}
	}
}

/**
* Generic refresh sprite function
* -- Only draws and erases pixels which have changed!
* @param spriteArray: the sprite bitmap to be drawn
* @param oldSpriteArray: the bitmap of the previous guise (to erase)
*/
void refreshSprite(const uint32_t* spriteArray, const uint32_t* oldSpriteArray, point_t current_pos,
								uint8_t spriteWidth, uint8_t spriteHeight, uint32_t sprite_color) {

	uint32_t current_row; // Holds the row we want to leave draw
	uint32_t old_row;	// Holds the old row
	uint32_t to_draw;	// Holds the new bits to be drawn
	uint32_t to_erase;	// Holds the old bits to erase
 	point_t index_pos;	// Holds the upper left position from which we will calculate offsets

	uint16_t row, col;
	int8_t y_diff = current_pos.y - current_pos.prev_y; // Calculate the y offset
	uint8_t abs_y_diff = y_diff;
	// Calculate the absolute value of the y difference
	if (y_diff < 0) {
		// If it's negative, switch it to positive
		abs_y_diff = -y_diff;
	}

	// Iterate over all the rows of the sprite
	for(row=0; row < spriteHeight + abs_y_diff; row++) {
		// Calculate the change in x between old and new sprites
		int8_t diff = current_pos.x - current_pos.prev_x;

		// Handle case that there is a change in y
		if (y_diff != 0) {
			if (y_diff > 0) { // If the move was downward (dy > 0)...

				// Extract the old row
				if(row < spriteHeight) {
					// As long as we aren't past the last old row, just get the row
					old_row = oldSpriteArray[row];
				}
				else { // otherwise, we are past the old position, so we set old row to zero
					old_row = 0;
				}

				// Extract the new/current row
				if(row - y_diff >= 0) { // Check to see if we are above the new sprite
					// If we are far enough down, just extract the offset row
					current_row = spriteArray[row - y_diff];
				}
				else { // Otherwise, we are above the new sprite
					current_row = 0; // Set that row to be empty
				}
				// We want to index off of the previous sprite's position
				index_pos.x = current_pos.prev_x;
				index_pos.y = current_pos.prev_y;
			}
			else { // If the move was upward (dy < 0)...

				// Extract the current row
				if(row < spriteHeight) { // If we are still within the bounds of the new sprite
					current_row = spriteArray[row]; // just extract the row
				}
				else { // Otherwise, we are past the new sprite
					current_row = 0; // Set the current row to be empty
				}

				// Extract the old row
				if(row - y_diff >= 0) { // If we aren't above the old sprite
					old_row = oldSpriteArray[row - y_diff]; // Just extract the offset row
				}
				else { // We are above the old sprite
					old_row = 0; // Set the old row to be empty
				}

				// We want to index off of the new sprite position
				index_pos = current_pos;
			}
		}
		// If we aren't moving the y direction, check the x
		else if(diff >= 0) {  // If we are moving right (dx > 0)
			current_row = spriteArray[row]; // Extract the current row
			old_row = (oldSpriteArray[row] << diff); // Extract a left-shifted version of the old sprite
			// We want to index off of the old sprite position
			index_pos.x = current_pos.prev_x;
			index_pos.y = current_pos.prev_y;
		}
		else { // If we are moving left (dx < 0)
			diff = -diff; // Get the absolute value of diff
			old_row = oldSpriteArray[row]; // Extract the old row
			current_row = (spriteArray[row] << diff); // Extract a left-shifted version of the new sprite
			// We want to index off of the new sprite position
			index_pos = current_pos;
		}
		// We want to draw everything that is in the new row and not in the old row
		to_draw = current_row & ~old_row;
		// We want to erase everything that is in the old row and not in the new row
		to_erase = old_row & ~current_row;

		// Iterate through each column
		for(col=0; col < (spriteWidth + diff); col++) {
			// Get the most significant bit (to use as a mask)
			uint32_t msb = 0x01 << (spriteWidth + diff - 1);

			// If the column has a bit set in "to_draw" then draw a pixel there
			if(to_draw & (msb >> col)) {
				// Call wirte pixel function with sprite color
				writePixel(row+index_pos.y, col+index_pos.x, sprite_color);
			}
			// If the column has a bit set in "to_erase" then draw the background
			else if(to_erase & (msb >> col)) {
				// Call wirte pixel function with background color
				writePixel(row+index_pos.y, col+index_pos.x, BACKGROUND_COLOR);
			}
		}
	}

}

/**
* Draws the tank sprite (only used on to initialize)
*/
void drawTank() {
	// Get the current position
	point_t tank_pos = global_getTankPositionGlobal();
	// Call the draw sprite function
	drawSprite(tank_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
}

/**
* Refreshes the tank sprite
*/
void render_refreshTank() {
	// Get the current position
	point_t tank_pos = global_getTankPositionGlobal();
	// Call the refresh sprite function
	refreshSprite(tank_15x8, tank_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
}

/**
* Draws the bunkers (only used at init)
*/
void drawBunkers() {
	// Iterate over bunkers
	uint8_t bunker;
	for(bunker = 0; bunker < BUNKER_COUNT; bunker++) {
		// Get bunker position
		point_t bunker_pos = global_getBunkerPosition(bunker);
		// Draw the full bunker sprite
		drawSprite(bunker_24x18, bunker_pos, BUNKER_WIDTH, BUNKER_HEIGHT, BUNKER_COLOR);
	}
}

/**
* Updates a block to reflect an increased erosion state
* -- NOTE: this does not actually changed the erosion state of any blocks
*/
void render_erodeBlock(uint8_t bunker, uint8_t block_index) {
	// Get the block position
	point_t block_pos = global_getBlockPosition(bunker, block_index);
	// Get the current erosion state
	erosionState_t block_state = global_getBlockState(bunker, block_index);

	// Use a switch to determine the appropriate bitmap
	const uint32_t* block_bitmap;
	switch(block_state) {
	case WHOLE: // Whole bunker
			block_bitmap = bunkerDamage3_6x6;
		break;

	case HIT1: // Bunker bitmap with one hit
			block_bitmap = bunkerDamage2_6x6;
		break;

	case HIT2: // Bunker bitmap with two hits
			block_bitmap = bunkerDamage1_6x6;
		break;

	case HIT3: // Bunker bitmap with three hits
			block_bitmap = bunkerDamage0_6x6;
		break;

	case DEAD: // Bunker bitmap with four hits -- now dead (blank)
			block_bitmap = bunkerDead_6x6;
	}

	// Iterate over all rows of the block
	uint8_t row, col;
	for(row=0; row < BUNKER_BLOCK_SIZE; row++) {
		// Negate the bitmap, so we know where to erase rather than draw
		uint32_t to_erase = ~block_bitmap[row];
		// Iterate over columns
		for(col = 0; col < BUNKER_BLOCK_SIZE; col++) {
			// Calculate most significant bit (to use as mask)
			uint32_t msb = 0x01 << (BUNKER_BLOCK_SIZE - 1);

			// If the selected row should be erased...
			if(to_erase & (msb >> col)) {
				// Write the background color to that pixel
				writePixel(row+block_pos.y, col+block_pos.x, BACKGROUND_COLOR);
			}
		}
	}
}

/**
* Refreshes the bullet sprites
*/
void render_bullets() {
	// Get the tank bullet position
	point_t tank_bullet = global_getTankBulletPosition();
	// Get the previous position
	point_t tank_bullet_prev;
	tank_bullet_prev.x = tank_bullet.prev_x;
	tank_bullet_prev.y = tank_bullet.prev_y;

	// Erase the previous bullet
	drawSprite(tankBullet_1x5, tank_bullet_prev, TANK_BULLET_WIDTH, BULLET_HEIGHT, BACKGROUND_COLOR);
	// Draw the new one
	drawSprite(tankBullet_1x5, tank_bullet, TANK_BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLOR);

	// Loop through the alien bullets
	uint8_t i;
	for(i = 0; i < BULLET_COUNT; i++){
		const uint32_t* bitmap; // Stores current bullet bitmap
		const uint32_t* old_bitmap; // Stores the old bullet bitmap

		// Get the current position of the bullet
		point_t bullet_loc = global_getAlienBulletPosition(i);
		// Get the old position of the bullet
		point_t loc_old;
		loc_old.x = bullet_loc.prev_x;
		loc_old.y = bullet_loc.prev_y;
		// Get the bullet type
		bulletType_t type = global_getAlienBulletType(i);

		// Depending on the type, determine both the current and previous bitmaps
		switch(type){
		case CROSS_UP:			// For the CROSS_UP position,
			bitmap = bulletCrossUp_3x5;			// The current sprite is CROSS_UP
			old_bitmap = bulletCrossMid_3x5;	// The previous sprite was CROSS_MID
			break;
		case CROSS_DOWN:		// For the CROSS_DOWN position,
			bitmap = bulletCrossDown_3x5;		// The current sprite is CROSS_DOWN
			old_bitmap = bulletCrossMid_3x5;	// The previous sprite was CROSS_MID
			break;
		case CROSS_MID2UP:		// For the CROSS_MID2UP position,
			bitmap = bulletCrossMid_3x5;		// The current sprite is CROSS_MID2UP
			old_bitmap = bulletCrossDown_3x5;	// The previous sprite was CROSS_DOWN
			break;
		case CROSS_MID2DOWN:	// For the CROSS_MID2DOWN position,
			bitmap = bulletCrossMid_3x5;		// The current sprite is CROSS_MID2DOWN
			old_bitmap = bulletCrossUp_3x5;		// The previous sprite was CROSS_UP
			break;
		case LIGHTNING1:		// For the LIGHTNING1 position,
			bitmap = bulletLightningOne_3x5;		// The current sprite is LIGHTNING1
			old_bitmap = bulletLightningTwo_3x5;	// The previous sprite was LIGHTNING2
			break;
		case LIGHTNING2:		// For the LIGHTNING2 position,
			bitmap = bulletLightningTwo_3x5;		// The current sprite is LIGHTNING2
			old_bitmap = bulletLightningOne_3x5;	// The previous sprite was LIGHTNING1
			break;
		}

		// Erase the old sprite
		drawSprite(old_bitmap, loc_old, ALIEN_BULLET_WIDTH, BULLET_HEIGHT, BACKGROUND_COLOR);
		// Draw the new one!
		drawSprite(bitmap, bullet_loc, ALIEN_BULLET_WIDTH, BULLET_HEIGHT, BULLET_COLOR);
	}
}

/**
* Draws all the aliens which are still alive -- only called at init
*/
void drawAliens() {
	uint8_t row, col;
	const uint32_t* alien_bitmap;
	// Get the alien block position (upper left)
	point_t alien_block_pos = global_getAlienBlockPosition();
	for(row=0; row < ALIEN_ROWS; row++) { // Iterate through the 5 rows
		for(col=0; col < ALIEN_COLS; col++) { // Iterate through each of the 11 aliens on a row
			if(global_isAlienAlive(row,col)) { // Check to see if the alien is still alive
				if(row == TOP_ALIEN_ROW) { // If we are drawing the top row...
					// Check if the position is in or out and assign the appropriate bitmap
					alien_bitmap = (global_isAlienPoseIn() ? alien_top_in_12x8 : alien_top_out_12x8);
				}
				else if(row >= MID_ALIEN_ROW && row < BOTTOM_ALIEN_ROW) { // If we are in the middle two rows...
					// Check if the position is in or out and assign the appropriate bitmap
					alien_bitmap = (global_isAlienPoseIn() ? alien_middle_in_12x8 : alien_middle_out_12x8);
				}
				else { // If we are in the bottom two rows
					// Check if the position is in or out and assign the appropriate bitmap
					alien_bitmap = (global_isAlienPoseIn() ? alien_bottom_in_12x8 : alien_bottom_out_12x8);
				}

				// Calculate the particular alien position
				point_t alien_pos;
				alien_pos.x = alien_block_pos.x + (col*ALIEN_X_SPACING); // Index off of the alien block position
				alien_pos.y = alien_block_pos.y + (row*ALIEN_Y_SPACING); // Index off of the alien block position

				// Draw the apporpriate sprite
				drawSprite(alien_bitmap, alien_pos, ALIEN_WIDTH, ALIEN_HEIGHT, ALIEN_COLOR);
			}
		}
	}
}

/**
* Refreshes all the alien sprites
*/
void render_refreshAliens() {
	uint8_t row, col;
	const uint32_t* alien_bitmap; // Stores the new alien bitmap
	const uint32_t* old_alien_bitmap; // Stores the old alien bitmap
	point_t alien_block_pos = global_getAlienBlockPosition();
	for(row=0; row < ALIEN_ROWS; row++) {		// Iterate through the 5 rows
		for(col=0; col < ALIEN_COLS; col++) {	// Iterate through each of the 11 aliens on a row
			if(global_isAlienAlive(row,col)) {	// Check to see if the alien is still alive
				if(row == TOP_ALIEN_ROW) {		// If we are drawing the top row...
					// Check if the position is in or out and assign the appropriate bitmap
					alien_bitmap = (global_isAlienPoseIn() ? alien_top_in_12x8 : alien_top_out_12x8);
					old_alien_bitmap = (global_isAlienPoseIn() ? alien_top_out_12x8 : alien_top_in_12x8);
				}
				else if(row >= MID_ALIEN_ROW && row < BOTTOM_ALIEN_ROW) { // If we are in the middle two rows...
					// Check if the position is in or out and assign the appropriate bitmap
					alien_bitmap = (global_isAlienPoseIn() ? alien_middle_in_12x8 : alien_middle_out_12x8);
					old_alien_bitmap = (global_isAlienPoseIn() ? alien_middle_out_12x8 : alien_middle_in_12x8);
				}
				else { // If we are in the bottom two rows
					// Check if the position is in or out and assign the appropriate bitmap
					alien_bitmap = (global_isAlienPoseIn() ? alien_bottom_in_12x8 : alien_bottom_out_12x8);
					old_alien_bitmap = (global_isAlienPoseIn() ? alien_bottom_out_12x8 : alien_bottom_in_12x8);
				}

				// Calculate the particular alien position
				point_t alien_pos;
				alien_pos.x = alien_block_pos.x + (col*ALIEN_X_SPACING); // Index off of the alien block position
				alien_pos.y = alien_block_pos.y + (row*ALIEN_Y_SPACING); // Index off of the alien block position
				alien_pos.prev_x = alien_block_pos.prev_x + (col*ALIEN_X_SPACING); // Index off of the alien block position
				alien_pos.prev_y = alien_block_pos.prev_y + (row*ALIEN_Y_SPACING); // Index off of the alien block position

				// Call the refresh sprite function for this alien
				refreshSprite(alien_bitmap, old_alien_bitmap, alien_pos, ALIEN_WIDTH, ALIEN_HEIGHT, ALIEN_COLOR);
			}
		}
	}
}

//Draws the alien explosion
void render_killAlien(uint8_t row, uint8_t col){
	const uint32_t* alien_bitmap;

	// Get the alien block position
	point_t alien_block_pos = global_getAlienBlockPosition();
	if(row == TOP_ALIEN_ROW) { // If we are on the top row...
		// Check if the position is in or out and assign the appropriate bitmap
		alien_bitmap = (global_isAlienPoseIn() ? alien_top_in_12x8 : alien_top_out_12x8);
	}
	else if(row >= MID_ALIEN_ROW && row < BOTTOM_ALIEN_ROW) { // If we are in the middle two rows...
		// Check if the position is in or out and assign the appropriate bitmap
		alien_bitmap = (global_isAlienPoseIn() ? alien_middle_in_12x8 : alien_middle_out_12x8);
	}
	else { // If we are in the bottom two rows
		// Check if the position is in or out and assign the appropriate bitmap
		alien_bitmap = (global_isAlienPoseIn() ? alien_bottom_in_12x8 : alien_bottom_out_12x8);
	}

	point_t alien_pos;
	alien_pos.x = alien_block_pos.x + (col*ALIEN_X_SPACING); // Index off of the alien block position
	alien_pos.y = alien_block_pos.y + (row*ALIEN_Y_SPACING); // Index off of the alien block position

	// Draw over the alien with the background color (erase it)
	drawSprite(alien_bitmap, alien_pos, ALIEN_WIDTH, ALIEN_HEIGHT, BACKGROUND_COLOR);

	// Draw over the alien with the background color (erase it)
	drawSprite(alien_explosion_12x10, alien_pos, ALIEN_WIDTH, ALIEN_EXPLOSION_HEIGHT, ALIEN_COLOR);
	explosion_location.x = alien_pos.x;
	explosion_location.y = alien_pos.y;
}

/**
* Erases a given alien
*/
void render_eraseAlien(uint8_t row, uint8_t col) {
	drawSprite(alien_explosion_12x10, explosion_location, ALIEN_WIDTH, ALIEN_EXPLOSION_HEIGHT, BACKGROUND_COLOR);
}

/**
* Draws the status bar
*/
void drawStatusBar() {
	// Set the score position
	point_t score_pos;
	score_pos.x = SCORE_TEXT_X;
	score_pos.y = STATUS_BAR_Y;
	// Draw the score text sprite
	drawSprite(scoreText_32x5, score_pos, SCORE_TEXT_WIDTH, SCORE_TEXT_HEIGHT, TEXT_COLOR);


	// Set the lives text position
	point_t lives_pos;
	lives_pos.x = LIVES_TEXT_X;
	lives_pos.y = STATUS_BAR_Y;
	// Draw the lives text sprite
	drawSprite(livesText_24x5, lives_pos, LIVES_TEXT_WIDTH, LIVES_TEXT_HEIGHT, TEXT_COLOR);

	// Get the current score
	uint16_t score = global_getScore();
	// TODO: Draw the score here...

	// Get the number of lives
	uint8_t lives = global_getLives();
	// For each remaining life...
	uint8_t i;
	for(i = 0; i < lives; i++) {
		// Calculate the appropriate position
		point_t life_pos;
		life_pos.x = (LIVES_TEXT_X + LIVES_TEXT_WIDTH + TEXT_2_LIFE_SPACING + (LIFE_2_LIFE_SPACING + TANK_WIDTH)*i); // Offset from the text
		life_pos.y = (STATUS_BAR_Y - LIFE_TANK_OFFSET); // Align the bottom of the tank with the bottom of the text

		// Draw a tank to represent a life
		drawSprite(tank_15x8, life_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
	}

}

/**
* Draws the base line
*/
void drawBaseLine() {
	// Loop over the whole screen (left to right)
	uint16_t i;
	for(i=0; i < SCREEN_WIDTH; i++) {
		// At the given y-value, draw each pixel, making a solid line across the screen
		writePixel(BASE_LINE_Y, i, BASE_LINE_COLOR);
	}
}

/**
* Initializes all the sprites
*/
void render_init() {
	// Initialize position and values
	globals_init();

	explosion_location.x = EXPLOSION_DEFAULT;
	explosion_location.y = EXPLOSION_DEFAULT;

	drawTank(); // Draw the tank
	drawBunkers(); // Draw the bunkers
	drawAliens(); // Draw the aliens

	drawStatusBar(); // Draw the status bar
	drawBaseLine(); // Draw the base line
}
















