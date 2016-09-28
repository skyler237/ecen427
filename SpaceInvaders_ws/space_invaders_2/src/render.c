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

#define SCALING_CONST 2
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define LINE_WIDTH 640
#define NEXT_COL 1
#define BACKGROUND_COLOR 0x00000000

#define TANK_WIDTH 15
#define TANK_HEIGHT 8
#define TANK_MSB 0x8000
#define TANK_COLOR 0x0000FF00

#define BLOCKS_PER_BUNKER 10
#define BUNKER_CNT 4
#define BUNKER_WIDTH 24
#define BUNKER_HEIGHT 18
#define BUNKER_COLOR 0x0000FF00
#define BLOCK_WIDTH 6
#define BLOCK_HEIGHT 6

#define ALIEN_ROWS 5
#define ALIEN_COLS 11
#define TOP_ALIEN_ROW 0
#define MID_ALIEN_ROW 1
#define BOTTOM_ALIEN_ROW 3
#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define ALIEN_COLOR 0xFFFFFFFF
#define ALIEN_X_SPACING (ALIEN_WIDTH + 4)
#define ALIEN_Y_SPACING (ALIEN_HEIGHT + 6)

#define STATUS_BAR_Y 10
#define TEXT_COLOR 0xFFFFFFFF
#define SCORE_TEXT_X 15
#define SCORE_TEXT_WIDTH 32
#define SCORE_TEXT_HEIGHT 5
#define LIVES_TEXT_X (SCREEN_WIDTH*2/3)
#define LIVES_TEXT_WIDTH 24
#define LIVES_TEXT_HEIGHT 5
#define TEXT_2_LIFE_SPACING 10
#define LIFE_2_LIFE_SPACING 5
#define LIFE_TANK_OFFSET (TANK_HEIGHT - LIVES_TEXT_HEIGHT)

#define BASE_LINE_Y (SCREEN_HEIGHT - 5)
#define BASE_LINE_COLOR 0x0000FF00
//=======================================================================================
// 							Private Helper Functions
//=======================================================================================

/**
 * Writes a particular color to a given pixel
 * Also provides an abstraction from the actual resolution (640x480) to our game resolution (320x240)
 * @param row, col: the row and column in GAME PIXELS - NOT SCREEN PIXELS (e.g. 320,240 would be bottom right 2x2 block)
 */
void writePixel(uint32_t* framePtr, uint16_t row, uint16_t col, uint32_t color) {
	// Write a 2x2 pixel block
	framePtr[row*SCALING_CONST*LINE_WIDTH + col*SCALING_CONST] = color; //Top left pixel
	framePtr[row*SCALING_CONST*LINE_WIDTH + (col*SCALING_CONST + NEXT_COL)] = color;	// Top right pixel
	framePtr[(row*SCALING_CONST*LINE_WIDTH + LINE_WIDTH) + col*SCALING_CONST] = color; // Bottom left pixel
	framePtr[(row*SCALING_CONST*LINE_WIDTH + LINE_WIDTH) + (col*SCALING_CONST + NEXT_COL)] = color; // Bottom right pixel
}

/**
 * Iterates through the entire screen, making all the pixels black
 */
void render_blankScreen(uint32_t* framePtr) {
	int row, col;
	for(row=0; row < SCREEN_HEIGHT; row++) { // Iterate through rows
		for(col=0; col < SCREEN_WIDTH; col++) { // Iterate through columns
			writePixel(framePtr, row, col, BACKGROUND_COLOR); // Write black to the whole screen
		}
	}
}

/**
 * Generic draw sprite function
 */
void drawSprite(uint32_t* framePtr, const uint32_t* spriteArray, point_t sprite_pos, uint8_t spriteWidth, uint8_t spriteHeight, uint32_t sprite_color) {
	int row, col;
	for(row=0; row < spriteHeight; row++) {
		uint32_t sprite_row = spriteArray[row];
		for(col=0; col < spriteWidth; col++) {
			uint32_t msb = 0x01 << (spriteWidth - 1);
			if(sprite_row & (msb >> col)) {
				writePixel(framePtr, row+sprite_pos.y, col+sprite_pos.x, sprite_color);
//				xil_printf("Printing pixel: row=%d,  col=%d\n\r", row+sprite_pos.x, col+sprite_pos.y);
			}
		}
	}
}

