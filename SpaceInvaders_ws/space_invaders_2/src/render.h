/*
 * render.h
 *
 *  Created on: Sep 21, 2016
 *      Author: superman
 */

#ifndef RENDER_H_
#define RENDER_H_

#include <stdint.h>

void render_blankScreen(uint32_t* framePtr);
void render_refreshTank(uint32_t* framePtr);
void render_refreshAliens(uint32_t* framePtr);
void render_eraseAlien(uint32_t* framePtr, uint8_t row, uint8_t col);
void render_erodeBlock(uint32_t* framePtr, uint8_t bunker, uint8_t block_index);
void render_bullets(uint32_t* framePtr);

void render_init(uint32_t* framePtr);
void render_refresh(uint32_t* framePtr);

#endif /* RENDER_H_ */
