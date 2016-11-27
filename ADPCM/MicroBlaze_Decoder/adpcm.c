/*****************************************************************
 * Name   	   : ADPCM - Decoder
 * Modified by : Rosh Mendis
 * Revisions   : v2.0
 * Description : Original Algorithm taken from Stichting Mathematisch 
 *				 Centrum, Amsterdam. 
 *				 Added:
 *				 - Getters, Setters for globals
 *				 - Added Header support
 * 				 - Added support for Rewind/FF
 *				 - Added Volume control
 *				 - Added Get Progress of song
 ****************************************************************/

/***********************************************************
Copyright 1992 by Stichting Mathematisch Centrum, Amsterdam, The
Netherlands.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Stichting Mathematisch
Centrum or CWI not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior permission.

STICHTING MATHEMATISCH CENTRUM DISCLAIMS ALL WARRANTIES WITH REGARD TO
THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH CENTRUM BE LIABLE
FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

******************************************************************/
/*
** Intel/DVI ADPCM coder/decoder.
**
** The algorithm for this coder was taken from the IMA Compatability Project
** proceedings, Vol 2, Number 2; May 1992.
**
** Version 1.0, 7-Jul-92.
*/

/* xilinx includes */
#include "xio.h"
#include "xstatus.h"

/* local application includes */
#include "adpcm.h"
#include "DAC.h"
#include "autmr.h"


#ifndef __STDC__
#define signed
#endif

// this is the multiplication format (fast/slow mult)
#define NODIVMUL


/* globals used by ADPCM decoder */
static struct adpcm_state state;
static char   inbuf[ADPCM_NSAMPLES/2]={0};
static short  outbuf[ADPCM_NSAMPLES]={0};
static int    intADPCMSampleCount=0;
static int    intOutBuffIndex=0;
static int    intInBuffIndex=0;
static int 	  intTotalFileSize=0;
static int 	  intFileStartAddr=0;
static int 	  intStopAddr=0;
static int 	  intSampleRate=0;
static int 	  intNumChannels=0;
static int 	  intBitsPerSample=0;
static Xuint8 intVolume=15;


/* local prototypes */
void ADPCM_Pause(void);


/* Intel ADPCM step variation table */
static long indexTable[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8,
};

