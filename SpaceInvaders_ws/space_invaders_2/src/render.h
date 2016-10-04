/*
 * render.h
 *
 *  Created on: Sep 21, 2016
 *      Author: superman
 */

#ifndef RENDER_H_
#define RENDER_H_

#include <stdint.h>

// Draws the backround color to the whole screen
<<<<<<< HEAD
void render_blankScreen();
// Refreshes the tank sprite
void render_refreshTank();
// Refreshes the alien sprites
void render_refreshAliens();
// Erases the given alien sprite
void render_eraseAlien(uint8_t row, uint8_t col);
// Updates a block to reflect an increased erosion state -- doesn't actually cause the erosion
void render_erodeBlock(uint8_t bunker, uint8_t block_index);
// Renders all the current bullets
void render_bullets();
//Draws the alien explosion
void render_killAlien(uint8_t row, uint8_t col);

// Initializes all the sprites. Also draws the status bar and sprites
void render_init();
=======
void render_blankScreen(uint32_t* framePtr);
// Refreshes the tank sprite
void render_refreshTank(uint32_t* framePtr);
// Refreshes the alien sprites
void render_refreshAliens(uint32_t* framePtr);
// Erases the given alien sprite
void render_eraseAlien(uint32_t* framePtr, uint8_t row, uint8_t col);
// Updates a block to reflect an increased erosion state -- doesn't actually cause the erosion
void render_erodeBlock(uint32_t* framePtr, uint8_t bunker, uint8_t block_index);
// Renders all the current bullets
void render_bullets(uint32_t* framePtr);

// Initializes all the sprites. Also draws the status bar and sprites
void render_init(uint32_t* framePtr);
>>>>>>> a488b33a0129a43e4a88fa10073cdf55f655d39c

#endif /* RENDER_H_ */
