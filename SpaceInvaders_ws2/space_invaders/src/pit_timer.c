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

#define TIMER_DEFAULT 1000000

#define START_MASK 0x1
#define INTERRUPT_MASK 0x2
#define LOOP_MASK 0x4

void pit_timer_init(){
	Xil_Out32(COUNT_REG, TIMER_DEFAULT);
	Xil_Out32(DELAY_COUNT_REG, TIMER_DEFAULT);
	Xil_Out32(CONTROL_REG, 0);
}

void pit_timer_start(){
	uint32_t settings = Xil_In32(CONTROL_REG);
	settings |= START_MASK;
	Xil_Out32(CONTROL_REG, settings);
}

void pit_timer_stop(){
	uint32_t settings = Xil_In32(CONTROL_REG);
	settings &= ~START_MASK;
	Xil_Out32(CONTROL_REG, settings);
}

void pit_timer_enableInt(){
	uint32_t settings = Xil_In32(CONTROL_REG);
	settings |= INTERRUPT_MASK;
	Xil_Out32(CONTROL_REG, settings);
}

void pit_timer_disableInt(){
	uint32_t settings = Xil_In32(CONTROL_REG);
	settings &= ~INTERRUPT_MASK;
	Xil_Out32(CONTROL_REG, settings);
}

void pit_timer_loopOn(){
	uint32_t settings = Xil_In32(CONTROL_REG);
	settings |= LOOP_MASK;
	Xil_Out32(CONTROL_REG, settings);
}

void pit_timer_loopOff(){
	uint32_t settings = Xil_In32(CONTROL_REG);
	settings &= ~LOOP_MASK;
	Xil_Out32(CONTROL_REG, settings);
}

void pit_timer_setTimer(uint32_t timer_value){
	Xil_Out32(DELAY_COUNT_REG, timer_value);
}

uint32_t pit_timer_readCountReg(){
	return Xil_In32(COUNT_REG);
}

uint32_t pit_timer_readDelayReg(){
	return Xil_In32(DELAY_COUNT_REG);
}

uint32_t pit_timer_readControlReg(){
	return Xil_In32(CONTROL_REG);
}