// stepsize table used to quantize the difference of the Original vs. Predicted
// sample value. Hence max value is = (2^16)-1
static long stepsizeTable[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

/*******************************************************************
 * @Name	: ADPCM_Decoder
 * @Desc	: Adaptive Differential Pulse Code Modulation-Decoder
 * @Param	: char indata[]	- input data buffer
 *			  short outdata[] - output data buffer
 *			  int len - size of input buffer
 *			  struct adpcm_state *state - previous state info
 * @Return 	: None
 *******************************************************************/
void ADPCM_Decoder(char indata[], short outdata[], int len, struct adpcm_state *state){
    signed char *inp;          /* Input buffer pointer */
    short *outp;               /* output buffer pointer */
    long sign;                  /* Current adpcm sign bit */
    long delta;                 /* Current adpcm output value (difference value) */
    long step;                  /* Stepsize */
    long valprev;               /* virtual previous output value */
    long vpdiff;                /* Current change to valprev */
    long index;                 /* Current step change index */
    long inputbuffer;           /* place to keep next 4-bit value */
    long bufferstep;            /* toggle between inputbuffer/input */

    outp = outdata;
    inp = (signed char *)indata;

    valprev = state->valprev;
    index = state->index;
    step = stepsizeTable[index];

    bufferstep = 0;

    len *=2;

    for ( ; len > 0 ; len-- ) {

       /* Step 1 - get the delta value and compute next index */
       if ( bufferstep ) {
           delta = inputbuffer & 0xf;			// get MSB 4 bits
       } else {
           inputbuffer = *inp++;				// get next data value (8bit)
           delta = (inputbuffer >> 4) & 0xf;	// get LSB 4 bits
       }
       bufferstep = !bufferstep;	// invert the variable

        /* Step 2 - Find new index value (for later) */
        index += indexTable[delta];
		// index must lie between 0 and 88 (else buffer overflow!)
        if ( index < 0 ){
            index = 0;
        }
        else if ( index > 88 ){
            index = 88;
        }

       /* Step 3 - Separate sign and magnitude */
       sign = delta & 8;		// 8 = 1000		(get MSB bit)
       delta = delta & 7;		// 7 = 0111		(get LSB 3 bits)

       /* Step 4 - update output value */
	   // check what multiplication method to use (fast/slow)
#ifdef NODIVMUL
		// fast multiplication (shifting)
        vpdiff = 0;
        if ( delta & 4 ){
            vpdiff  = (step << 2);
        }
        if ( delta & 2 ){
            vpdiff += (step << 1);
        }
        if ( delta & 1 ){
            vpdiff += step;
        }
        vpdiff >>= 2;
#else
        // slow multiplication (expensive)
		vpdiff = (delta*step) >> 2;
#endif
       
	   if ( sign )
         valprev -= vpdiff; // subtract the difference if sign is 1 (negative)
       else
         valprev += vpdiff;	// add back the difference if sign is 0 (positive)

       /* Step 5 - clamp output value */
       if ( valprev > 32767 )
         valprev = 32767;
       else if ( valprev < -32768 )
         valprev = -32768;

       /* Step 6 - Update step value */
       step = stepsizeTable[index];

       /* Step 7 - Output value */
       *outp++ = valprev;
    }
	
	// save state info for next block
    state->valprev = valprev;
    state->index = index;
}

/*******************************************************************
 * @Name	: ADPCM_CheckADP_header
 * @Desc	: Checks the ADPCM file header, and returns false if
 *			  'ADPCM' tag is not found. If true, extracts SampleRate,
 *			  NumChannels, BitsPerSample
 * @Param	: None
 * @Return 	: True/False
 *******************************************************************/
Xuint8 ADPCM_CheckADP_header(void){
	
	int ix=0;
	
	ix=intInBuffIndex;    
	
	// check if file has the 'ADPCM' tag
    if ((XIo_In8(ix) == 0x41) && (XIo_In8(ix+1) == 0x44) &&
        (XIo_In8(ix+2) == 0x50) && (XIo_In8(ix+3) == 0x43) &&
        (XIo_In8(ix+4) == 0x4D)){
	
           intNumChannels = XIo_In8(ix+5);
           xil_printf("\r\n");
           xil_printf("ADPCM_CheckADP_header:: intNumChannels = %d\r\n", intNumChannels);

           intSampleRate = (Xuint32)XIo_In8(ix+6)+((Xuint32)XIo_In8(ix+7)<<8) +\
						  ((Xuint32)XIo_In8(ix+8)<<16) + ((Xuint32)XIo_In8(ix+9)<<24);
						  
           xil_printf("ADPCM_CheckADP_header:: intSampleRate = %d\r\n", intSampleRate);

           intBitsPerSample = (Xuint32)XIo_In8(ix+10)+((Xuint32)XIo_In8(ix+11)<<8);
           xil_printf("ADPCM_CheckADP_header:: intBitsPerSample = %d\r\n", intBitsPerSample);

           xil_printf("\r\n");
		   
		   // change the start buffer index
		   intInBuffIndex=ix+12;

           return 1;

        }
    else{
            return 0;
    }
}



/*******************************************************************
 * @Name	: ADPCM_Pause
 * @Desc	: Stalls processor in a loop
 * @Param	: None
 * @Return 	: None
 *******************************************************************/
void ADPCM_Pause(void){		
	Xuint32 count;	
    count=0x00000003;
	while (count--) asm volatile ("nop");
}

/*******************************************************************
 * @Name	: ADPCM_MainLoop
 * @Desc	: ADPCM has it's own loop, decodes the block of data
 *			  within the loop. After every decode the timer is started
 *			  to output the PCM samples
 * @Param	: None
 * @Return 	: None
 *******************************************************************/
void ADPCM_MainLoop(void){
	
	int j=0;
	
	//----------------------------------------------------
    // ADPCM MAIN LOOP - goes on until end of file
    //----------------------------------------------------
	//xil_printf("sizeof(inbuf) = %d\r\n",sizeof(inbuf));
	//xil_printf("sizeof(outbuf) = %d\r\n",sizeof(outbuf));
	xil_printf("ADPCM_MainLoop:: enter\r\n");
		
	/* loop while the pointer in memory is lower than total filesize */	
	while(intInBuffIndex<intStopAddr){

		//xil_printf("ADPCM_MainLoop:: inside loop\r\n");
		
		ADPCM_Pause();
		
		/* only start to decode the next batch of adpcm samples
		   only when the previous samples have been completely
		   decoded.
		 */
		if((intADPCMSampleCount == 0) && (intInBuffIndex<intTotalFileSize)){
			
			// disable all interrupts - critical section
			microblaze_disable_interrupts();			
						
			// -- start decoding ADPCM_NSAMPLES of the adpcm file --
			// fils in input buffer
			for (j=0;j<(ADPCM_NSAMPLES/2);j++){
				inbuf[j]=XIo_In8(intInBuffIndex+j); // get data from SDRAM, byte by byte

				//xil_printf("ADPCM_MainLoop:: inbuff fill [%08X] = 0x%02X\r\n",intInBuffIndex+j,inbuf[j]);
			}
			
			// call the adpcm decoder to decode the NSAMPLES
			ADPCM_Decoder(inbuf, outbuf, sizeof(inbuf), &state);			
		
			// reset the counter again to count down
			intADPCMSampleCount=ADPCM_NSAMPLES;
			
			//increment the adpcm file pointer (get next chunk of data from SDRAM)
			intInBuffIndex=intInBuffIndex+(ADPCM_NSAMPLES/2);
			
			//reset the outbuff index
			intOutBuffIndex=0;
			
			// enable the interrupt - so the samples can be output 
			AuTmr_StartTimer(intSampleRate);			
						
			// enable all interrupts - critical section finished
			microblaze_enable_interrupts();			
		}
		else{
			// print reason why it didnt decode
			//xil_printf("main:: intADPCMSampleCount = %d\r\n", intADPCMSampleCount);					
		}
	}
	xil_printf("ADPCM_MainLoop:: Finished ADPCM file playing !!\r\n");
	AuTmr_StopTimer();	
	//xil_printf("ADPCM_MainLoop::exit intInBuffIndex = %08x\r\n", intInBuffIndex);
	
}



/*******************************************************************
 * @Name	: ADPCM_PlayOut
 * @Desc	: The outbuf contains the decoded PCM. Everytime the
 *			  interrupt kicks in this function is called to output
 *			  data to the DAC.
 * @Param	: None
 * @Return 	: None
 *******************************************************************/
void ADPCM_PlayOut(void){
	
	// adpcm specific
	Xuint8 intLSBs=0;
	Xuint8 intMSBs=0;
	Xuint16 intDACdata=0;
	
	
	//xil_printf("ADPCM_PlayOut::enter \r\n");
	
	if(intOutBuffIndex<(ADPCM_NSAMPLES+1)){
			
		// split into 8 bit groups
		intLSBs = outbuf[intOutBuffIndex]; 
		intMSBs = outbuf[intOutBuffIndex] >> 8;
		
		// reverse bit order and adjust unsigned scalling
		intDACdata = intLSBs | (intMSBs << 8);	// swap bit around
		intDACdata = intDACdata + 32768;		
		
		// set the appropriate volume (0->15)
		intDACdata = intDACdata*intVolume/15;
		
		// send data to dac (mono)
		DAC_SendDataToBothChannel(intDACdata,intDACdata);
		
		//xil_printf("ADPCM_PlayOut:: Sent to DAC=, 0x%04X \r\n", intDACdata);
		
		// decrement sample counter: when this gets to zero
		// the next batch of samples will be decoded.
		intADPCMSampleCount--;
		intOutBuffIndex++;				
	}
	else{
	
		DAC_PowerDownDAC();
		
		intADPCMSampleCount=0;
		intOutBuffIndex=0;
		
		// disable the interrupt - so the next batch can be decoded
		AuTmr_StopTimer();
		//XIntc_mDisableIntr(XPAR_XPS_INTC_0_BASEADDR, XPAR_XPS_TIMER_0_INTERRUPT_MASK);
	}	
}

/*******************************************************************
 * @Name	: ADPCM_FastForward
 * @Desc	: adds 1 to the input buffer pointer
 * @Param	: None
 * @Return 	: None
 *******************************************************************/
void ADPCM_FastForward(void){
	//intInBuffIndex = intInBuffIndex+(ADPCM_NSAMPLES/2);
	
	if((intInBuffIndex+1) > intStopAddr){
		intInBuffIndex = intInBuffIndex;
	}
	else{
		//intInBuffIndex = intInBuffIndex + (ADPCM_NSAMPLES/2);
		intInBuffIndex++;
	}
}

/*******************************************************************
 * @Name	: ADPCM_Rewind
 * @Desc	: subtracts 1 from the input buffer pointer
 * @Param	: None
 * @Return 	: None
 *******************************************************************/
void ADPCM_Rewind(void){
	//intInBuffIndex = intInBuffIndex-(ADPCM_NSAMPLES/2);
	
	if((intInBuffIndex-1) < intFileStartAddr){
		intInBuffIndex = intInBuffIndex;
	}
	else{
		//intInBuffIndex = intInBuffIndex - (ADPCM_NSAMPLES/2);
		intInBuffIndex--;
	}	
}

/*******************************************************************
 * @Name	: ADPCM_SetVolume
 * @Desc	: sets the volume (0->15). this value is then scalled to 
 *			  a fraction and multiplied with the DAC output sample.
 * @Param	: intV (volume)
 * @Return 	: None
 *******************************************************************/
 void ADPCM_SetVolume(Xuint8 intV){
	if(intV < 16){
		intVolume = intV;
	}
	else{
	// max volume level reached for ADPCM
		intVolume = intVolume;
	}
}


/*******************************************************************
 * GETTERS AND SETTERS for GLOBAL Vars
 *******************************************************************/
void ADPCM_SetOutBuffIndex(int intO){
	intOutBuffIndex = intO;
}

void ADPCM_SetInBuffIndex(int intI){
	intInBuffIndex = intI;
}

void ADPCM_SetSampleCount(int intSC){
	intADPCMSampleCount = intSC;
}

int ADPCM_GetSampleCount(void){
	return intADPCMSampleCount;
}

int ADPCM_GetInBuffIndex(void){
	return intInBuffIndex;
}

int ADPCM_GetOutBuffIndex(void){
	return intOutBuffIndex;
}

int ADPCM_GetSampleRate(void){
	return intSampleRate;
}


/*******************************************************************
 * @Name	: ADPCM_GetSongProgress
 * @Desc	: Calculates song progress. 
 *			 (input buffer pointer/file end position) *100%
 * @Param	: None
 * @Return 	: Xuint8 intProgress (%)
 *******************************************************************/
Xuint8 ADPCM_GetSongProgress(void){
	
	Xuint8 intProgress = 0;
	intProgress = ((intInBuffIndex/intStopAddr)*100);
	
	return intProgress;
}

/*******************************************************************
 * @Name	: ADPCM_Initialise
 * @Desc	: Initialises the Global variables. Has to be called
 *			  befoe playing a new song. The decoder must know the 
 *			  memory end address and start address.
 * @Param	: int intSDRAMOffset (end addr)
			  int intFSAdr		(start addr)
 * @Return 	: None
 *******************************************************************/
void ADPCM_Initialise(int intSDRAMOffset, int intFSAdr){
	
	intADPCMSampleCount = 0;
	intOutBuffIndex 	= 0;
	intInBuffIndex 		= intFSAdr;
	intTotalFileSize 	= intSDRAMOffset;
	intFileStartAddr	= intFSAdr;	// file start address (in SDRAM)
	
	// have to deduct 19KB (0x4c00) from file (for some unknown reason)
	//intStopAddr			= (intFSAdr+intSDRAMOffset)-(33*ADPCM_NSAMPLES);
	intStopAddr			= (intFSAdr+intSDRAMOffset)-0x4C00;
	//intStopAddr			= (intFSAdr+intSDRAMOffset);
	
	intSampleRate=0;
	intNumChannels=0;
	intBitsPerSample=0;
	
	xil_printf("ADPCM_Initialise: intTotalFileSize = %08X\r\n", intTotalFileSize);
	xil_printf("ADPCM_Initialise: intInBuffIndex = %08X\r\n", intInBuffIndex);
	xil_printf("ADPCM_Initialise: intStopAddr = %08X\r\n", intStopAddr);
		
}
