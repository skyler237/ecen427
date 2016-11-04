/*
 * pit_timer.c
 *
 *  Created on: Nov 2, 2016
 *      Author: superman
 */
#include "pit_timer.h"
#include "xil_io.h"
#include "xparameters.h"

// Base addresses for the PIT registers
#define COUNT_REG XPAR_PITIMER_0_BASEADDR 			// Count register - holds timer count value
#define DELAY_COUNT_REG (XPAR_PITIMER_0_BASEADDR+8)	// Delay count register - holds the "top" value of the timer
#define CONTROL_REG (XPAR_PITIMER_0_BASEADDR+4)		// Control register - controls timer settings

// Default initial value for the timer
#define TIMER_DEFAULT 1000000

// Control masks
#define START_MASK 0x1 			// Mask to enable/disable decrementing the counter
#define INTERRUPT_MASK 0x2		// Mask to enable/disable interrupts
#define LOOP_MASK 0x4			// Mask to enable/disable looping to the top when counter expires

// Initialize the PIT
void pit_timer_init(){
	Xil_Out32(COUNT_REG, TIMER_DEFAULT); // Set the default value for the counter
	Xil_Out32(DELAY_COUNT_REG, TIMER_DEFAULT); // Set the delay count register to default value
	Xil_Out32(CONTROL_REG, 0); // Initialize control to zero (decrement, interrupts, and looping off)
}

// Starts the timer 
void pit_timer_start(){
	// Read out current settings
	uint32_t settings = Xil_In32(CONTROL_REG);
	// Add the start bit
	settings |= START_MASK;
	// Write back the control
	Xil_Out32(CONTROL_REG, settings);
}

// Stops the timer
void pit_timer_stop(){
	// Read out current settings
	uint32_t settings = Xil_In32(CONTROL_REG);
	// Remove the start bit
	settings &= ~START_MASK;
	// Write back the control
	Xil_Out32(CONTROL_REG, settings);
}

// Enables interrupts for the timer
void pit_timer_enableInt(){
	// Read out current settings
	uint32_t settings = Xil_In32(CONTROL_REG);
	// Add the interrupt bit
	settings |= INTERRUPT_MASK;
	// Write back the control
	Xil_Out32(CONTROL_REG, settings);
}

// Disables interrupts for the timer
void pit_timer_disableInt(){
	// Read out current settings
	uint32_t settings = Xil_In32(CONTROL_REG);
	// Remove the interrupt bit
	settings &= ~INTERRUPT_MASK;
	// Write back the control
	Xil_Out32(CONTROL_REG, settings);
}

// Enable looping 
void pit_timer_loopOn(){
	// Read out current settings
	uint32_t settings = Xil_In32(CONTROL_REG);
	// Add the loop bit
	settings |= LOOP_MASK;
	// Write back the control
	Xil_Out32(CONTROL_REG, settings);
}

// Disable looping
void pit_timer_loopOff(){
	// Read out current settings
	uint32_t settings = Xil_In32(CONTROL_REG);
	// Remove the loop bit
	settings &= ~LOOP_MASK;
	// Write back the control
	Xil_Out32(CONTROL_REG, settings);
}

// Set the timer value
void pit_timer_setTimer(uint32_t timer_value){
	// Set the delay count reg value
	Xil_Out32(DELAY_COUNT_REG, timer_value);
}

// Returns the count register value
uint32_t pit_timer_readCountReg(){
	// Read the count reg value
	return Xil_In32(COUNT_REG);
}

// Returns the dealy count register value
uint32_t pit_timer_readDelayReg(){
	// Read the delay count reg value
	return Xil_In32(DELAY_COUNT_REG);
}

// Returns the control register value
uint32_t pit_timer_readControlReg(){
	// Read the control reg value
	return Xil_In32(CONTROL_REG);
}
