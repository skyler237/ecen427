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

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include <stdint.h>
#include "dma.h"
#include "platform.h"
#include "xparameters.h"
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.

#define ARRAY_LENGTH 10
uint32_t source[ARRAY_LENGTH];
uint32_t destination[ARRAY_LENGTH];
static uint8_t interrupted = 0;

void print(char *str);

void dma_interrupt_handler() {
	xil_printf("Interrupt\n\r");
	interrupted = 1;
}

void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);

	 // Check the PIT interrupt first.
	if (intc_status & XPAR_DMA_0_MYINTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_DMA_0_MYINTERRUPT_MASK);
		dma_interrupt_handler();
	}

}

int main()
{
	/*uint32_t source_word = 0xDEADBEEF;
	uint32_t destination_word = 0x0;

    init_platform();

    xil_printf("Hello World\n\r");
    cleanup_platform();
    xil_printf("Printing value before DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

    DMA_MasterRecvWord(XPAR_DMA_0_BASEADDR, (Xuint32) &source_word);
    DMA_MasterSendWord(XPAR_DMA_0_BASEADDR, (Xuint32) &destination_word);

    xil_printf("Printing value after DMA transfer.\n\r");
    xil_printf("%x\r\n", destination_word);

    cleanup_platform();

    return 0;*/
	 // Set up interrupts

	init_platform();

	 microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	 XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_DMA_0_MYINTERRUPT_MASK));
	 XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	 xil_printf("start\n\r");
	 microblaze_enable_interrupts();
	uint32_t i;
	for(i = 0; i < ARRAY_LENGTH; i++){
		source[i] = i+1;
		destination[i] = 0;
	}

	if(interrupted){
		xil_printf("Interrupt Early\n\r");
	}
	DMA_go(XPAR_DMA_0_BASEADDR, source, destination, ARRAY_LENGTH);
	xil_printf("Control: %x\n\r", DMA_readReg(XPAR_DMA_0_BASEADDR, DMA_MST_CNTL_REG_OFFSET));
	xil_printf("SourceAddr: %x\n\r", DMA_readReg(XPAR_DMA_0_BASEADDR, DMA_SLV_REG0_OFFSET));
	xil_printf("DestAddr: %x\n\r", DMA_readReg(XPAR_DMA_0_BASEADDR, DMA_SLV_REG1_OFFSET));
	xil_printf("Length: %x\n\r", DMA_readReg(XPAR_DMA_0_BASEADDR, DMA_SLV_REG2_OFFSET));
	xil_printf("%x\n\r", destination);

	while(!interrupted);
	xil_printf("DMA Transfer Complete \n\r");
	xil_printf("Source: \n\r");
	for(i = 0; i < ARRAY_LENGTH; i++){
		xil_printf("%d\n\r", source[i]);
	}

	xil_printf("Destination: \n\r");
	for(i = 0; i < ARRAY_LENGTH; i++){
		xil_printf("%d\n\r", destination[i]);
	}

	return 0;
}
