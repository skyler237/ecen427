/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdint.h>
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

uint8_t currentButtonState; // Holds the current value of the push buttons (updated each PB interrupt)
uint8_t upBtnDebounceCounter; // Used to debounce up button (max = 5 = 50ms)
uint8_t downBtnDebounceCounter; // Used to debounce down button (max = 5 = 50ms)
uint8_t leftBtnDebounceCounter; // Used to debounce left button (max = 5 = 50ms)
uint8_t centerBtnDebounceCounter; // Used to debounce center button (max = 5 = 50ms)
uint8_t rightBtnDebounceCounter; // Used to debounce right button (max = 5 = 50ms)

uint8_t secondCounter; // Used to count 1 second (max = 100 = 1000ms)
uint8_t secondsValue; // Holds the current seconds value for the clock
uint8_t minutesValue; // Holds the current minutes value for the clock
uint8_t hoursValue; // Holds the current hours value for the clock



// Push Button Definitions
#define CENTER_BTN 0x01
#define RIGHT_BTN 0x02
#define DOWN_BTN 0x04
#define LEFT_BTN 0x08
#define UP_BTN 0x10
#define DEBOUNCE_MAX 5

// Clock Definitions
#define SEC_COUNTER_MAX 100
#define SEC_VALUE_MAX 59
#define SEC_VALUE_MIN 0
#define MIN_VALUE_MAX 59
#define MIN_VALUE_MIN 0
#define HR_VALUE_MAX 12
#define HR_VALUE_MIN 1
#define HALF_SECOND 50

#define TIME_OVERFLOW 1
#define NO_TIME_OVERFLOW 0

#define TRUE 1
#define FALSE 0

uint8_t downHalfSecond = HALF_SECOND;
uint8_t upHalfSecond = HALF_SECOND;
// Increment seconds and check for roll-over
// @return: returns 1/true if there is roll-over - must increment minutes
uint8_t increment_seconds(){
	if(secondsValue == SEC_VALUE_MAX){ // If at max...
		secondsValue = SEC_VALUE_MIN; // Loop back to zero
		return TIME_OVERFLOW; // Indicate time roll-over
	}
	else{ // Otherwise,
		secondsValue++; // just increment time
		return NO_TIME_OVERFLOW; // and return normal (no overflow)
	}
}

// Increment minutes and check for roll-over
// @return: returns 1/true if there is roll-over - must increment hours
uint8_t increment_minutes(){
	if(minutesValue == MIN_VALUE_MAX){ // If at max...
		minutesValue = MIN_VALUE_MIN; // Loop back to zero
		return TIME_OVERFLOW; // Indicate time roll-over
	}
	else{ // Otherwise,
		minutesValue++; // just increment time
		return NO_TIME_OVERFLOW; // and return normal (no overflow)
	}
}

// Increment hours and check for roll-over
void increment_hours(){
	if(hoursValue == HR_VALUE_MAX){ // If at max...
		hoursValue = HR_VALUE_MIN; // Loop back to min
	}
	else{ // Otherwise,
		hoursValue++; // just increment time
	}
}

uint8_t decrement_seconds(){
	if(secondsValue == SEC_VALUE_MIN){ // If at min...
		secondsValue = SEC_VALUE_MAX; // Loop back to top
		return TIME_OVERFLOW; // Indicate time roll-over
	}
	else{ // Otherwise,
		secondsValue--; // just increment time
		return NO_TIME_OVERFLOW; // and return normal (no overflow)
	}
}

// Increment minutes and check for roll-over
// @return: returns 1/true if there is roll-over - must increment hours
uint8_t decrement_minutes(){
	if(minutesValue == MIN_VALUE_MIN){ // If at min...
		minutesValue = MIN_VALUE_MAX; // Loop back to top
		return TIME_OVERFLOW; // Indicate time roll-over
	}
	else{ // Otherwise,
		minutesValue--; // just increment time
		return NO_TIME_OVERFLOW; // and return normal (no overflow)
	}
}

