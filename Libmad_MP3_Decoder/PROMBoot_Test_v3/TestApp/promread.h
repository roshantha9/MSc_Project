/******************************************************************************
 * Name : PROM Boot-loader
 * FileName: promread.h
 * Author : Rosh Mendis 
 * Date Created : 10/08/2011
 * Description : Contains defines and function prototypes
 ******************************************************************************/ 

#define ADDRSYNC 0x9F8FAFBF
#define END_PROM 0xFFFFFFFF

#define OE_HIGH 	0x2   // 0010
#define OE_LOW 		0x0
#define CCLK_HIGH 	0x4
#define CCLK_LOW 	0x0
#define CE_HIGH 	0x1
#define CE_LOW 		0x0

#define DATAREAD 0x0
#define ADDRREAD 0x1

//to enable debugging to UART uncomment the following
#define DEBUG

#ifdef DEBUG
#define INFO(format,args...) xil_printf(format, ##args)
#else
#define INFO(format,args...)
#endif


// prototypes
int promread_init(void);
Xuint32 prom_read_helper( Xuint8 num);
int promread_start(void);
Xuint32 PROMREAD_GetAddrPointer();
