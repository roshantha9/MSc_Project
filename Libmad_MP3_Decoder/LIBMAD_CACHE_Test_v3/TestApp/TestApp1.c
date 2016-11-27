#include "xparameters.h"
#include "xbasic_types.h"
#include "xspi.h"
#include <xtmrctr.h>
#include <xintc_l.h>
#include "DAC.h"
#include "xio.h"
#include "wave.h"
#include "autmr.h"
#include <xuartlite.h>
#include "xmpmc.h"
#include "SDRAM.h"
#include "sd.h"
#include "adpcm.h"


XSpi mySPI;

/* global used by main file */
static Xuint8 intCurrSongIndex=0;
static Xuint8 intCurrSongFType=0;

// supported file types
#define MAIN_SONG_TYPE_WAV			1
#define MAIN_SONG_TYPE_ADP			2
#define MAIN_SONG_TYPE_MP3			3
#define MAIN_SONG_TYPE_UNSUPP 		10

/* global used by WAV decoder */
//Xuint8 gjb[100]={0};
Xuint32 addr=MEM_BASEADDR;
Xuint32 DataLeft=0;

/* function prototypes */
void MAIN_Pause(void);
void timer_int_handler(void * baseaddr_p);
void MAIN_PrintFiles(void);
void MAIN_GetCurrFileType(Xuint8 intFID);

// *****************************************************************************
void MAIN_Pause(void){
	
	Xuint32 count;	
    count=0x00FFFFFF;
	while (count--) asm volatile ("nop");
}


void MAIN_PrintFiles(void){
	
	int i=0;
	
	for(i=0;i<4;i++){	
		
		xil_printf("SD_file[%d].Filename is: %x %x %x %x %x %x %x %x \r\n",i,\
		SD_file[i].Filename[0],SD_file[i].Filename[1],SD_file[i].Filename[2],\
		SD_file[i].Filename[3],SD_file[i].Filename[4],SD_file[i].Filename[5],\
		SD_file[i].Filename[6],SD_file[i].Filename[7]);
		
		xil_printf("SD_file[%d].Filetype is: %x %x %x\r\n",i,SD_file[i].Filetype[0],\
					SD_file[i].Filetype[1],SD_file[i].Filetype[2]);			
		
		xil_printf("\r\n");			
	}
}


void MAIN_GetCurrFileType(Xuint8 intFID){		
	
	/* wav - wave file */
	if( (SD_file[intFID].Filetype[0] == 0x57) &&
	    (SD_file[intFID].Filetype[1] == 0x41) &&
	    (SD_file[intFID].Filetype[2] == 0x56) ){
		
		intCurrSongFType = MAIN_SONG_TYPE_WAV;	
		xil_printf("MAIN_GetCurrFileType:: WAV!\r\n");
	}
	else
	/* adp - ADPCM sound file */
	if( (SD_file[intFID].Filetype[0] == 0x41) &&
	    (SD_file[intFID].Filetype[1] == 0x44) &&
	    (SD_file[intFID].Filetype[2] == 0x50) ){
		
		intCurrSongFType = MAIN_SONG_TYPE_ADP;	
		xil_printf("MAIN_GetCurrFileType:: ADPCM!\r\n");
	}
	else{
		intCurrSongFType = MAIN_SONG_TYPE_UNSUPP;	
		xil_printf("MAIN_GetCurrFileType:: Unsupported!\r\n");		
		
		xil_printf("SD_file[%d].Filetype is: %x %x %x\r\n",intFID,SD_file[intFID].Filetype[0],\
					SD_file[intFID].Filetype[1],SD_file[intFID].Filetype[2]);			
		
	}
		
	//xil_printf("MAIN_GetCurrFileType:: return - %d\r\n", intCurrSongFType);
}

// *****************************************************************************
void timer_int_handler(void * baseaddr_p) {
	/* Add variable declarations here */
	unsigned int csr;
	
	// wav specific vars
    Xuint8 aaa,bbb,j;
    Xuint16 intCombinedSamples,temp,ChannelA,ChannelB;
	
	// Read timer 0 CSR to see if it raised the interrupt 
	csr = XTmrCtr_mGetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR,0);
	// If the interrupt occured, then increment a counter and set one_second_flag 
	if (csr & XTC_CSR_INT_OCCURED_MASK)
	{
		/* handle situation of - WAV file */
		if(intCurrSongFType == MAIN_SONG_TYPE_WAV){
			if (DataLeft != 0){			
				addr=Wav_PlayPerSample(addr);
				DataLeft=DataLeft-BlockAlign;
				//xil_printf( " addr      = %06x\r\n", addr );
				//xil_printf( " DataLeft      = %06d\r\n", DataLeft );
			}
			else{
				addr=MEM_BASEADDR;
				DataLeft=WaveSize-8000;
				//DataLeft=WaveSize;
			}
		}
		/* handle situation of - ADPCM file */
		else
		if(intCurrSongFType == MAIN_SONG_TYPE_ADP){
			//xil_printf("timer_int_handler:: \r\n");				
			ADPCM_PlayOut();
		}
		else{
			xil_printf("timer_int_handler:: Unknown File Type !! \r\n");
		}
	}
	
	/* Clear the timer interrupt */
	XTmrCtr_mSetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR,0,csr);	
}