void refreshSprite(uint32_t* framePtr, const uint32_t* spriteArray, const uint32_t* oldSpriteArray, point_t current_pos,
								uint8_t spriteWidth, uint8_t spriteHeight, uint32_t sprite_color) {

	uint32_t current_row;
	uint32_t old_row;
	uint32_t to_draw;
	uint32_t to_erase;
	point_t index_pos;
//	xil_printf("\n\nCall Entered");
//	xil_printf("curr_x = %d, prev_x = %d\n\r", current_pos.x, current_pos.prev_x);
	int row, col;
		for(row=0; row < spriteHeight; row++) {
			int8_t diff = current_pos.x - current_pos.prev_x;
			if(diff >= 0) {
				current_row = spriteArray[row];
				old_row = (oldSpriteArray[row] << diff);
				index_pos.x = current_pos.prev_x;
				index_pos.y = current_pos.prev_y;
			}
			else {
				diff = -diff;
				old_row = oldSpriteArray[row];
				current_row = (spriteArray[row] << diff);
				index_pos = current_pos;
			}
//			xil_printf("index_pos = %d\n\r", index_pos.x);
			to_draw = current_row & ~old_row;
			to_erase = old_row & ~current_row;
//			xil_printf("TO Draw: %d\n To Erase: %d\n", to_draw, to_erase );

			for(col=0; col < (spriteWidth + diff); col++) {
				uint32_t msb = 0x01 << (spriteWidth + diff - 1);
				if(to_draw & (msb >> col)) {
					writePixel(framePtr, row+index_pos.y, col+index_pos.x, sprite_color);
//					xil_printf("Printing pixel: row=%d,  col=%d\n\r", row+index_pos.x, col+index_pos.y);
				}
				if(to_erase & (msb >> col)) {
					writePixel(framePtr, row+index_pos.y, col+index_pos.x, BACKGROUND_COLOR);
//					xil_printf("Erasing pixel: row=%d,  col=%d\n\r", row+index_pos.x, col+index_pos.y);
				}
			}
		}

}

//void drawTank(uint32_t* framePtr) {
//	int row, col;
//	point_t tank_pos = getTankPositionGlobal();
//	for(row=0; row < TANK_HEIGHT; row++) {
//		uint16_t tank_row = tank_15x8[row];
//		for(col=0; col < TANK_WIDTH; col++) {
//			if(tank_row & (TANK_MSB >> col)) {
//				writePixel(framePtr, row+tank_pos.x, col+tank_pos.y, TANK_COLOR);
//			}
//		}
//	}
//}

void drawTank(uint32_t* framePtr) {
	point_t tank_pos = global_getTankPositionGlobal();
	drawSprite(framePtr, tank_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
}

void render_refreshTank(uint32_t* framePtr) {
	point_t tank_pos = global_getTankPositionGlobal();
	refreshSprite(framePtr, tank_15x8, tank_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
}

void drawBunkers(uint32_t* framePtr) {
	int bunker;
	for(bunker = 0; bunker < BUNKER_CNT; bunker++) {
		point_t bunker_pos = global_getBunkerPosition(bunker);

		drawSprite(framePtr, bunker_24x18, bunker_pos, BUNKER_WIDTH, BUNKER_HEIGHT, BUNKER_COLOR);
	}
}

void drawErodedBlock(uint32_t* framePtr, uint8_t bunker, uint8_t block_index) {
	point_t block_pos = global_getBlockPosition(bunker, block_index);
	erosionState_t block_state = global_getBlockState(bunker, block_index);
	const uint32_t* block_bitmap;
	switch(block_state) {
	case WHOLE:
			block_bitmap = bunkerDamage3_6x6;
		break;

	case HIT1:
			block_bitmap = bunkerDamage2_6x6;
		break;

	case HIT2:
			block_bitmap = bunkerDamage1_6x6;
		break;

	case HIT3:
			block_bitmap = bunkerDamage0_6x6;
		break;

	case DEAD:
			block_bitmap = bunkerDead_6x6;
	}

	drawSprite(framePtr, block_bitmap, block_pos, BLOCK_WIDTH, BLOCK_HEIGHT, BUNKER_COLOR);
}

void drawAliens(uint32_t* framePtr) {
	uint8_t row, col;
	const uint32_t* alien_bitmap;
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
//				xil_printf("alien position: x=%d, y=%d\n\r", alien_pos.x, alien_pos.y);
				drawSprite(framePtr, alien_bitmap, alien_pos, ALIEN_WIDTH, ALIEN_HEIGHT, ALIEN_COLOR);
			}
		}
	}
}

