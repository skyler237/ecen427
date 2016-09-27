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

void render_init(uint32_t* framePtr);
void render_refresh(uint32_t* framePtr);

#endif /* RENDER_H_ */