int main (void) {

	Xuint8 intInData;
	Xuint8 intOutData;

	const u32 StartAdr = MEM_BASEADDR + MEM_OFFSET;
	const u32 Range = MEM_SIZE - MEM_OFFSET;
	u32 MemError = 0;
	Xuint32 status,i;
	XUartLite UartLite;
	
	// Clear the screen
	xil_printf("%c[2J",27);
	
	/* hardcode the current song index */
	intCurrSongIndex=11;		//4	//2
	
	DAC_Initialise();
 
	sd_spi_ini();

	sdcard_power();
	if (sdcard_init()==0x01){
		xil_printf("SD card in SPI MOD_BYTEe!\r\n"); 
	if(block_length()==0x01)
		xil_printf("block length set success!\r\n"); 
	
		read_phsical();	
		read_boot();
		read_fdt();		
   }   
   
   /* get file type of chosen song */     
   begin_read_file(intCurrSongIndex);
   //begin_read_file(5);
      
   if (shift_fat()==0x01){
		xil_printf("transmit finish!!!\r\n");
   }   
   
   /* set current song type */
   MAIN_GetCurrFileType(intCurrSongIndex);
   
   /* Register the Timer interrupt handler in the vector table */
   XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
                             XPAR_XPS_INTC_0_XPS_TIMER_0_INTERRUPT_INTR,
                             (XInterruptHandler) timer_int_handler,
                             (void *)XPAR_XPS_TIMER_0_BASEADDR);
	
	xil_printf("--- Going to play the wav \r\n");

	/* Start the interrupt controller */
	XIntc_mMasterEnable(XPAR_XPS_INTC_0_BASEADDR);    
	
	/* Enable MB interrupts */
	microblaze_disable_interrupts();	

	/* check the wav header - if the file type is wave */
	if(intCurrSongFType == MAIN_SONG_TYPE_WAV){
		status=Wav_CheckHeader(MEM_BASEADDR);
	}else{
		/* do nothing - probably an adpcm file */
		status=XST_SUCCESS;
		xil_printf("---> File is NOT a WAV file !!\r\n");
	}		
	
	if (status != XST_SUCCESS){
		xil_printf("Wav header check failed!\r\n");
		return XST_FAILURE;
	}else xil_printf("Wav header check seccuss!\r\n");
	
	/* start stop timer accordingly based on correct filetype */
	/* the sample rate for ADPCM is FIXED at 8Khz */
	if(intCurrSongFType == MAIN_SONG_TYPE_WAV){
		
		/* Enable MB interrupts */
		microblaze_enable_interrupts();
		
		DataLeft=WaveSize;//-1000;	
		xil_printf( " WAV SampleRate      = %06d\r\n", SampleRate );
		AuTmr_StartTimer(SampleRate);
		xil_printf("AuTmr_StartTimer!\r\n");
	}
	else
	if(intCurrSongFType == MAIN_SONG_TYPE_ADP){
		xil_printf( " ADPCM SampleRate      = %06d\r\n", ADPCM_FIXED_SAMPLERATE );
		//AuTmr_StartTimer(ADPCM_FIXED_SAMPLERATE);	// fixed sample rate for ADPCM
		xil_printf("AuTmr_StartTimer!\r\n");		
		
		// feed sdram offset, start address to adpcm
		ADPCM_Initialise(sdcard_get_sdram_offset(), MEM_BASEADDR);
		
		// check the adpcm header
		ADPCM_CheckADP_header();
		
		// this will keep looping until song has finished decoding+playing
		ADPCM_MainLoop(); 		
		
		xil_printf("Finished ADPCM file playing !!\r\n");
	}
	else{
		xil_printf("Cannot play song - unsupported format, exiting");
		return XST_FAILURE;
	}
		
	
	// continuously loop
	while(1){
	
	}

  return 0;

}
