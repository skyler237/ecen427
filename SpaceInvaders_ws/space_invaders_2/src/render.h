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

#endif /* RENDER_H_ */
