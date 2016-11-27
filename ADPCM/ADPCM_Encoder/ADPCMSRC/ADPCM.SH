#! /bin/sh
# This is a shell archive.  Remove anything before this line, then unpack
# it by saving it into a file and typing "sh file".  To overwrite existing
# files, type "sh file -c".  You can also feed this as standard input via
# unshar, or by typing "sh <file", e.g..  If this archive is complete, you
# will see the following message at the end:
#		"End of shell archive."
# Contents:  README adpcm.2 Makefile adpcm.h adpcm.c testc.c testd.c
# Wrapped by jack@schelvis.cwi.nl on Tue Jul  7 15:25:07 1992
PATH=/bin:/usr/bin:/usr/ucb ; export PATH
if test -f 'README' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'README'\"
else
echo shar: Extracting \"'README'\" \(899 characters\)
sed "s/^X//" >'README' <<'END_OF_FILE'
XThis directory contains version 1.0 of a simple 16 bit PCM <-> 4 bit
XADPCM coder and decoder. See the source and the manpage for comments
Xon the algorithm.
X
XThe routines have been tested on an SGI Indigo running Irix 4.0.2 and
Xon a Sparcstation 1+ running SunOS 4.1.1.
X
XOn a Sun you should compile with -DNODIVMUL. The resulting code will
Xcompress at 250Ksample/sec and decompress at 300Ksample/sec.
X
XOn an SGI you should compile *without* -DNODIVMUL, and the compressor
Xruns at 350Ksample/sec and the decompressor at 700Ksample/sec.
X
XIf you use this package I would like to hear from you.
XI am especially interested in people who can test interoperability
Xwith proven Intel/DVI ADPCM coders. I have tried to get the algorithm
Xcorrect, but you never know....
X
X	Jack Jansen
X	Centre for Mathematics and Computer Science
X	Kruislaan 413
X	Amsterdam
X	the Netherlands
X
X	+31 20 592 4098
X	Jack.Jansen@cwi.nl
END_OF_FILE
if test 899 -ne `wc -c <'README'`; then
    echo shar: \"'README'\" unpacked with wrong size!
fi
# end of 'README'
fi
if test -f 'adpcm.2' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'adpcm.2'\"
else
echo shar: Extracting \"'adpcm.2'\" \(1866 characters\)
sed "s/^X//" >'adpcm.2' <<'END_OF_FILE'
X.TH ADPCM 2 "7 June 1992"
X.SH NAME
Xadpcm - Adaptive delta compression for audio samples
X.B
X#include "adpcm.h"
X.br
Xadpcm_coder(short inbuf[], char outbuf[], int nsample,
X.br
X	struct adpcm_state *state);
X.br
Xadpcm_decoder(char inbuf[], short outbuf[], int nsample,
X.br
X	struct adpcm_state *state);
X.SH DESCRIPTION
XThese routine convert 16 bit linear PCM samples to 4 bit ADPCM code
Xand vice versa. The ADPCM code used is the Intel/DVI ADPCM code which
Xis being recommended by the IMA Digital Audio Technical Working Group.
X.PP
X\fIAdpcm_coder\fP converts \fInsample\fP 16 bit samples from
X\fIinbuf\fP and stores the 4 bit codes, packed two per byte, in
X\fIoutbuf\fP. \fIState\fP points to a structure containing the coder
Xstate information, a short and a char. It should be initialized to
Xzero before the first call. The coder updates the state record, and it
Xshould be passed to each subsequent call.
X.PP
X\fIAdpcm_decoder\fP converts \fInsample\fP 4 bit codes from
X\fIinbuf\fP to 16 bit samples and stores these in \fPoutbuf\fP.
X\fIState\fP is a state record similar to that used by the coder.
X.PP
XIf you compress audio to transmit it across a lossy network it is a
Xgood idea to send the compressor state along with each packet. (This
Xmust be the state as it was \fIbefore\fP the call to
X\fIadpcm_coder\fP, not the state after the return). This state record
Xcan then be fed to the decompressor. This way the decompressor can
Xtolerate packet loss.
X.SH NOTES
XThe routines have not been tested against an official Intel/DVI coder
Xyet, so incompatabilities might exist.
X.br
XThe state record is not byte-order-independent. Remember this when
Xtransmitting it across the network.
X.SH "SEE ALSO"
XProposal for Standardized Audio Interchange Formats, IMA compatability
Xproject proceedings, Vol 2, Issue 2, May 1992.
X.SH AUTHOR
XJack Jansen, CWI, Amsterdam
X.br
XJack.Jansen@cwi.nl
END_OF_FILE
if test 1866 -ne `wc -c <'adpcm.2'`; then
    echo shar: \"'adpcm.2'\" unpacked with wrong size!
