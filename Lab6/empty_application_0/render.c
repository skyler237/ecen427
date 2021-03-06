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
#define packword10(b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | \
 (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 12 bit word.
#define packword12(b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) | \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | \
 (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 15 bit word.
#define packword14(b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 15 bit word.
#define packword15(b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 24 bit word.
#define packword24(b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |						  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |						  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 6 bit word.
#define packword6(b5,b4,b3,b2,b1,b0) \
((b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 5 bit word.
#define packword5(b4,b3,b2,b1,b0) \
((b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

// Packs each horizontal line of the figures into a single 3 bit word.
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

static const  uint32_t tank_explosion1_15x8[] =
{
packword15(0,1,0,0,0,0,0,1,0,0,0,0,0,1,0),
packword15(0,0,0,0,1,0,0,0,0,0,0,0,0,0,0),
packword15(0,0,1,0,0,0,0,1,0,0,0,0,0,1,0),
packword15(0,0,0,0,0,0,1,0,0,0,0,1,0,0,1),
packword15(0,1,0,0,0,1,1,0,0,1,0,0,0,0,0),
packword15(0,0,0,0,1,1,1,1,1,1,1,0,0,1,0),
packword15(0,0,0,1,1,1,1,1,1,1,1,1,0,0,0),
packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0)
};

static const  uint32_t tank_explosion2_15x8[] =
{
packword15(0,0,0,0,0,1,0,0,0,0,0,0,0,0,0),
packword15(0,0,0,1,0,0,0,0,1,0,0,1,0,0,0),
packword15(0,0,0,0,0,1,0,1,0,0,1,0,1,0,0),
packword15(0,0,0,1,0,0,0,0,0,0,0,0,0,1,0),
packword15(0,0,0,0,0,0,1,0,1,1,0,0,0,0,0),
packword15(0,1,0,0,1,1,1,1,1,1,1,1,0,0,0),
packword15(0,0,0,1,1,1,1,1,1,1,1,1,1,0,0),
packword15(0,1,1,1,1,1,1,1,1,1,1,1,1,1,0)
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


static const uint32_t zeroText_5x5[] =
{
	packword5(0,1,1,1,0),
	packword5(1,0,0,0,1),
	packword5(1,0,0,0,1),
	packword5(1,0,0,0,1),
	packword5(0,1,1,1,0),
};

static const uint32_t oneText_5x5[] =
{
	packword5(0,0,0,1,1),
	packword5(0,0,0,0,1),
	packword5(0,0,0,0,1),
	packword5(0,0,0,0,1),
	packword5(0,0,0,0,1),
};

static const uint32_t twoText_5x5[] =
{
	packword5(1,1,1,1,0),
	packword5(0,0,0,0,1),
	packword5(0,1,1,1,1),
	packword5(1,0,0,0,0),
	packword5(1,1,1,1,1),
};

static const uint32_t threeText_5x5[] =
{
	packword5(1,1,1,1,0),
	packword5(0,0,0,0,1),
	packword5(0,1,1,1,1),
	packword5(0,0,0,0,1),
	packword5(1,1,1,1,0),
};

static const uint32_t fourText_5x5[] =
{
	packword5(1,0,0,0,1),
	packword5(1,0,0,0,1),
	packword5(1,1,1,1,1),
	packword5(0,0,0,0,1),
	packword5(0,0,0,0,1),
};

static const uint32_t fiveText_5x5[] =
{
	packword5(1,1,1,1,1),
	packword5(1,0,0,0,0),
	packword5(1,1,1,1,0),
	packword5(0,0,0,0,1),
	packword5(1,1,1,1,0),
};

static const uint32_t sixText_5x5[] =
{
	packword5(0,1,1,1,0),
	packword5(1,0,0,0,0),
	packword5(1,1,1,1,0),
	packword5(1,0,0,0,1),
	packword5(0,1,1,1,0),
};

static const uint32_t sevenText_5x5[] =
{
	packword5(0,1,1,1,1),
	packword5(0,0,0,0,1),
	packword5(0,0,0,0,1),
	packword5(0,0,0,1,0),
	packword5(0,0,0,1,0),
};

static const uint32_t eightText_5x5[] =
{
	packword5(0,1,1,1,0),
	packword5(1,0,0,0,1),
	packword5(0,1,1,1,0),
	packword5(1,0,0,0,1),
	packword5(0,1,1,1,0),
};

static const uint32_t nineText_5x5[] =
{
	packword5(0,1,1,1,0),
	packword5(1,0,0,0,1),
	packword5(0,1,1,1,1),
	packword5(0,0,0,0,1),
	packword5(0,1,1,1,0),
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

static const uint32_t G_10x10[] =
{
		packword10(0,0,1,1,1,1,1,1,1,1),
		packword10(0,0,1,1,1,1,1,1,1,1),
		packword10(1,1,0,0,0,0,0,0,0,0),
		packword10(1,1,0,0,0,0,0,0,0,0),
		packword10(1,1,0,0,1,1,1,1,1,1),
		packword10(1,1,0,0,1,1,1,1,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(0,0,1,1,1,1,1,1,1,1),
		packword10(0,0,1,1,1,1,1,1,1,1)
};

static const uint32_t A_10x10[] =
{
		packword10(0,0,1,1,1,1,1,1,0,0),
		packword10(0,0,1,1,1,1,1,1,0,0),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,1,1,1,1,1,1,1,1),
		packword10(1,1,1,1,1,1,1,1,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1)
};



static const uint32_t M_10x14[] =
{
		packword14(1,1,1,1,1,1,0,0,1,1,1,1,0,0),
		packword14(1,1,1,1,1,1,0,0,1,1,1,1,0,0),
		packword14(1,1,0,0,0,0,1,1,0,0,0,0,1,1),
		packword14(1,1,0,0,0,0,1,1,0,0,0,0,1,1),
		packword14(1,1,0,0,0,0,1,1,0,0,0,0,1,1),
		packword14(1,1,0,0,0,0,1,1,0,0,0,0,1,1),
		packword14(1,1,0,0,0,0,1,1,0,0,0,0,1,1),
		packword14(1,1,0,0,0,0,1,1,0,0,0,0,1,1),
		packword14(1,1,0,0,0,0,1,1,0,0,0,0,1,1),
		packword14(1,1,0,0,0,0,1,1,0,0,0,0,1,1)
};

static const uint32_t E_10x10[] =
{
		packword10(1,1,1,1,1,1,1,1,1,1),
		packword10(1,1,1,1,1,1,1,1,1,1),
		packword10(1,1,0,0,0,0,0,0,0,0),
		packword10(1,1,0,0,0,0,0,0,0,0),
		packword10(1,1,1,1,1,1,1,1,0,0),
		packword10(1,1,1,1,1,1,1,1,0,0),
		packword10(1,1,0,0,0,0,0,0,0,0),
		packword10(1,1,0,0,0,0,0,0,0,0),
		packword10(1,1,1,1,1,1,1,1,1,1),
		packword10(1,1,1,1,1,1,1,1,1,1)
};

static const uint32_t O_10x10[] =
{
		packword10(0,0,1,1,1,1,1,1,0,0),
		packword10(0,0,1,1,1,1,1,1,0,0),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(0,0,1,1,1,1,1,1,0,0),
		packword10(0,0,1,1,1,1,1,1,0,0)
};

static const uint32_t V_10x10[] =
{
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(0,0,1,1,0,0,1,1,0,0),
		packword10(0,0,1,1,0,0,1,1,0,0),
		packword10(0,0,0,0,1,1,0,0,0,0),
		packword10(0,0,0,0,1,1,0,0,0,0)
};

static const uint32_t R_10x10[] =
{
		packword10(1,1,1,1,1,1,1,1,0,0),
		packword10(1,1,1,1,1,1,1,1,0,0),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,1,1,1,1,1,1,0,0),
		packword10(1,1,1,1,1,1,1,1,0,0),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1),
		packword10(1,1,0,0,0,0,0,0,1,1)
};



#define FRAME_BUFFER_0_ADDR 0xC1000000
#define EXPLOSION_DEFAULT -1

static point_t explosion_location; // Keeps track of the last exploding alien guise
static point_t score_loc; // Keeps track of the location for the flashing score (when killing the UFO)
static uint16_t ufo_score; // Keeps track of the UFO score to flash
// General frame pointer for all rendering
static uint32_t * framePtr = (uint32_t *) FRAME_BUFFER_0_ADDR; 

// Screen constants
#define SCALING_CONST 2 // Scaling constant between game pixels and actual screen density
#define LINE_WIDTH 640	// Number of screen pixels in one line
#define NEXT_COL 1		// Denotes the next column over
#define BACKGROUND_COLOR 0x00000000	// Background color for the screen

// Tank values
#define TANK_MSB 0x8000			// Most significant bit of the tank sprite rows
#define TANK_COLOR GREEN		// Tank color

// Bunker values
#define BUNKER_COLOR GREEN // Bunker color

// Alien values
#define ALIEN_COLOR 0xFFFFFFFF	// Alien color
#define ALIEN_EXPLOSION_HEIGHT 10 //Height in pixels of the explosion sprite

// Text values
#define TEXT_COLOR 0xFFFFFFFF	// General text color
#define NUMBER_WIDTH 5
#define NUMBER_HEIGHT 5
#define NUMBER_SPACING (NUMBER_WIDTH + 1)
#define NUMBER_COLOR GREEN

// Modulus values
#define THOUSANDS_PLACE 1000
#define HUNDREDS_PLACE 100
#define TENS_PLACE 10

// Lives display values
#define LIVES_TEXT_X (SCREEN_WIDTH*2/3)	// Lives text x offset
#define LIVES_TEXT_WIDTH 24		// Lives text sprite width
#define LIVES_TEXT_HEIGHT TEXT_HEIGHT	// Lives text sprite height
#define TEXT_2_LIFE_SPACING 10	// Spacing from lives text to the first life sprite (tank)
#define LIFE_2_LIFE_SPACING 5	// Spacing between life sprites (tanks)
#define LIFE_TANK_OFFSET (TANK_HEIGHT - LIVES_TEXT_HEIGHT)	// Offset to help align life sprites and lives text

// Game over splash text values
#define GAMEOVER_TEXT_HEIGHT 10
#define GAMEOVER_TEXT_WIDTH 10
#define GAMEOVER_M_TEXT_WIDTH 14 // the 'M' character is wider than the rest
#define GAMEOVER_TEXT_X 110
#define GAMEOVER_TEXT_Y 80
#define GAMEOVER_TEXT_SPACING 1 // Spacing between individual letters
#define GAMEOVER_WORD_SPACING 6 // Spacing between the words
#define GAMEOVER_TEXT_COLOR GREEN

#define BASE_LINE_COLOR GREEN	// Base line color

#define BULLET_COLOR 0xFFFFFFFF		// Bullet color
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

void drawSpriteWithBG(const uint32_t* spriteArray, point_t sprite_pos, uint8_t spriteWidth, uint8_t spriteHeight, uint32_t sprite_color) {
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
			else {
				writePixel(row+sprite_pos.y, col+sprite_pos.x, BACKGROUND_COLOR);
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
				if ((row + index_pos.y >= 0) && (row + index_pos.y < SCREEN_HEIGHT) && (col + index_pos.x >= 0) && (col + index_pos.x < SCREEN_WIDTH)){
					writePixel(row+index_pos.y, col+index_pos.x, sprite_color);
				}
			}
			// If the column has a bit set in "to_erase" then draw the background
			else if(to_erase & (msb >> col)) {
				// Call wirte pixel function with background color
				if ((row + index_pos.y >= 0) && (row + index_pos.y < SCREEN_HEIGHT) && (col + index_pos.x >= 0) && (col + index_pos.x < SCREEN_WIDTH)){
					writePixel(row+index_pos.y, col+index_pos.x, BACKGROUND_COLOR);
				}
			}
		}
	}

}

/**
* Draws the tank sprite (only used on to initialize)
*/
void drawTank() {
	// Get the current position
	point_t tank_pos = global_getTankPosition();
	// Call the draw sprite function
	drawSprite(tank_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
}

/**
* Refreshes the tank sprite
*/
void render_refreshTank() {
	// Get the current position
	point_t tank_pos = global_getTankPosition();
	// Call the refresh sprite function
	if (!global_isTankDead()){ // When the tank is not dead, draw like normal
		refreshSprite(tank_15x8, tank_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
	}
	else { // If the tank is dead, show "rubble" guise
		// Get the death timer value
		uint32_t deathTimer = global_getDeathTimer();
		// If the timer is just starting, display the tank explosion
		if (deathTimer == TANK_DEATH_TIMER_MAX){
			refreshSprite(tank_explosion1_15x8, tank_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
		}
		// For ever "flashing timer" interval...
		else if (deathTimer % FLASHING_TIMER_MAX == 0){//Switch which death sprite is shown
			// For even increments of the flashing count, show one guise
			if((deathTimer / FLASHING_TIMER_MAX)%DEATH_GUISES == 0){
				refreshSprite(tank_explosion1_15x8, tank_explosion2_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
			}
			// For odd increments of the flashing count, show the other guise
			else { 
				refreshSprite(tank_explosion2_15x8, tank_explosion1_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
			}
		}
		// When the timer is almost all done, redraw the original tank
		else if (deathTimer == REVIVE_TANK){
			refreshSprite(tank_15x8, tank_explosion2_15x8, tank_pos, TANK_WIDTH, TANK_HEIGHT, TANK_COLOR);
		}
	}
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
 * Erases the given block
 */
void render_eraseBlock(uint8_t bunker_index, uint8_t block_index) {
	point_t block_pos = global_getBlockPosition(bunker_index, block_index);
	drawSprite(bunkerDamage3_6x6, block_pos, BUNKER_BLOCK_SIZE, BUNKER_BLOCK_SIZE, BACKGROUND_COLOR);
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
void drawAliens(uint32_t color) {
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
				drawSprite(alien_bitmap, alien_pos, ALIEN_WIDTH, ALIEN_HEIGHT, color);
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
	global_setFlashingTimer();
}

/**
* Erases a given alien
*/
void render_eraseAlien() {
	drawSprite(alien_explosion_12x10, explosion_location, ALIEN_WIDTH, ALIEN_EXPLOSION_HEIGHT, BACKGROUND_COLOR);
}

/**
 * Draws the GAME OVER splash screen text 
 */
void render_gameOver(){
	// Erases all the aliens
	drawAliens(BACKGROUND_COLOR);

	// Keep a cursor to draw each individual letter
	point_t cursor;
	cursor.x = GAMEOVER_TEXT_X;
	cursor.y = GAMEOVER_TEXT_Y;

	// Draw the 'G'
	drawSpriteWithBG(G_10x10, cursor, GAMEOVER_TEXT_WIDTH, GAMEOVER_TEXT_HEIGHT, GAMEOVER_TEXT_COLOR);
	// Move the cursor
	cursor.x += GAMEOVER_TEXT_WIDTH+GAMEOVER_TEXT_SPACING;

	// Draw the 'A'
	drawSpriteWithBG(A_10x10, cursor, GAMEOVER_TEXT_WIDTH, GAMEOVER_TEXT_HEIGHT, GAMEOVER_TEXT_COLOR);
	// Move the cursor
	cursor.x += GAMEOVER_TEXT_WIDTH+GAMEOVER_TEXT_SPACING;

	// Draw the 'M'
	drawSpriteWithBG(M_10x14, cursor, GAMEOVER_M_TEXT_WIDTH, GAMEOVER_TEXT_HEIGHT, GAMEOVER_TEXT_COLOR);
	// Move the cursor
	cursor.x += GAMEOVER_M_TEXT_WIDTH+GAMEOVER_TEXT_SPACING;

	// Draw the 'E'
	drawSpriteWithBG(E_10x10, cursor, GAMEOVER_TEXT_WIDTH, GAMEOVER_TEXT_HEIGHT, GAMEOVER_TEXT_COLOR);
	// Move the cursor -- add the space between words
	cursor.x += GAMEOVER_TEXT_WIDTH+GAMEOVER_WORD_SPACING; 

	// Draw the 'O'
	drawSpriteWithBG(O_10x10, cursor, GAMEOVER_TEXT_WIDTH, GAMEOVER_TEXT_HEIGHT, GAMEOVER_TEXT_COLOR);
	// Move the cursor
	cursor.x += GAMEOVER_TEXT_WIDTH+GAMEOVER_TEXT_SPACING;

	// Draw the 'V'
	drawSpriteWithBG(V_10x10, cursor, GAMEOVER_TEXT_WIDTH, GAMEOVER_TEXT_HEIGHT, GAMEOVER_TEXT_COLOR);
	// Move the cursor
	cursor.x += GAMEOVER_TEXT_WIDTH+GAMEOVER_TEXT_SPACING;

	// Draw the 'E'
	drawSpriteWithBG(E_10x10, cursor, GAMEOVER_TEXT_WIDTH, GAMEOVER_TEXT_HEIGHT, GAMEOVER_TEXT_COLOR);
	// Move the cursor
	cursor.x += GAMEOVER_TEXT_WIDTH+GAMEOVER_TEXT_SPACING;

	// Draw the 'R'
	drawSpriteWithBG(R_10x10, cursor, GAMEOVER_TEXT_WIDTH, GAMEOVER_TEXT_HEIGHT, GAMEOVER_TEXT_COLOR);
	// Move the cursor
	cursor.x += GAMEOVER_TEXT_WIDTH+GAMEOVER_TEXT_SPACING;

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
	// Draw the score
	render_score(score, SCORE_NUMBER_X, STATUS_BAR_Y, GREEN);

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

void render_loseLife(){
	uint8_t lives = global_getLives();
	point_t life_pos;
	life_pos.x = (LIVES_TEXT_X + LIVES_TEXT_WIDTH + TEXT_2_LIFE_SPACING + (LIFE_2_LIFE_SPACING + TANK_WIDTH)*(lives)); // Offset from the text
	life_pos.y = (STATUS_BAR_Y - LIFE_TANK_OFFSET); // Align the bottom of the tank with the bottom of the text

	// Draw a tank to represent a life
	drawSprite(tank_15x8, life_pos, TANK_WIDTH, TANK_HEIGHT, BACKGROUND_COLOR);
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

	// Initialize the explosion location
	explosion_location.x = EXPLOSION_DEFAULT;
	explosion_location.y = EXPLOSION_DEFAULT;

	drawTank(); // Draw the tank
	drawBunkers(); // Draw the bunkers
	drawAliens(ALIEN_COLOR); // Draw the aliens

	drawStatusBar(); // Draw the status bar
	drawBaseLine(); // Draw the base line
}

/**
 * Returns a pointer to the appropriate number bitmap for the given digit
 */
const uint32_t* getNumberBitmap(uint8_t digit) {
	switch(digit) {
	case 0: // Get '0' bitmap
		return zeroText_5x5;
	case 1: // Get '1' bitmap
		return oneText_5x5;
	case 2: // Get '2' bitmap
		return twoText_5x5;
	case 3: // Get '3' bitmap
		return threeText_5x5;
	case 4: // Get '4' bitmap
		return fourText_5x5;
	case 5: // Get '5' bitmap
		return fiveText_5x5;
	case 6: // Get '6' bitmap
		return sixText_5x5;
	case 7: // Get '7' bitmap
		return sevenText_5x5;
	case 8: // Get '8' bitmap
		return eightText_5x5;
	case 9: // Get '9' bitmap
		return nineText_5x5;
	}
	return 0;
}

/**
 * Updates the score bar
 */
void render_score(uint16_t score, uint16_t x, uint16_t y, uint32_t color) {
	// Keep a cursor to draw the score digits
	point_t cursor;
	cursor.x = x;
	cursor.y = y;

	const uint32_t* bitmap;

	// Extract each digit
	uint8_t thousands = score / THOUSANDS_PLACE; // Get thousands place digit
	score -= thousands * THOUSANDS_PLACE;
	uint8_t hundreds = score / HUNDREDS_PLACE;   // Get hundreds place digit
	score -= hundreds * HUNDREDS_PLACE;
	uint8_t tens = score / TENS_PLACE;			 // Get tens place digit
	uint8_t ones = score % TENS_PLACE;			 // Get ones place digit

	if(thousands != 0) { // If there is a thousands place digit,
		// Get the appropriate bitmap
		bitmap = getNumberBitmap(thousands);
		// Draw the digit
		drawSpriteWithBG(bitmap, cursor, NUMBER_WIDTH, NUMBER_HEIGHT, color);
		// Move the cursor
		cursor.x += NUMBER_SPACING;
	}
	if (hundreds != 0 || thousands != 0) {
		// Get the appropriate bitmap
		bitmap = getNumberBitmap(hundreds);
		// Draw the digit
		drawSpriteWithBG(bitmap, cursor, NUMBER_WIDTH, NUMBER_HEIGHT, color);
		// Move the cursor
		cursor.x += NUMBER_SPACING;
	}
	if (tens != 0 || hundreds != 0 || thousands != 0) {
		// Get the appropriate bitmap
		bitmap = getNumberBitmap(tens);
		// Draw the digit
		drawSpriteWithBG(bitmap, cursor, NUMBER_WIDTH, NUMBER_HEIGHT, color);
		// Move the cursor
		cursor.x += NUMBER_SPACING;
	}
	// Get the appropriate bitmap
	bitmap = getNumberBitmap(ones);
	// Draw the digit
	drawSpriteWithBG(bitmap, cursor, NUMBER_WIDTH, NUMBER_HEIGHT, color);
}

/**
 * Draws the UFO sprite
 */
void render_UFO(){
	refreshSprite(saucer_16x7, saucer_16x7, global_getUFOPosition(), UFO_WIDTH, UFO_HEIGHT, UFO_COLOR);
}

/**
 * When the UFO is hit, it updates the UFO position and flashes the score
 */
void render_killUFO(uint16_t score){
	// Get the position for the score to flash
	score_loc.x = global_getUFOPosition().x;
	score_loc.y = global_getUFOPosition().y;
	
	// Update the UFO score variable
	ufo_score = score;

	// Draw the UFO
	drawSprite(saucer_16x7, score_loc, UFO_WIDTH, UFO_HEIGHT, BACKGROUND_COLOR);
	// Flash the score
	render_score(score, score_loc.x, score_loc.y, UFO_COLOR);
}

/**
 * Erases the UFO score display
 */ 
void render_eraseUFOScore(){
	render_score(ufo_score, score_loc.x, score_loc.y, BACKGROUND_COLOR);
}














