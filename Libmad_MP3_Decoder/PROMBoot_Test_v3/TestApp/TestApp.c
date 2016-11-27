/******************************************************************************
 * Name : PROM Boot-loader (Main Application)
 * FileName: TeatApp.c
 * Author : Rosh Mendis 
 * Date Created : 10/08/2011
 * Description : First Disables PROM-1, then starts copying to SDRAM from PROM2, 
 *				 Then jump to start of SDRAM
 ******************************************************************************/ 
#include "xparameters.h"
#include "xio.h"
#include "xbasic_types.h"
#include "xgpio.h"
#include "xmpmc.h"
#include "promread.h"


// GPIO to disable PROM-1
XGpio GpioOutputPROM1;

/* program counter setting */
#define PROG_START_ADDR 	XPAR_MPMC_0_MPMC_BASEADDR
// Function point that is used at the end of the program to jump to the
// address location stated by PROG_START_ADDR
int (*func_ptr) ();


int main (void) {


	Xuint32 LED_GPIOStatus;
	Xuint32 PROM1_GPIOStatus;	

	// Clear the screen
	xil_printf("%c[2J",27);

	// Hello World
	xil_printf("** PROM READ TEST - v2.0 **\r\n");
	xil_printf("***************************\r\n");
	
	//----------------------------------------------------
	// Set the OE of the PROM-1 to LOW
	//----------------------------------------------------
	// Initialize the GPIO driver
	PROM1_GPIOStatus = XGpio_Initialize(&GpioOutputPROM1,
					XPAR_PROM1_OE_DEVICE_ID);
				
	if (PROM1_GPIOStatus != XST_SUCCESS){
	xil_printf("ERROR: PROM1_GPIOStatus failed to initialise\r\n");
	return XST_FAILURE;
	}

	// Set the direction for all signals to be outputs
	XGpio_SetDataDirection(&GpioOutputPROM1, 1, 0x0);

	// set the OE pin low
	XGpio_DiscreteWrite(&GpioOutputPROM1, 1, 0x0);	
	
	//----------------------------------------------------
	// INITIALISE PROMREAD-2
	//----------------------------------------------------		
	promread_init();	
	
	//----------------------------------------------------
	// START READING FROM PROM
	//----------------------------------------------------	
	promread_start();

	
	xil_printf("------------->> Finished reading/writing !\r\n");
	
	xil_printf("------------->> changing program counter and jumping !\r\n");
	
	// function point that is set to point to the address of PROG_START_ADDR
	func_ptr = PROG_START_ADDR;
	// jump to start execution code at the address PROG_START_ADDR
	func_ptr();
	
	xil_printf("------------->> Oops jump didn't work !\r\n");		
	
  while(1){
  }
  return 0;

}