fi
# end of 'adpcm.2'
fi
if test -f 'Makefile' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'Makefile'\"
else
echo shar: Extracting \"'Makefile'\" \(473 characters\)
sed "s/^X//" >'Makefile' <<'END_OF_FILE'
X#
X# Makefile for adpcm and test programs
X#
X# For Suns, define -DNODIVMUL (to make code run 5 times faster)
X#CFLAGS=-DNODIVMUL -O
X#
X# For SGI's don't define -DNODIVMUL (it only makes things slower)
XCFLAGS=-O
X
XALLSRC=README adpcm.2 Makefile adpcm.h adpcm.c testc.c testd.c
X
Xall: testc testd adpcm.o
X
Xadpcm.o: adpcm.h
X
Xtestc: testc.o adpcm.o
X	cc testc.o adpcm.o -o testc
X
Xtestd: testd.o adpcm.o
X	cc testd.o adpcm.o -o testd
X
Xadpcm.shar: $(ALLSRC)
X	shar $(ALLSRC) > adpcm.shar
END_OF_FILE
if test 473 -ne `wc -c <'Makefile'`; then
    echo shar: \"'Makefile'\" unpacked with wrong size!
fi
# end of 'Makefile'
fi
if test -f 'adpcm.h' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'adpcm.h'\"
else
echo shar: Extracting \"'adpcm.h'\" \(411 characters\)
sed "s/^X//" >'adpcm.h' <<'END_OF_FILE'
X/*
X** adpcm.h - include file for adpcm coder.
X**
X** Version 1.0, 7-Jul-92.
X*/
X
Xstruct adpcm_state {
X    short	valprev;	/* Previous output value */
X    char	index;		/* Index into stepsize table */
X};
X
X#ifdef __STDC__
X#define ARGS(x) x
X#else
X#define ARGS(x) ()
X#endif
X
Xvoid adpcm_coder ARGS((short [], char [], int, struct adpcm_state *));
Xvoid adpcm_decoder ARGS((char [], short [], int, struct adpcm_state *));
END_OF_FILE
if test 411 -ne `wc -c <'adpcm.h'`; then
    echo shar: \"'adpcm.h'\" unpacked with wrong size!
