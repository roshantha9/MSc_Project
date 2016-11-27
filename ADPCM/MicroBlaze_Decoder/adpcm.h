/*****************************************************************
 * Name   	   : ADPCM - Decoder (Header) (adpcm.h)
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
#ifndef ADPCM__H
#define ADPCM__H



#define ADPCM_NSAMPLES 			50
//#define ADPCM_FIXED_SAMPLERATE 	16000

struct adpcm_state {
    short      valprev;        /* Previous output value */
    char       index;          /* Index into stepsize table */
};

// main decode logic
extern void ADPCM_Decoder(char [], short [], int, struct adpcm_state *);

// getters and setters
extern void ADPCM_SetOutBuffIndex(int intO);
extern void ADPCM_SetInBuffIndex(int intI);
extern void ADPCM_SetSampleCount(int intSC);
extern int ADPCM_GetSampleCount(void);
extern int ADPCM_GetInBuffIndex(void);
extern int ADPCM_GetOutBuffIndex(void);
extern int ADPCM_GetSampleRate(void);
extern void ADPCM_SetVolume(Xuint8 intV);

// initialise - set ram address etc.
extern void ADPCM_Initialise(int intSDRAMOffset, int intFSAdr);

// play out the decoded samples
extern void ADPCM_PlayOut(void);

// decode the next chunk of data - do this in a loop.
extern void ADPCM_MainLoop(void);

// extract the header info from the song
extern Xuint8 ADPCM_CheckADP_header(void);

// rewind and ffwd
extern void ADPCM_FastForward(void);
extern void ADPCM_Rewind(void);

// calculate progress
extern Xuint8 ADPCM_GetSongProgress(void);

#endif