// Increment hours and check for roll-over
void decrement_hours(){
	if(hoursValue == HR_VALUE_MIN){ // If at min...
		hoursValue = HR_VALUE_MAX; // Loop back to max
	}
	else{ // Otherwise,
		hoursValue--; // just increment time
	}
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.
void timer_interrupt_handler() {

	uint8_t secondsDebounced = FALSE;
	uint8_t minutesDebounced = FALSE;
	uint8_t hoursDebounced = FALSE;

	// -----Debounce switches-----
	// Check which buttons are pressed - increment debounce count
	if(currentButtonState & CENTER_BTN) { // If center button is pressed...
		if(centerBtnDebounceCounter < DEBOUNCE_MAX) { // If not at max,
		  centerBtnDebounceCounter++; // Increment debounce counter
		}
		else if(centerBtnDebounceCounter == DEBOUNCE_MAX) { // If the button is debounced...
			minutesDebounced = TRUE;
		}
	}

	if(currentButtonState & RIGHT_BTN) { // If right button is pressed...
		if(rightBtnDebounceCounter < DEBOUNCE_MAX) { // If not at max,
		rightBtnDebounceCounter++; // Increment debounce counter
		}
		else if(rightBtnDebounceCounter == DEBOUNCE_MAX) { // If the button is debounced...
			secondsDebounced = TRUE;
		}
	}

	if(currentButtonState & LEFT_BTN) { // If left button is pressed...
		if(leftBtnDebounceCounter < DEBOUNCE_MAX) { // If not at max,
			leftBtnDebounceCounter++; // Increment debounce counter
		}
		else if(leftBtnDebounceCounter == DEBOUNCE_MAX) { // If the button is debounced...
			hoursDebounced = TRUE;
		}
	}

	if (secondsDebounced|| minutesDebounced || hoursDebounced){
		if(currentButtonState & DOWN_BTN) { // If down button is pressed...
			if(downBtnDebounceCounter < SEC_COUNTER_MAX) { // If not at max,
				downBtnDebounceCounter++; // Increment debounce counter
			}
			else {
				if (downHalfSecond < HALF_SECOND) {
					downHalfSecond++;
				}
				else {
					downHalfSecond = 0;
				}
			}
			if(downBtnDebounceCounter == DEBOUNCE_MAX || (downBtnDebounceCounter == SEC_COUNTER_MAX && downHalfSecond == HALF_SECOND)) { // If the button is debounced...
				if(secondsDebounced == TRUE) {
					decrement_seconds();
				}
				if(minutesDebounced == TRUE) {
					decrement_minutes();
				}
				if(hoursDebounced == TRUE) {
					decrement_hours();
				}
				// Increment past max so we don't register continuous presses
				downBtnDebounceCounter++;
				xil_printf("%02d:%02d:%02d\r", hoursValue, minutesValue, secondsValue);
			}

		}

		if(currentButtonState & UP_BTN) { // If up button is pressed...
			if(upBtnDebounceCounter < DEBOUNCE_MAX) { // If not at max,
			upBtnDebounceCounter++; // Increment debounce counter
			}
			else if(upBtnDebounceCounter == DEBOUNCE_MAX) { // If the button is debounced...
				if(secondsDebounced == TRUE) {
					increment_seconds();
				}
				if(minutesDebounced == TRUE) {
					increment_minutes();
				}
				if(hoursDebounced == TRUE) {
					increment_hours();
				}
				// Increment past max so we don't register continuous presses
				upBtnDebounceCounter++;
				xil_printf("%02d:%02d:%02d\r", hoursValue, minutesValue, secondsValue);
			}
		}
	}

	else{
		// -----Advance the time-----
		// increment time counter
		secondCounter++;
		// Check for max
		if(secondCounter == SEC_COUNTER_MAX) {
			// if 1 second has passed (100 ticks) increment clock
			//===========================COMMENT HERE=====================
			if(increment_seconds()){
				if(increment_minutes()){
					increment_hours();
				}
			}
			// Update clock display
			xil_printf("%02d:%02d:%02d\r", hoursValue, minutesValue, secondsValue);

			// Reset second counter
			secondCounter = 0;
		}
	}
}


// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
  // Check which buttons are high/low
  if(!(currentButtonState & CENTER_BTN)) { // If center button is not pressed...
	  // Clear the debounce timer
	  centerBtnDebounceCounter = 0;
  }

  if(!(currentButtonState & RIGHT_BTN)) { // If right button is not pressed...
	  // Clear the debounce timer
	  rightBtnDebounceCounter = 0;
  }

  if(!(currentButtonState & DOWN_BTN)) { // If down button is not pressed...
	  // Clear the debounce timer
	  downBtnDebounceCounter = 0;
  }

  if(!(currentButtonState & LEFT_BTN)) { // If left button is not pressed...
	  // Clear the debounce timer
	  leftBtnDebounceCounter = 0;
  }

  if(!(currentButtonState & UP_BTN)) { // If up button is not pressed...
	  // Clear the debounce timer
	  upBtnDebounceCounter = 0;
  }

  XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
  XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}


// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
// Question: Why is the timer_interrupt_handler() called after ack'ing the interrupt controller
// but pb_interrupt_handler() is called before ack'ing the interrupt controller?
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		pb_interrupt_handler();
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
	}
}

int main (void) {
    init_platform();
    // Initialize the GPIO peripherals.
    int success;
    print("hello world\n\r");
    success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
    // Set the push button peripheral to be inputs.
    XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
    // Enable the global GPIO interrupt for push buttons.
    XGpio_InterruptGlobalEnable(&gpPB);
    // Enable all interrupts in the push button peripheral.
    XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

    microblaze_register_handler(interrupt_handler_dispatcher, NULL);
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
    		(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();

    while(1);  // Program never ends.

    cleanup_platform();

    return 0;
}
