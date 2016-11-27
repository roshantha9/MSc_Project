/******************************************************************************
 * Name : PROM Boot-loader
 * FileName: promread.c
 * Author : Rosh Mendis 
 * Date Created : 10/08/2011
 * Description : Contains functions to:
 *				 - Initialise GPIO
 *				 - Read from PROM (bit-toggling)
 *				(Code adapted from XAPP482 - Xilinx Inc)
 *
 ******************************************************************************/ 
#include "xparameters.h"
#include "xgpio.h"
#include "xbasic_types.h"
#include <promread.h>
#include "xio.h"

/* global vars */
XGpio GpioInputPROMREAD_DIN;	// Data in GPIO
XGpio GpioOutputPROMREAD_CNTRL;	// PROM2 control GPIO
Xuint32 intSDRAMAddr = XPAR_MPMC_0_MPMC_BASEADDR;	// base address of SDRAM

/*****************************************************************************
* Function name    :  pause (void) 
* returns          :  void
* parameters	   :  void
* Description      :  Stalls processor for some time
*****************************************************************************/
void pause(void){
	Xuint32 count;	
   count=0x00FFFF;
	while (count--) asm volatile ("nop");
	
}

/*****************************************************************************
* Function name    :  PROMREAD_GetAddrPointer (void) 
* returns          :  Xuint32
* parameters	   :  void
* Description      :  returns SDRAM end address (getter)
*****************************************************************************/
Xuint32 PROMREAD_GetAddrPointer(){
	return intSDRAMAddr;
}

/*****************************************************************************
* Function name    :  read_helper (void) 
* returns          :  Void
* parameters	   :  data read word size
* Description      :  Reads Platform FLASH  device serially 32 time and puts
*					  it into 32-bit word                   
*****************************************************************************/
Xuint32 prom_read_helper( Xuint8 num)
{
	Xuint8 i;
	Xuint32 data32=0;

	for( i=0;i<num;i++){
				
		//single bit is shifted into the 32-bit word. 
		data32 = (data32 << 1) + XGpio_DiscreteRead(&GpioInputPROMREAD_DIN, 1);	
	
		//clock the PROM to output data - (toggle CCLK)		
		XGpio_DiscreteWrite(&GpioOutputPROMREAD_CNTRL, 1, OE_HIGH | CCLK_HIGH | CE_LOW);		
		XGpio_DiscreteWrite(&GpioOutputPROMREAD_CNTRL, 1, OE_HIGH | CCLK_LOW | CE_LOW); 	
	}

	return data32;	
}

/*****************************************************************************
* Function name    :  promread_init(void)
* returns          :  int (success/fail)
* parameters	   :  None
* Description      :  Initialises the GPIO IP Cores
*****************************************************************************/
int promread_init(void){
	
	
	Xuint32 PROMREAD_DIN_status;
	Xuint32 PROMREAD_CNTRL_status;
	
	//----------------------------------------------------
	// INITIALISE PROM DIN Connections (GPIO) - DIN
	//----------------------------------------------------	
	
	xil_printf("  - Setting up the PROMREAD-DIN connections\r\n");
	
	// Initialize the GPIO driver
	PROMREAD_DIN_status = XGpio_Initialize(&GpioInputPROMREAD_DIN,
							XPAR_PROMREAD_DIN_GPIO_DEVICE_ID);
				
	if (PROMREAD_DIN_status != XST_SUCCESS){
	xil_printf("ERROR: PROMREAD_DIN failed to Initialise\r\n");
	return XST_FAILURE;
	}

	// Set the direction for all signals to be input
	XGpio_SetDataDirection(&GpioInputPROMREAD_DIN, 1, 0x1);	
	
	//----------------------------------------------------
	// INITIALISE PROM CNTRL Connections (GPIO) - CCLK, CE, OE
	//----------------------------------------------------	
	
	xil_printf("  - Setting up the PROMREAD-CNTRL connections\r\n");
	
	// Initialize the GPIO driver
	PROMREAD_CNTRL_status = XGpio_Initialize(&GpioOutputPROMREAD_CNTRL,
							XPAR_PROMREAD_CNTRL_GPIO_DEVICE_ID);
				
	if (PROMREAD_CNTRL_status != XST_SUCCESS){
	xil_printf("ERROR: PROMREAD_CNTRL failed to Initialise\r\n");
	return XST_FAILURE;
	}

	// Set the direction for all signals to be outputs
	XGpio_SetDataDirection(&GpioOutputPROMREAD_CNTRL, 1, 0x0);		
	
	//sets the INIT and CCLK bits high	(3 bits)
	XGpio_DiscreteWrite(&GpioOutputPROMREAD_CNTRL, 1, (OE_HIGH | CCLK_LOW | CE_LOW));

	return 0;	

}

/*****************************************************************************
* Function name    :  promread_start()
* returns          :  int
* Created by       :  HRM
* Date Created     :  21/07/2011
* Description      :  Start reading and reporting the readout from prom
*****************************************************************************/
int promread_start(void){
	
	Xuint32 data 	= 0;
	Xuint32 count	= 0;
	
	pause();
	
	xil_printf("promread_start:: enter\r\n");
	
	/* search prom until APPLICATION market is found */
	while(data != ADDRSYNC){
		data = prom_read_helper(32);		
		//xil_printf("%08X ", data);		
	}
	
	xil_printf("addrsync found!\r\n");
	xil_printf("now getting application data!\r\n");
	
	/* copy remaining data into SDRAM */
	while(data != END_PROM){	// read until END Marker
		data = prom_read_helper(32);		
		//xil_printf("%08X ", data);		
		
		XIo_Out32(intSDRAMAddr, data);		// write out data to SDRAM	
		
		/* for every write check the read */
		if(XIo_In32(intSDRAMAddr) != data){			
			xil_printf("SDRAM write unsuccesful!\r\n");
			return 0;		
		}
		intSDRAMAddr = intSDRAMAddr+4; // increment sdram address pointer
	}
	
	xil_printf("Finished copying data to SDRAM, end addr = 0x%08X\r\n", intSDRAMAddr);	

	return 0;
}


