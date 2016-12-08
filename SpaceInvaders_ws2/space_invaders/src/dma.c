/*****************************************************************************
* Filename:          C:\Users\superman\Documents\ecen427\SpaceInvaderTestHW2/drivers/dma_v1_00_a/src/dma.c
* Version:           1.00.a
* Description:       dma Driver Source File
* Date:              Mon Dec 05 17:13:48 2016 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "dma.h"

/************************** Function Definitions ***************************/


/**
 *
 * User logic master module to send/receive word to/from remote system memory.
 * While sending, one word is read from user logic local FIFO and written to remote system memory.
 * While receiving, one word is read from remote system memory and written to user logic local FIFO.
 *
 * @param   BaseAddress is the base address of the DMA device.
 * @param   Src/DstAddress is the destination system memory address from/to which the data will be fetched/stored.
 *
 * @return  None.
 *
 * @note    None.
 *
 */
void DMA_MasterSendWord(Xuint32 BaseAddress, Xuint32 DstAddress)
{
  /*
   * Set user logic master control register for write transfer.
   */
  Xil_Out8(BaseAddress+DMA_MST_CNTL_REG_OFFSET, MST_SGWR);

  /*
   * Set user logic master address register to drive IP2Bus_Mst_Addr signal.
   */
  Xil_Out32(BaseAddress+DMA_MST_ADDR_REG_OFFSET, DstAddress);

  /*
   * Set user logic master byte enable register to drive IP2Bus_Mst_BE signal.
   */
  Xil_Out16(BaseAddress+DMA_MST_BE_REG_OFFSET, 0xFFFF);

  /*
   * Start user logic master write transfer by writting special pattern to its go port.
   */
  Xil_Out8(BaseAddress+DMA_MST_GO_PORT_OFFSET, MST_START);
}

void DMA_MasterRecvWord(Xuint32 BaseAddress, Xuint32 SrcAddress)
{
  /*
   * Set user logic master control register for read transfer.
   */
  Xil_Out8(BaseAddress+DMA_MST_CNTL_REG_OFFSET, MST_SGRD);

  /*
   * Set user logic master address register to drive IP2Bus_Mst_Addr signal.
   */
  Xil_Out32(BaseAddress+DMA_MST_ADDR_REG_OFFSET, SrcAddress);

  /*
   * Set user logic master byte enable register to drive IP2Bus_Mst_BE signal.
   */
  Xil_Out16(BaseAddress+DMA_MST_BE_REG_OFFSET, 0xFFFF);

  /*
   * Start user logic master read transfer by writting special pattern to its go port.
   */
  Xil_Out8(BaseAddress+DMA_MST_GO_PORT_OFFSET, MST_START);
}

void DMA_go(Xuint32 BaseAddress, Xuint32 SrcAddress, Xuint32 DstAddress, Xuint32 length){
	  /*
	   * Set user logic master control register for read transfer.
	   */
	  Xil_Out8(BaseAddress+DMA_MST_CNTL_REG_OFFSET, MST_SGRD);

	  /*
	   * Set source address
	   */
	  Xil_Out32(BaseAddress+DMA_SLV_REG0_OFFSET, SrcAddress);

	  /*
	   * Set dest. address
	   */
	  Xil_Out32(BaseAddress+DMA_SLV_REG1_OFFSET, DstAddress);

	  /*
	   * Set dest. address
	   */
	  Xil_Out32(BaseAddress+DMA_SLV_REG2_OFFSET, length);

	  Xil_Out32(BaseAddress+DMA_MST_LEN_REG_OFFSET, 1);

	  /*
	   * Set user logic master byte enable register to drive IP2Bus_Mst_BE signal.
	   */
	  Xil_Out16(BaseAddress+DMA_MST_BE_REG_OFFSET, 0xFFFF);

	  /*
	   * Start user logic master read transfer by writing special pattern to its go port.
	   */
	  Xil_Out8(BaseAddress+DMA_MST_GO_PORT_OFFSET, MST_START);
}

uint32_t DMA_readReg(Xuint32 BaseAddress, Xuint32 offset){
	return Xil_In32(BaseAddress+offset);
}