fi
# end of 'adpcm.h'
fi
if test -f 'adpcm.c' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'adpcm.c'\"
else
echo shar: Extracting \"'adpcm.c'\" \(6342 characters\)
sed "s/^X//" >'adpcm.c' <<'END_OF_FILE'
X/***********************************************************
XCopyright 1992 by Stichting Mathematisch Centrum, Amsterdam, The
XNetherlands.
X
X                        All Rights Reserved
X
XPermission to use, copy, modify, and distribute this software and its 
Xdocumentation for any purpose and without fee is hereby granted, 
Xprovided that the above copyright notice appear in all copies and that
Xboth that copyright notice and this permission notice appear in 
Xsupporting documentation, and that the names of Stichting Mathematisch
XCentrum or CWI not be used in advertising or publicity pertaining to
Xdistribution of the software without specific, written prior permission.
X
XSTICHTING MATHEMATISCH CENTRUM DISCLAIMS ALL WARRANTIES WITH REGARD TO
XTHIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
XFITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH CENTRUM BE LIABLE
XFOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
XWHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
XACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
XOF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
X
X******************************************************************/
X
X/*
X** Intel/DVI ADPCM coder/decoder.
X**
X** The algorithm for this coder was taken from the IMA Compatability Project
X** proceedings, Vol 2, Number 2; May 1992.
X**
X** Version 1.0, 7-Jul-92.
X*/
X
X#include "adpcm.h"
X
X#ifndef __STDC__
X#define signed
X#endif
X
X/* Intel ADPCM step variation table */
Xstatic int indexTable[16] = {
X    -1, -1, -1, -1, 2, 4, 6, 8,
X    -1, -1, -1, -1, 2, 4, 6, 8,
X};
X
Xstatic int stepsizeTable[89] = {
X    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
X    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
X    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
X    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
X    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
X    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
X    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
X    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
X    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
X};
X    
Xvoid
Xadpcm_coder(indata, outdata, len, state)
X    short indata[];
X    char outdata[];
X    int len;
X    struct adpcm_state *state;
X{
X    short *inp;			/* Input buffer pointer */
X    signed char *outp;		/* output buffer pointer */
X    int val;			/* Current input sample value */
X    int sign;			/* Current adpcm sign bit */
X    int delta;			/* Current adpcm output value */
X    int step;			/* Stepsize */
X    int valprev;		/* virtual previous output value */
X    int vpdiff;			/* Current change to valprev */
X    int index;			/* Current step change index */
X    int outputbuffer;		/* place to keep previous 4-bit value */
X    int bufferstep;		/* toggle between outputbuffer/output */
X
X    outp = (signed char *)outdata;
X    inp = indata;
X
X    valprev = state->valprev;
X    index = state->index;
X    step = stepsizeTable[index];
X    
X    bufferstep = 1;
X
X    for ( ; len > 0 ; len-- ) {
X	val = *inp++;
X
X	/* Step 1 - compute difference with previous value */
X	delta = val - valprev;
X	sign = (delta < 0) ? 8 : 0;
X	if ( sign ) delta = (-delta);
X
X	/* Step 2 - Divide and clamp */
X#ifdef NODIVMUL
X        {
X	    int tmp = 0;
X
X	    vpdiff = 0;
X	    if ( delta > step ) {
X		tmp = 4;
X		delta -= step;
X		vpdiff = step;
X	    }
X	    step >>= 1;
X	    if ( delta > step  ) {
X		tmp |= 2;
X		delta -= step;
X		vpdiff += step;
X	    }
X	    step >>= 1;
X	    if ( delta > step ) {
X		tmp |= 1;
X		vpdiff += step;
X	    }
X	    delta = tmp;
X	}
X#else
X	delta = (delta<<2) / step;
X	if ( delta > 7 ) delta = 7;
X
X	vpdiff = (delta*step) >> 2;
X#endif
X	  
X	/* Step 3 - Update previous value */
X	if ( sign )
X	  valprev -= vpdiff;
X	else
X	  valprev += vpdiff;
X
X	/* Step 4 - Clamp previous value to 16 bits */
X	if ( valprev > 32767 )
X	  valprev = 32767;
X	else if ( valprev < -32768 )
X	  valprev = -32768;
X
X	/* Step 5 - Assemble value, update index and step values */
X	delta |= sign;
X	
X	index += indexTable[delta];
X	if ( index < 0 ) index = 0;
X	if ( index > 88 ) index = 88;
X	step = stepsizeTable[index];
X
X	/* Step 6 - Output value */
X	if ( bufferstep ) {
X	    outputbuffer = (delta << 4) & 0xf0;
X	} else {
X	    *outp++ = (delta & 0x0f) | outputbuffer;
X	}
X	bufferstep = !bufferstep;
X    }
X
X    /* Output last step, if needed */
X    if ( !bufferstep )
X      *outp++ = outputbuffer;
X    
X    state->valprev = valprev;
X    state->index = index;
X}
X
Xvoid
Xadpcm_decoder(indata, outdata, len, state)
X    char indata[];
X    short outdata[];
X    int len;
X    struct adpcm_state *state;
X{
X    signed char *inp;		/* Input buffer pointer */
X    short *outp;		/* output buffer pointer */
X    int sign;			/* Current adpcm sign bit */
X    int delta;			/* Current adpcm output value */
X    int step;			/* Stepsize */
X    int valprev;		/* virtual previous output value */
X    int vpdiff;			/* Current change to valprev */
X    int index;			/* Current step change index */
X    int inputbuffer;		/* place to keep next 4-bit value */
X    int bufferstep;		/* toggle between inputbuffer/input */
X
X    outp = outdata;
X    inp = (signed char *)indata;
X
X    valprev = state->valprev;
X    index = state->index;
X    step = stepsizeTable[index];
X
X    bufferstep = 0;
X    
X    for ( ; len > 0 ; len-- ) {
X	
X	/* Step 1 - get the delta value and compute next index */
X	if ( bufferstep ) {
X	    delta = inputbuffer & 0xf;
X	} else {
X	    inputbuffer = *inp++;
X	    delta = (inputbuffer >> 4) & 0xf;
X	}
X	bufferstep = !bufferstep;
X
X	/* Step 2 - Find new index value (for later) */
X	index += indexTable[delta];
X	if ( index < 0 ) index = 0;
X	if ( index > 88 ) index = 88;
X
X	/* Step 3 - Separate sign and magnitude */
X	sign = delta & 8;
X	delta = delta & 7;
X
X	/* Step 4 - update output value */
X#ifdef NODIVMUL
X	vpdiff = 0;
X	if ( delta & 4 ) vpdiff  = (step << 2);
X	if ( delta & 2 ) vpdiff += (step << 1);
X	if ( delta & 1 ) vpdiff += step;
X	vpdiff >>= 2;
X#else
X	vpdiff = (delta*step) >> 2;
X#endif
X	if ( sign )
X	  valprev -= vpdiff;
X	else
X	  valprev += vpdiff;
X
X	/* Step 5 - clamp output value */
X	if ( valprev > 32767 )
X	  valprev = 32767;
X	else if ( valprev < -32768 )
X	  valprev = -32768;
X
X	/* Step 6 - Update step value */
X	step = stepsizeTable[index];
X
X	/* Step 7 - Output value */
X	*outp++ = valprev;
X    }
X
X    state->valprev = valprev;
X    state->index = index;
X}
END_OF_FILE
if test 6342 -ne `wc -c <'adpcm.c'`; then
    echo shar: \"'adpcm.c'\" unpacked with wrong size!
