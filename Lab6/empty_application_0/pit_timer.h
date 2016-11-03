/*
 * pit_timer.h
 *
 *  Created on: Nov 2, 2016
 *      Author: superman
 */

#ifndef PIT_TIMER_H_
#define PIT_TIMER_H_

#include <stdint.h>

void pit_timer_init();

void pit_timer_start();

void pit_timer_stop();

void pit_timer_enableInt();

void pit_timer_disableInt();

void pit_timer_loopOn();

void pit_timer_loopOff();

void pit_timer_setTimer(uint32_t timer_value);

uint32_t pit_timer_readCountReg();

uint32_t pit_timer_readDelayReg();

uint32_t pit_timer_readControlReg();

#endif /* PIT_TIMER_H_ */
