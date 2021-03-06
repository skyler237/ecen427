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
void render_eraseAlien();
// Updates a block to reflect an increased erosion state -- doesn't actually cause the erosion
void render_erodeBlock(uint8_t bunker, uint8_t block_index);
// Completely erases the given bunker block
void render_eraseBlock(uint8_t bunker_index, uint8_t block_index);
// Renders all the current bullets
void render_bullets();
//Draws the alien explosion
void render_killAlien(uint8_t row, uint8_t col);
// Updates the score bar
void render_score(uint16_t score, uint16_t x, uint16_t y, uint32_t color);
//Erases one of the lives off of the score bar
void render_loseLife();
//Draws the UFO at the top of the screen
void render_UFO();
// Flashes the score in place of where the UFO was
void render_killUFO(uint16_t score);
// Erase the UFO score display
void render_eraseUFOScore();
// Displays the Game Over splash text
void render_gameOver();
// Initializes all the sprites. Also draws the status bar and sprites
void render_init();

#endif /* RENDER_H_ */
