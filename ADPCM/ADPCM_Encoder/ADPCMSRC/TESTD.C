/* testd - Test adpcm decoder */

#include "adpcm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

struct adpcm_state state;

#define NSAMPLES 1000

char   abuf[NSAMPLES/2];
short  sbuf[NSAMPLES];

int main() {
    FILE    *f, *f2;
    char    fname[64],fname2[64];

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

    while( fread(&abuf,sizeof(abuf),1,f) )
    {
        adpcm_decoder(abuf, sbuf, sizeof(abuf), &state);
        fwrite(&sbuf,sizeof(sbuf),1,f2);
    }
    fclose(f);
    fclose(f2);
    printf("Final valprev=%d, index=%d\r\n",
           state.valprev, state.index);
    //exit(0);

    return 0;
}
