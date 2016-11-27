/*-----------------------BEGIN USER SETABLE DEFINES -------------------------*/
#ifndef MPMC_DEVICE_ID
  #define MPMC_DEVICE_ID XPAR_MPMC_0_DEVICE_ID
#endif

/* Checks all memory writes/reads for accesses outside defined mem_space */
#ifndef CHECK_MEMORY_ADDRESS
  #define CHECK_MEMORY_ADDRESS 0
#endif
/* Specifies how many passes of the test to run, -1 == infinite */
#ifndef NUM_ITERATIONS
	#define NUM_ITERATIONS 1
#endif
/* Removes variable printing, which reduces the executable size */
#ifndef SMALL_FOOTPRINT
	#define SMALL_FOOTPRINT 0
#endif
/* Offset used to run the program out of the memory to test and not clobber our
 * software. Set this value to power of 2 larger than the size of this program 
 */
#ifndef MEM_OFFSET
	#define MEM_OFFSET 0x0
#endif
/* Tests 0-4 enable/disable, useful to reduce program size */
/* TEST0 - Write all zeros and verify */
/* TEST1 - Write all ones and verify */
/* TEST2 - Test for stuck together row/col bits */
/* TEST3 - MAX BA/ROW/COL NOISE */
/* TEST4 - DATA = !ADDR test */
#ifndef TEST0
	#define TEST0 1
#endif
#ifndef TEST1
	#define TEST1 1
#endif
#ifndef TEST2
	#define TEST2 1
#endif
#ifndef TEST3
	#define TEST3 1
#endif
#ifndef TEST4
	#define TEST4 1
#endif

/* Note: Pass C and D Disabled by default */
/* Pass A) ICache: On , DCache: On */
/* Pass B) ICache: Off, DCache: Off */
/* Pass C) ICache: On , DCache: Off */
/* Pass D) ICache: Off, DCache: On */
#ifndef PASS_A
	#define PASS_A 1
#endif
#ifndef PASS_B
	#define PASS_B 1
#endif
#ifndef PASS_C
	#define PASS_C 0
#endif
#ifndef PASS_D
	#define PASS_D 0
#endif

/*------------------------ END USER SETABLE DEFINES -------------------------*/

/* System dependent memory parameters, override with caution */
#if (MPMC_DEVICE_ID == 0)
	#ifndef MEM_BASEADDR
  #define MEM_BASEADDR  XPAR_MPMC_0_MPMC_BASEADDR
	#endif
	#ifndef MEM_HIGHADDR
  #define MEM_HIGHADDR  XPAR_MPMC_0_MPMC_HIGHADDR
	#endif
	#ifndef MEM_SIZE
		#define MEM_SIZE (MEM_HIGHADDR - MEM_BASEADDR) + 1
	#endif
	#ifndef MEM_WIDTH
  #define MEM_WIDTH     64      
	#endif
	#ifndef MEM_BANK_BITS
  #define MEM_BANK_BITS 2
	#endif
	#ifndef MEM_ROW_BITS
  #define MEM_ROW_BITS  13
	#endif
	#ifndef MEM_COL_BITS
  #define MEM_COL_BITS  10
	#endif
#endif

#if (MPMC_DEVICE_ID == 1)
	#ifndef MEM_BASEADDR
		#define MEM_BASEADDR XPAR_MPMC_1_MPMC_BASEADDR
	#endif
	#ifndef MEM_HIGHADDR
		#define MEM_HIGHADDR XPAR_MPMC_1_MPMC_HIGHADDR
	#endif
	#ifndef MEM_SIZE
		#define MEM_SIZE (MEM_HIGHADDR - MEM_BASEADDR) + 1
	#endif
	#ifndef MEM_WIDTH
		#define MEM_WIDTH XPAR_MPMC_1_MEM_DATA_WIDTH   
	#endif
	#ifndef MEM_BANK_BITS
		#define MEM_BANK_BITS XPAR_MPMC_1_MEM_PART_NUM_BANK_BITS
	#endif
	#ifndef MEM_ROW_BITS
		#define MEM_ROW_BITS XPAR_MPMC_1_MEM_PART_NUM_ROW_BITS
	#endif
	#ifndef MEM_COL_BITS
		#define MEM_COL_BITS XPAR_MPMC_1_MEM_PART_NUM_COL_BITS
	#endif
#endif

