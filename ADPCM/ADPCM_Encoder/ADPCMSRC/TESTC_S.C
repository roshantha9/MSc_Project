/* testc - Test adpcm coder */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
//#include "../ADPCMSRC/ADPCM.H"


struct adpcm_state {
    short      valprev_l;        /* Previous output value */
    char       index_l;          /* Index into stepsize table */
    short      valprev_r;        /* Previous output value */
    char       index_r;          /* Index into stepsize table */
};

struct adpcm_state state;

//void adpcm_coder (short [], char [], int, struct adpcm_state *);
//void adpcm_decoder (char [], short [], int, struct adpcm_state *);
void adpcm_coder(short indata[], char outdata[], int len, struct adpcm_state *state);
void adpcm_decoder(char indata[], short outdata[], int len, struct adpcm_state *state);

#define NSAMPLES 1000

#define XST_FAILURE 0
#define XST_SUCCESS 1

char   abuf[NSAMPLES/2];
short  sbuf[NSAMPLES];

unsigned int SampleRate=0;
unsigned int AudioFormat=0;
unsigned int NumChannels=0;
unsigned int WaveSize=0;
unsigned int ByteRate=0;
unsigned int BlockAlign=0;
unsigned int BitsPerSample=0;

unsigned char wav_header[44]={0};
unsigned char adpcm_header[12]={0};

/* local function prototypes */
unsigned int ADPCM_WavCheckwav_header();


unsigned int ADPCM_WavCheckwav_header()
{

	// write 'ADPCM' to adpcm_header
	adpcm_header[0] = 0x41; //A
	adpcm_header[1] = 0x44; //D
	adpcm_header[2] = 0x50; //P
	adpcm_header[3] = 0x43; //C
    adpcm_header[4] = 0x4D; //M


	if (wav_header[0]==0x52 && wav_header[1]==0x49 &&
		wav_header[2]==0x46 && wav_header[3]==0x46 &&
		wav_header[8]==0x57 && wav_header[9]==0x41 &&
		wav_header[10]==0x56 && wav_header[11]==0x45 )
		{
    AudioFormat=(unsigned int)wav_header[20];
	printf( " AudioFormat  = %06u\r\n", AudioFormat );
	//---------------------------------------------

	//-------NumChannels---------------------------
	//Mono = 1, Stereo = 2, etc.
	NumChannels=wav_header[22];
	printf( " NumChannels  = %06u\r\n", NumChannels );
	// 1 byte
	adpcm_header[5] = wav_header[22];

    //-----------------SampleRate       -------------------
	SampleRate=(unsigned int)wav_header[24]+((unsigned int)wav_header[25]<<8) + ((unsigned int)wav_header[26]<<16) + ((unsigned int)wav_header[27]<<24);
	printf( " SampleRate   = %06u\r\n", SampleRate );
	// 4 bytes
	adpcm_header[6] = wav_header[24];
    adpcm_header[7] = wav_header[25];
    adpcm_header[8] = wav_header[26];
    adpcm_header[9] = wav_header[27];


	//----------ByteRate == SampleRate * NumChannels * BitsPerSample/8
	ByteRate=(unsigned int)wav_header[28]+((unsigned int)wav_header[29]<<8) + ((unsigned int)wav_header[30]<<16) + ((unsigned int)wav_header[31]<<24);
    printf( " ByteRate     = %06u\r\n", ByteRate );

    //------------BlockAlign== NumChannels * BitsPerSample/8
    //The number of bytes for one sample including all channels.
	BlockAlign=	(unsigned int)wav_header[32]+((unsigned int)wav_header[33]<<8);
    printf( " BlockAlign   = %06u\r\n", BlockAlign );

   //--------------- BitsPerSample
   //8 bits = 8, 16 bits = 16, etc.
    BitsPerSample=(unsigned int)wav_header[34]+((unsigned int)wav_header[35]<<8);
	printf( " BitsPerSample= %06u\r\n", BitsPerSample );
	// 2 bytes
	adpcm_header[10] = wav_header[34];
	adpcm_header[11] = wav_header[35];


	//WaveSize == NumSamples * NumChannels * BitsPerSample/8
    //This is the number of bytes in the data.

	WaveSize=(unsigned int)wav_header[40]+((unsigned int)wav_header[41]<<8) + ((unsigned int)wav_header[42]<<16) + ((unsigned int)wav_header[43]<<24);
	printf( " WaveSize     = %06u\r\n", WaveSize );
			return XST_SUCCESS;
		}
		else return XST_FAILURE;
}




int main() {
    FILE    *f, *f2;
    char    fname[64],fname2[64];
    //int i=0;

    if (_argc < 3 )
    {
        printf("Usage: %s infile outfile\r\n", _argv[0]);
        exit(1);
    }
    strcpy(fname,_argv[1]);
    strcpy(fname2,_argv[2]);

    if ( NULL == (f2=fopen(fname2,"wb")) )
    {
        printf("Error opening '%s', terminating..\r\n", fname2);
        exit(4);
    }
    if ( NULL == (f=fopen(fname,"rb")) )
    {
        printf("Error opening '%s', terminating..\r\n", fname);
        exit(4);
    }

    // read and store the wav file wav_header info
    fread(&wav_header,sizeof(wav_header),1,f);
    ADPCM_WavCheckwav_header(); // this constructs the adpcm file header
                                // extracts data from the wav header

    // write out the ADPCM wav_header
    fwrite(&adpcm_header, sizeof(adpcm_header), 1, f2);

    while( fread(&sbuf,sizeof(sbuf),1,f) )
    {
        adpcm_coder(sbuf, abuf, sizeof(sbuf)/4, &state);
        fwrite(&abuf,sizeof(abuf),1,f2);
    }

    fclose(f);
    fclose(f2);
    exit(0);

    return 0;
}
