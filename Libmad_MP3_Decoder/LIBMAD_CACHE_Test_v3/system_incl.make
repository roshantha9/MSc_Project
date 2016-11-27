#################################################################
# Makefile generated by Xilinx Platform Studio 
# Project:D:\Xtemp\LIBMAD_CACHE_Test_v2\system.xmp
#
# WARNING : This file will be re-generated every time a command
# to run a make target is invoked. So, any changes made to this  
# file manually, will be lost when make is invoked next. 
#################################################################

XILINX_EDK_DIR = /cygdrive/c/Xilinx/10.1/edk
NON_CYG_XILINX_EDK_DIR = c:/Xilinx/10.1/edk

SYSTEM = system

MHSFILE = system.mhs

MSSFILE = system.mss

FPGA_ARCH = spartan3e

DEVICE = xc3s1200efg400-4

LANGUAGE = vhdl

SEARCHPATHOPT = 

SUBMODULE_OPT = 

PLATGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(SUBMODULE_OPT)

LIBGEN_OPTIONS = -mhs $(MHSFILE) -p $(DEVICE) $(SEARCHPATHOPT) \
                   $(MICROBLAZE_0_LIBG_OPT)

VPGEN_OPTIONS = -p $(DEVICE) $(SEARCHPATHOPT)

MANAGE_FASTRT_OPTIONS = -reduce_fanout no

OBSERVE_PAR_OPTIONS = -error yes

TESTAPP_OUTPUT_DIR = TestApp
TESTAPP_OUTPUT = $(TESTAPP_OUTPUT_DIR)/executable.elf

MICROBLAZE_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/microblaze/mb_bootloop.elf
PPC405_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc405/ppc_bootloop.elf
PPC440_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc440/ppc440_bootloop.elf
BOOTLOOP_DIR = bootloops

MICROBLAZE_0_BOOTLOOP = $(BOOTLOOP_DIR)/microblaze_0.elf
MICROBLAZE_0_XMDSTUB = microblaze_0/code/xmdstub.elf

BRAMINIT_ELF_FILES =   $(MICROBLAZE_0_BOOTLOOP) 
BRAMINIT_ELF_FILE_ARGS =   -pe microblaze_0  $(MICROBLAZE_0_BOOTLOOP) 

ALL_USER_ELF_FILES = $(TESTAPP_OUTPUT) 

SIM_CMD = vsim

BEHAVIORAL_SIM_SCRIPT = simulation/behavioral/$(SYSTEM)_setup.do

STRUCTURAL_SIM_SCRIPT = simulation/structural/$(SYSTEM)_setup.do

TIMING_SIM_SCRIPT = simulation/timing/$(SYSTEM)_setup.do

DEFAULT_SIM_SCRIPT = $(BEHAVIORAL_SIM_SCRIPT)

MIX_LANG_SIM_OPT = -mixed yes

SIMGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(BRAMINIT_ELF_FILE_ARGS) $(MIX_LANG_SIM_OPT)  -s mti -X D:/Xtemp/LIBMAD_CACHE_Test_v2/ -E D:/Xtemp/LIBMAD_CACHE_Test_v2/


LIBRARIES =  \
       microblaze_0/lib/libxil.a 
VPEXEC = virtualplatform/vpexec.exe

LIBSCLEAN_TARGETS = microblaze_0_libsclean 

PROGRAMCLEAN_TARGETS = TestApp_programclean 

CORE_STATE_DEVELOPMENT_FILES = 

WRAPPER_NGC_FILES = implementation/microblaze_0_wrapper.ngc \
implementation/mb_plb_wrapper.ngc \
implementation/ilmb_wrapper.ngc \
implementation/dlmb_wrapper.ngc \
implementation/dlmb_cntlr_wrapper.ngc \
implementation/ilmb_cntlr_wrapper.ngc \
implementation/lmb_bram_wrapper.ngc \
implementation/rs232_wrapper.ngc \
implementation/clock_generator_0_wrapper.ngc \
implementation/debug_module_wrapper.ngc \
implementation/proc_sys_reset_0_wrapper.ngc \
implementation/mpmc_0_wrapper.ngc \
implementation/xps_intc_0_wrapper.ngc \
implementation/xps_spi_0_wrapper.ngc \
implementation/xps_spi_1_wrapper.ngc \
implementation/xps_timer_0_wrapper.ngc \
implementation/timer_1_wrapper.ngc

