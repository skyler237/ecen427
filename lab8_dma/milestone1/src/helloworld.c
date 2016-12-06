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

void print(char *str);

int main()
{
	uint32_t source_word = 0xDEADBEEF;
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

//    cleanup_platform();

    return 0;
}
