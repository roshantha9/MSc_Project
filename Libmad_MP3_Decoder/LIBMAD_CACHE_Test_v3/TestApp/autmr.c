#include "xparameters.h"
#include "xbasic_types.h"
#include "DAC.h"
#include "xstatus.h"
#include <xtmrctr.h>
#include <xintc_l.h>



void AuTmr_StartTimer(Xuint32 SampleRate){
  
  xil_printf("AuTmr_StartTimer::enter \r\n");
  
  XIntc_mEnableIntr(XPAR_XPS_INTC_0_BASEADDR, XPAR_XPS_TIMER_0_INTERRUPT_MASK);
  XTmrCtr_mSetLoadReg(XPAR_XPS_TIMER_0_BASEADDR, 0, 66000000/SampleRate); 

  /* Reset the timers, and clear interrupts */
  XTmrCtr_mSetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR, 0, XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK );
	
  /* Enable timer interrupts in the interrupt controller */
  //XIntc_mEnableIntr(XPAR_XPS_TIMER_0_BASEADDR, XPAR_XPS_TIMER_0_INTERRUPT_MASK);
  
  /* Start the timers */
  XTmrCtr_mSetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR, 0, XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_ENABLE_INT_MASK | 
						XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK);  
   
}

void AuTmr_StopTimer(){
XIntc_mDisableIntr(XPAR_XPS_INTC_0_BASEADDR, XPAR_XPS_TIMER_0_INTERRUPT_MASK);
}