POSTSYN_NETLIST = implementation/$(SYSTEM).ngc

SYSTEM_BIT = implementation/$(SYSTEM).bit

DOWNLOAD_BIT = implementation/download.bit

SYSTEM_ACE = implementation/$(SYSTEM).ace

UCF_FILE = data/system.ucf

BMM_FILE = implementation/$(SYSTEM).bmm

BITGEN_UT_FILE = etc/bitgen.ut

XFLOW_OPT_FILE = etc/fast_runtime.opt
XFLOW_DEPENDENCY = __xps/xpsxflow.opt $(XFLOW_OPT_FILE)

XPLORER_DEPENDENCY = __xps/xplorer.opt
XPLORER_OPTIONS = -p $(DEVICE) -uc $(SYSTEM).ucf -bm $(SYSTEM).bmm -max_runs 7

FPGA_IMP_DEPENDENCY = $(BMM_FILE) $(POSTSYN_NETLIST) $(UCF_FILE) $(XFLOW_DEPENDENCY)

#################################################################
# SOFTWARE APPLICATION TESTAPP
#################################################################

TESTAPP_SOURCES = /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/TestApp.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/version.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/bit.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/decoder.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/fixed.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/frame.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/huffman.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/layer3.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/stream.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/synth.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/timer.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/autmr.c /cygdrive/d/Xtemp/LIBMAD_CACHE_Test_v2/TestApp/DAC.c 

TESTAPP_HEADERS = /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/bit.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/COMMON.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/config.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/decoder.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/fixed.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/frame.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/global.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/huffman.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/layer3.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/mad.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/stream.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/synth.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/timer.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/version.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/sf_table.dat /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/imdct_l_arm.S /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/D.dat /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/imdct_s.dat /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/qc_table.dat /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/libmad-0.15.1b/rq_table.dat /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/autmr.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/DAC.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/SDRAM.h /cygdrive/d//Xtemp/LIBMAD_CACHE_Test_v2/TestApp/samples/speech_sample_16KHz.h 

TESTAPP_CC = mb-gcc
TESTAPP_CC_SIZE = mb-size
TESTAPP_CC_OPT = -O2
TESTAPP_CFLAGS = 
TESTAPP_CC_SEARCH = # -B
TESTAPP_LIBPATH = -L./microblaze_0/lib/ # -L
TESTAPP_INCLUDES = -I./microblaze_0/include/  -ITestApp/libmad-0.15.1b/ -ITestApp/ -ITestApp/samples/ # -I
TESTAPP_LFLAGS = # -l
TESTAPP_LINKER_SCRIPT = 
TESTAPP_LINKER_SCRIPT_FLAG = #-Wl,-T -Wl,$(TESTAPP_LINKER_SCRIPT) 
TESTAPP_CC_DEBUG_FLAG =  -g 
TESTAPP_CC_PROFILE_FLAG = # -pg
TESTAPP_CC_GLOBPTR_FLAG= # -mxl-gp-opt
TESTAPP_MODE = executable
TESTAPP_LIBG_OPT = -$(TESTAPP_MODE) microblaze_0
TESTAPP_CC_INFERRED_FLAGS= -mxl-soft-mul -mxl-pattern-compare -mcpu=v7.10.d 
TESTAPP_CC_START_ADDR_FLAG= -Wl,-defsym -Wl,_TEXT_START_ADDR=0x86000000
TESTAPP_CC_STACK_SIZE_FLAG= -Wl,-defsym -Wl,_STACK_SIZE=0x1600
TESTAPP_CC_HEAP_SIZE_FLAG= -Wl,-defsym -Wl,_HEAP_SIZE=0x19000
TESTAPP_OTHER_CC_FLAGS= $(TESTAPP_CC_GLOBPTR_FLAG)  \
                  $(TESTAPP_CC_START_ADDR_FLAG) $(TESTAPP_CC_STACK_SIZE_FLAG) $(TESTAPP_CC_HEAP_SIZE_FLAG)  \
                  $(TESTAPP_CC_INFERRED_FLAGS)  \
                  $(TESTAPP_LINKER_SCRIPT_FLAG) $(TESTAPP_CC_DEBUG_FLAG) $(TESTAPP_CC_PROFILE_FLAG) 
