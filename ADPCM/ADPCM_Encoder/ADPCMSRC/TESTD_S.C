/* testd - Test adpcm decoder */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

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

#define NSAMPLES 20000

char   abuf[NSAMPLES/2];
short  sbuf[NSAMPLES];

unsigned int SampleRate=0;
unsigned int NumChannels=0;
unsigned int BitsPerSample=0;

unsigned char adpcm_header[12]={0};

/* local function prototypes */
unsigned int ADPCM_CheckADP_header(void);

unsigned int ADPCM_CheckADP_header(void){

    // check if file has the 'ADPCM' tag
    if ((adpcm_header[0] == 0x41) && (adpcm_header[1] == 0x44) &&
        (adpcm_header[2] == 0x50) && (adpcm_header[3] == 0x43) &&
        (adpcm_header[4] == 0x4D)){

           NumChannels =    adpcm_header[5];
           printf("\r\n");
           printf("ADPCM_CheckADP_header:: NumChannels = %u\r\n", NumChannels);

           SampleRate =(unsigned int)adpcm_header[6]+((unsigned int)adpcm_header[7]<<8) + ((unsigned int)adpcm_header[8]<<16) + ((unsigned int)adpcm_header[9]<<24);
           printf("ADPCM_CheckADP_header:: SampleRate = %u\r\n", SampleRate);

           BitsPerSample = (unsigned int)adpcm_header[10]+((unsigned int)adpcm_header[11]<<8);
           printf("ADPCM_CheckADP_header:: BitsPerSample = %u\r\n", BitsPerSample);

           printf("\r\n");


           return 1;

        }
    else{
            return 0;
    }
}


int main() {
    FILE    *f, *f2;
    char    fname[64],fname2[64];
    int     n;

    printf("ADPCM 4:1 stereo decoder v1.0 by Patrick Aalto 1993\r\n");
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

    // extract header info from the ADPCM file
    fread(&adpcm_header,1,sizeof(adpcm_header),f);
    if(ADPCM_CheckADP_header()){
        printf("Decoding '%s' into '%s', please wait...\r\n", fname, fname2);
        while( n = fread(&abuf,1,sizeof(abuf),f) )
        {
            adpcm_decoder(abuf, sbuf, n, &state);
            fwrite(&sbuf,4,n,f2);
        }
        fclose(f);
        fclose(f2);
        exit(0);
    }
    else{
        printf("File '%s' is an invalid ADPCM file, exiting\r\n", fname);
        fclose(f);
        fclose(f2);
        exit(0);
    }

    return 0;

}
