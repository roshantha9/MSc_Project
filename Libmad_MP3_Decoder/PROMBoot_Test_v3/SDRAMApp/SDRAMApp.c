#include "xparameters.h"
#include "xbasic_types.h"

extern int _start;
extern int _exception_handler;
extern int __interrupt_handler;


int main (void) {

Xuint32 data=0, tim=0;
int x;

//Output to UART specifing that code is running from SRAM
  xil_printf("\n\r-- Running from SRAM() --\n\r");

//Resets the Reset, exception and interrupt handlers
xil_printf("Resetting reset, exception and interrupt handlers \n\r");
xil_printf("based on RUN_FRAM_SRAM compiler derivatives.\n\r");
	//inserts a jump instruction at 0x0 (MicroBlaze Reset) to jump to the 
	//start of the RUN_FROM_SRAM program as specifed by the handler by _start
	// during the compilation of RUN_FROM_SRAM
	//  The compiler will create thw following warning that can be safely ignored:
	//		"warning: initialization makes integer from pointer without a cast"
	x = &_start;
	*(int*)(0x0) = 0xb0000000 | (((x-1) & 0xFFFF0000) >> 16 );
	*(int*)(0x4) = 0xb8000000 | (((x-1) & 0xFFFF));

	// inserts a jump instruction at 0x8 (MicroBlaze exception handler) to 
	// jump to the exception handler of the RUN_FROM_SRAM program as 
	// specifed by the handler by _exception_handler
	// during the compilation of RUN_FROM_SRAM
	//  The compiler will create thw following warning that can be safely ignored:
	//		"warning: initialization makes integer from pointer without a cast"
	x = &_exception_handler;
	*(int*)(0x8) = 0xb0000000 | (((x-1) & 0xFFFF0000) >> 16 );
	*(int*)(0xB) = 0xb8000000 | (((x-1) & 0xFFFF));

	// inserts a jump instruction at 0x10 (MicroBlaze interrupt handler) to 
	// jump to the interrupt handler of the RUN_FROM_SRAM program as 
	// specifed by the handler by _interrupt_handler 
	// during the compilation of RUN_FROM_SRAM	
	//  The compiler will create thw following warning that can be safely ignored:
	//		"warning: initialization makes integer from pointer without a cast"
	x = &__interrupt_handler;
	*(int*)(0x10) = 0xb0000000 | (((x-1) & 0xFFFF0000) >> 16 );
	*(int*)(0x14) = 0xb8000000 | (((x-1) & 0xFFFF));


	//Xuint32 LED_GPIOStatus;


  // Clear the screen
  xil_printf("%c[2J",27);

  
  // Hello World
  xil_printf("Hello World - this is running from the SDRAM! \r\n");
  
  
  	
  while(1){
  }


  return 0;

}
