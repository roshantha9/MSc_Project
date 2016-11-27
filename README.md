# MSc.-Project
MSc. (DSE UoY) project documentation and partial source code

(some files may be fragmented/corrupted as my hard disk failed before pushing to git)

* Individual Project report included in /Documentation/

Bellow are a list source code that are included in this folder:

/ADPCM/ADPCM_Encoder
-------------------------------
ADPCM Encoder - runnable on a PC.
Contains original source code for Encoder+Decoder

/ADPCM/MicroBlaze_Decoder
------------------------------------
ADPCM Decoder ported to the MicroBlaze platform
(adpcm.c, adpcm.h)
includes interface functions

/ADPCM/WAV2ADPCM_GUI
-------------------------------
GUI tool developed in Python, used for ADPCM encoder (on the PC)

/Libmad_MP3_Decoder/LIBMAD_CACHE_Test_v3
---------------------------------------------------
Test Application, used to test the Libmad MP3 decoder on the Spartan 3E FPGA
Cache enabled MicroBlaze

/Libmad_MP3_Decoder/PROMBoot_Test_v3
-----------------------------------------------
Bootloader Test Application.
Attempts to load application code from PROM to SDRAM