void render_refreshAliens(uint32_t* framePtr) {
	uint8_t row, col;
	const uint32_t* alien_bitmap;
	const uint32_t* old_alien_bitmap;
	point_t alien_block_pos = global_getAlienBlockPosition();
//	xil_printf("Alien_pos: x=%d, y=%d\n\r", alien_block_pos.x, alien_block_pos.y);
	for(row=0; row < ALIEN_ROWS; row++) { // Iterate through the 5 rows
		for(col=0; col < ALIEN_COLS; col++) { // Iterate through each of the 11 aliens on a row
			if(global_isAlienAlive(row,col)) { // Check to see if the alien is still alive
				if(row == TOP_ALIEN_ROW) { // If we are drawing the top row...
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
//				xil_printf("alien position: x=%d, y=%d\n\r", alien_pos.x, alien_pos.y);
				refreshSprite(framePtr, alien_bitmap, old_alien_bitmap, alien_pos, ALIEN_WIDTH, ALIEN_HEIGHT, ALIEN_COLOR);
			}
		}
	}
}

void drawStatusBar(uint32_t* framePtr) {
	point_t score_pos;
	score_pos.x = SCORE_TEXT_X;
	score_pos.y = STATUS_BAR_Y;
	drawSprite(framePtr, scoreText_32x5, score_pos, SCORE_TEXT_WIDTH, SCORE_TEXT_HEIGHT, TEXT_COLOR);

	point_t lives_pos;
	lives_pos.x = LIVES_TEXT_X;
	lives_pos.y = STATUS_BAR_Y;
	drawSprite(framePtr, livesText_24x5, lives_pos, LIVES_TEXT_WIDTH, LIVES_TEXT_HEIGHT, TEXT_COLOR);

	uint16_t score = global_getScore();
	// Draw the score here...

	uint8_t lives = global_getLives();
	uint8_t i;
	for(i = 0; i < lives; i++) {
		point_t life_pos;
		life_pos.x = (LIVES_TEXT_X + LIVES_TEXT_WIDTH + TEXT_2_LIFE_SPACING + (LIFE_2_LIFE_SPACING + TANK_WIDTH)*i);
		life_pos.y = (STATUS_BAR_Y - LIFE_TANK_OFFSET);

		drawSprite(framePtr, tank_15x8, life_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
	}

}

void drawBaseLine(uint32_t* framePtr) {
	uint16_t i;
	for(i=0; i < SCREEN_WIDTH; i++) {
		writePixel(framePtr, BASE_LINE_Y, i, BASE_LINE_COLOR);
	}
}

void render_init(uint32_t* framePtr) {
	globals_init();

	drawTank(framePtr);
	drawBunkers(framePtr);
	drawAliens(framePtr);

	drawStatusBar(framePtr);
	drawBaseLine(framePtr);
}

void render_refresh(uint32_t* framePtr) {
	render_refreshTank(framePtr);
//	refreshBunkers(framePtr);
	render_refreshAliens(framePtr);

//	refreshStatusBar(framePtr);
}
















