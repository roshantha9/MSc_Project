/** Xilinx dpecific defines **/
#include "xparameters.h"
#include "xbasic_types.h"
#include "xtmrctr.h"
#include "xspi.h"
#include <xintc_l.h>
#include "xio.h"

/** local custom defines **/
#include "mad.h"
//#include "speech_sample_24KHz_64kbps.h"
#include "speech_sample_16KHz.h"
//#include "chilll_32Kbps_16Khz.h"

#include "COMMON.h"
#include "DAC.h"
#include "autmr.h"
#include "SDRAM.h"


#define TIMER_COUNTER_1 		1
#define SDRAM_DATA_START_LOC	(MEM_BASEADDR + 0x00100000)

#define SAMPLE_RATE	16000


XTmrCtr XPS_Timer0;
Xuint32 intDevStatus;
XSpi mySPI;

static int decode(unsigned char const *, unsigned long);

Xuint32 intSDRAMAddrOffset=0;
Xuint32 intDataRemaining=0;

void pause(void){
	Xuint32 count;	
    count=0x0000FFFF;
	while (count--) asm volatile ("nop");	
}



// *****************************************************************************
void timer_int_handler(void * baseaddr_p) {
	/* Add variable declarations here */
	unsigned int csr;
	
	// wav specific vars
    Xuint8 aaa=0;
	Xuint8 bbb=0;
	Xuint8 j=0;
    Xuint16 intCombinedSamples=0;
	
	static Xuint32 addr = SDRAM_DATA_START_LOC;
	
	// Read timer 0 CSR to see if it raised the interrupt 
	csr = XTmrCtr_mGetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR,0);	
	
	// If the interrupt occured, then increment a counter and set one_second_flag 
	if (csr & XTC_CSR_INT_OCCURED_MASK)
	{		
		// if zero is reached - it means finished playing song
		if(intDataRemaining > 0){			
				
			aaa=XIo_In8(addr);	// LSBs
			addr++;
			bbb=XIo_In8(addr);	// MSBs
			addr++;			
	
			intCombinedSamples = aaa | (bbb << 8);
			
			//intCombinedSamples = bbb | (aaa << 8);
			intCombinedSamples = intCombinedSamples + 32768;			

			intDataRemaining=intDataRemaining-2;	// decrement remaining counter		
			
			//xil_printf("(%08X) = %04x\r\n",addr, intCombinedSamples); 
			xil_printf("%04x ", intCombinedSamples); 
			
			DAC_SendDataToBothChannel(intCombinedSamples,intCombinedSamples);
			
		}
		else{
			xil_printf("Finished Playing whole song \r\n");
			AuTmr_StopTimer();
			DAC_PowerDownDAC();	

			addr=SDRAM_DATA_START_LOC;
			intDataRemaining=intSDRAMAddrOffset;
			
			pause();
			
			AuTmr_StartTimer(SAMPLE_RATE);
			
		}	
	}
	
	/* Clear the timer interrupt */
	XTmrCtr_mSetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR,0,csr);	
}



int main (void) {

  Xuint32 intInitialTimerVal=0;
  Xuint32 intCurTimerVal=0;
  Xuint32 intCaliberationVal=0;
  Xuint32 intDecoderRunTime=0;

  /***************************/
  microblaze_disable_icache();
  microblaze_init_icache_range(0,XPAR_MICROBLAZE_0_CACHE_BYTE_SIZE);
  microblaze_enable_icache();

  microblaze_disable_dcache();  
  microblaze_init_dcache_range(0,XPAR_MICROBLAZE_0_DCACHE_BYTE_SIZE);
  microblaze_enable_dcache();
  /***************************/
  
  // Clear the screen
  xil_printf("%c[2J",27);

  xil_printf("***** LIBMAD Cache TEST v2 ******\r\n");
  xil_printf("*****************************\r\n");

  xil_printf("Main::sizeof(speech_sample_24KHz_64kbps = %d\r\n",sizeof(speech_sample_16KHz));
  xil_printf("Main::(void *)speech_sample_24KHz_64kbps = 0x%X\r\n",(void *)speech_sample_16KHz);

  xil_printf("Main::going to decode now..\r\n");

  xil_printf("Main:: Initialise Timer \r\n");
  
  /************************************************
   *	Initialise timer
   ************************************************/
   //Initialize and configuring the timer
   intDevStatus = XTmrCtr_Initialize(&XPS_Timer0, XPAR_TIMER_1_DEVICE_ID);
            if ( intDevStatus != XST_SUCCESS ) {
                    xil_printf("XTmrCtr_Initialize() failed.\r\n");
            }
   
   //Set options to the Control register and enable the timer
   XTmrCtr_SetOptions(&XPS_Timer0, TIMER_COUNTER_1, XTC_ENABLE_ALL_OPTION | XTC_AUTO_RELOAD_OPTION);
   
   //Read TCR register             
   intInitialTimerVal = XTmrCtr_GetValue(&XPS_Timer0, TIMER_COUNTER_1);
   //intCurTimerVal = XTmrCtr_GetValue(&XPS_Timer0, TIMER_COUNTER_1);
   //intCaliberationVal = intCurTimerVal - intInitialTimerVal;
   xil_printf("Decoding Started - Initial Timer value is %d \r\n", intInitialTimerVal);  
      
    //Start timer
    XTmrCtr_Start(&XPS_Timer0, TIMER_COUNTER_1);
  
   xil_printf("====================================================\r\n");
   decode((void *)speech_sample_16KHz, sizeof(speech_sample_16KHz));
   xil_printf("\r\n====================================================\r\n");
   
   //Read the TCR register             
   intCurTimerVal = XTmrCtr_GetValue(&XPS_Timer0, TIMER_COUNTER_1);            
   intDecoderRunTime = intCurTimerVal - intInitialTimerVal;
   xil_printf("Decoding Finished - Total decoding time is %d \r\n", intDecoderRunTime);

   
   /************************************************
   *	Finished Decoding - now start playing
   ************************************************/
   intDataRemaining = intSDRAMAddrOffset;
   
   xil_printf("File Size in SDRAM:  intSDRAMAddrOffset = %08x\r\n", intSDRAMAddrOffset);
   
   DAC_Initialise();
   DAC_PowerDownDAC();	
   
	/* now start playing the sound */
	/* Register the Timer interrupt handler in the vector table */
    XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
                             XPAR_XPS_INTC_0_XPS_TIMER_0_INTERRUPT_INTR,
                             (XInterruptHandler) timer_int_handler,
                             (void *)XPAR_XPS_TIMER_0_BASEADDR);

   /* Start the interrupt controller */
   XIntc_mMasterEnable(XPAR_XPS_INTC_0_BASEADDR);    
   
   /* Enable MB interrupts */
   microblaze_enable_interrupts();		  
   xil_printf("AuTmr_StartTimer : Now output to DAC\r\n");
   AuTmr_StartTimer(SAMPLE_RATE);
      
   

  while(1){
    // do nothing for now
    }

  return 0;
}