fi
# end of 'adpcm.c'
fi
if test -f 'testc.c' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'testc.c'\"
else
echo shar: Extracting \"'testc.c'\" \(485 characters\)
sed "s/^X//" >'testc.c' <<'END_OF_FILE'
X/* testc - Test adpcm coder */
X
X#include "adpcm.h"
X#include <stdio.h>
X
Xstruct adpcm_state state;
X
X#define NSAMPLES 1000
X
Xchar	abuf[NSAMPLES/2];
Xshort	sbuf[NSAMPLES];
X
Xmain() {
X    int n;
X
X    while(1) {
X	n = read(0, sbuf, NSAMPLES*2);
X	if ( n < 0 ) {
X	    perror("input file");
X	    exit(1);
X	}
X	if ( n == 0 ) break;
X	adpcm_coder(sbuf, abuf, n/2, &state);
X	write(1, abuf, n/4);
X    }
X    fprintf(stderr, "Final valprev=%d, index=%d\n",
X	    state.valprev, state.index);
X    exit(0);
X}
END_OF_FILE
if test 485 -ne `wc -c <'testc.c'`; then
    echo shar: \"'testc.c'\" unpacked with wrong size!
fi
# end of 'testc.c'
fi
if test -f 'testd.c' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'testd.c'\"
else
echo shar: Extracting \"'testd.c'\" \(489 characters\)
sed "s/^X//" >'testd.c' <<'END_OF_FILE'
X/* testd - Test adpcm decoder */
X
X#include "adpcm.h"
X#include <stdio.h>
X
Xstruct adpcm_state state;
X
X#define NSAMPLES 1000
X
Xchar	abuf[NSAMPLES/2];
Xshort	sbuf[NSAMPLES];
X
Xmain() {
X    int n;
X
X    while(1) {
X	n = read(0, abuf, NSAMPLES/2);
X	if ( n < 0 ) {
X	    perror("input file");
X	    exit(1);
X	}
X	if ( n == 0 ) break;
X	adpcm_decoder(abuf, sbuf, n*2, &state);
X	write(1, sbuf, n*4);
X    }
X    fprintf(stderr, "Final valprev=%d, index=%d\n",
X	    state.valprev, state.index);
X    exit(0);
X}
END_OF_FILE
if test 489 -ne `wc -c <'testd.c'`; then
    echo shar: \"'testd.c'\" unpacked with wrong size!
fi
# end of 'testd.c'
fi
echo shar: End of shell archive.
exit 0
-- 
Mark T. Price (sg)           mark@godzilla.Quotron.COM
>plan 9 studios<             "Bite me, it's fun!" -- Crow, MST 3000