#if (MPMC_DEVICE_ID == 2)
	#ifndef MEM_BASEADDR
		#define MEM_BASEADDR XPAR_MPMC_2_MPMC_BASEADDR
	#endif
	#ifndef MEM_HIGHADDR
		#define MEM_HIGHADDR XPAR_MPMC_2_MPMC_HIGHADDR
	#endif
	#ifndef MEM_SIZE
		#define MEM_SIZE (MEM_HIGHADDR - MEM_BASEADDR) + 1
	#endif
	#ifndef MEM_WIDTH
		#define MEM_WIDTH XPAR_MPMC_2_MEM_DATA_WIDTH
	#endif
	#ifndef MEM_BANK_BITS
		#define MEM_BANK_BITS XPAR_MPMC_2_MEM_PART_NUM_BANK_BITS
	#endif
	#ifndef MEM_ROW_BITS
		#define MEM_ROW_BITS XPAR_MPMC_2_MEM_PART_NUM_ROW_BITS
	#endif
	#ifndef MEM_COL_BITS
		#define MEM_COL_BITS XPAR_MPMC_2_MEM_PART_NUM_COL_BITS
	#endif
#endif

#if (MPMC_DEVICE_ID == 3)
	#ifndef MEM_BASEADDR
		    #define MEM_BASEADDR XPAR_MPMC_3_MPMC_BASEADDR
	#endif
	#ifndef MEM_HIGHADDR
		    #define MEM_HIGHADDR XPAR_MPMC_3_MPMC_HIGHADDR
	#endif
	#ifndef MEM_SIZE
		    #define MEM_SIZE (MEM_HIGHADDR - MEM_BASEADDR) + 1
	#endif
	#ifndef MEM_WIDTH
		    #define MEM_WIDTH XPAR_MPMC_3_MEM_DATA_WIDTH   
	#endif
	#ifndef MEM_BANK_BITS
		    #define MEM_BANK_BITS XPAR_MPMC_3_MEM_PART_NUM_BANK_BITS
	#endif
	#ifndef MEM_ROW_BITS
		    #define MEM_ROW_BITS XPAR_MPMC_3_MEM_PART_NUM_ROW_BITS
	#endif
	#ifndef MEM_COL_BITS
		    #define MEM_COL_BITS XPAR_MPMC_3_MEM_PART_NUM_COL_BITS
	#endif
#endif

/* Calcualate our bit masks for Column, Row and Bank addresses, we are assuming
 * the memory is a power of 2 and even alignment
 */
#define START_BIT  (MEM_WIDTH == 8 ? 0 : MEM_WIDTH == 16 ? 1 : \
	MEM_WIDTH == 32 ? 2 : MEM_WIDTH == 64 ? 3 : 4)
#define COL_ADDR_MASK   (((( 1 << MEM_COL_BITS) - 1) << (START_BIT)) & \
	(MEM_SIZE - 1))
#define ROW_ADDR_MASK   (((( 1 << MEM_ROW_BITS) - 1) << (START_BIT + \
	MEM_COL_BITS)) & (MEM_SIZE - 1))
#define BANK_ADDR_MASK  (((( 1 << MEM_BANK_BITS) - 1) << (START_BIT + \
	MEM_COL_BITS + MEM_ROW_BITS)) & (MEM_SIZE - 1))

/* Macros to read and write words directly to memory, with optional address 
 * checking.  Not created as a function as to not create extra overhead in 
 * unnecessary function calls, since this will be heavily used code.
 */

#if (CHECK_MEMORY_ADDRESS == 1)
	#define WR_WORD(ADDR, DATA) \
	if ((ADDR) < (MEM_BASEADDR + MEM_OFFSET) || (ADDR) > (MEM_HIGHADDR + \
		MEM_OFFSET + MEM_SIZE -1)) { \
		printf ("\r\nInternal Error: Write Address 0x%08x outside of \
		range 0x%08x-0x%08x.", ADDR, MEM_BASEADDR + MEM_OFFSET, \
		MEM_BASEADDR + MEM_OFFSET + MEM_SIZE - 1); \
	} \
	else { \
		*(U32Ptr)(ADDR) = (DATA); \
	}


	#define RD_WORD (ADDR, DATA) \
	if ((ADDR) < MEM_BASEADDR || (ADDR) > MEM_HIGHADDR) { \
		printf ("\r\nInternal Error: Read Address 0x%08x outside of \
		range 0x%08x-0x%08x.", ADDR, MEM_BASEADDR + MEM_OFFSET, \
		MEM_BASEADDR + MEM_OFFSET + MEM_SIZE - 1); \
	} \
	else { \
		(DATA) = *(U32Ptr)(ADDR); \
	}
#else
	#define WR_WORD(ADDR, DATA) *(U32Ptr)(ADDR) = (DATA);  
	#define RD_WORD(ADDR, DATA) (DATA) = *(U32Ptr)(ADDR);
#endif

#define printf( ... ) xil_printf( __VA_ARGS__ )


/* This type is a word address ptr */
typedef volatile u32 *U32Ptr;


/* Global Variables, accessible from XMD */
static u32 PassCount = 1;
static u32 TotalErrors = 0;
