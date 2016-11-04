/*
 * pit_timer.h
 *
 *  Created on: Nov 2, 2016
 *      Author: superman
 */

#ifndef PIT_TIMER_H_
#define PIT_TIMER_H_

#include <stdint.h>

// Initialize the PIT
void pit_timer_init();
// Starts the timer 
void pit_timer_start();
// Stops the timer
void pit_timer_stop();
// Enables interrupts for the timer
void pit_timer_enableInt();
// Disables interrupts for the timer
void pit_timer_disableInt();
// Enable looping 
void pit_timer_loopOn();
// Disable looping
void pit_timer_loopOff();
// Set the timer value
void pit_timer_setTimer(uint32_t timer_value);
// Returns the count register value
uint32_t pit_timer_readCountReg();
// Returns the dealy count register value
uint32_t pit_timer_readDelayReg();
// Returns the control register value
uint32_t pit_timer_readControlReg();



#endif /* PIT_TIMER_H_ */