/*
 * This is a private message structure. A generic pointer to this structure
 * is passed to each of the callback functions. Put here any data you need
 * to access from within the callbacks.
 */

struct buffer {
  unsigned char const *start;
  unsigned long length;
};

/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */

static enum mad_flow input(void *data, struct mad_stream *stream){
  struct buffer *buffer = data;

  #if X_DEBUG
    xil_printf("input::Enter\r\n");
  #endif

  if (!buffer->length)
    return MAD_FLOW_STOP;

  mad_stream_buffer(stream, buffer->start, buffer->length);

  buffer->length = 0;

  return MAD_FLOW_CONTINUE;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

static inline signed int scale(mad_fixed_t sample){

  #if X_DEBUG
   xil_printf("scale::Enter\r\n");
  #endif

  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

static enum mad_flow output(void *data,
		     struct mad_header const *header,
		     struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples;
  mad_fixed_t const *left_ch, *right_ch;

  #if X_DEBUG
   xil_printf("output::Enter\r\n");
  #endif

  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];

  while (nsamples--) {
    signed int sample;

    /* output sample(s) in 16-bit signed little-endian PCM */

    sample = scale(*left_ch++);
    //putchar((sample >> 0) & 0xff);
    //putchar((sample >> 8) & 0xff);

	 //xil_printf("%02X",(sample >> 0) & 0xff);
	 //xil_printf("%02X",(sample >> 8) & 0xff);
	 
	 /* write the values to SDRAM */
	 XIo_Out8(SDRAM_DATA_START_LOC+intSDRAMAddrOffset, (sample>>0)&0xFF);
	 intSDRAMAddrOffset++;
	 XIo_Out8(SDRAM_DATA_START_LOC+intSDRAMAddrOffset, (sample>>8)&0xFF);	 
	 intSDRAMAddrOffset++;
	 
	 
    if (nchannels == 2) {
      sample = scale(*right_ch++);
      //putchar((sample >> 0) & 0xff);
      //putchar((sample >> 8) & 0xff);

		//xil_printf("%02X",(sample >> 0) & 0xff);
		//xil_printf("%02X",(sample >> 8) & 0xff);
    }
  }

  return MAD_FLOW_CONTINUE;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */

static enum mad_flow error(void *data,
		    struct mad_stream *stream,
		    struct mad_frame *frame)
{
  struct buffer *buffer = data;
    #if X_DEBUG
        xil_printf("error::Enter\r\n");
    #endif

  xil_printf("decoding error 0x%04x (%s) at byte offset %u\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - buffer->start);

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}

/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */

static int decode(unsigned char const *start, unsigned long length)
{
  struct buffer buffer;
  struct mad_decoder decoder;
  int result;


    #if X_DEBUG
        xil_printf("decode::Enter\r\n");
    #endif

  /* initialize our private message structure */

  buffer.start  = start;
  buffer.length = length;

  /* configure input, output, and error functions */

  mad_decoder_init(&decoder, &buffer,
		   input, 0 /* header */, 0 /* filter */, output,
		   error, 0 /* message */);

  /* start decoding */

  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */

  mad_decoder_finish(&decoder);

  return result;
}


