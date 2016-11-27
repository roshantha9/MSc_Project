#ifndef DAC_H			/* prevent circular inclusions */
#define DAC_H			/* by using protection macros */

// -- DAC specific defines --
#define DAC_CMD_WRB_LDB			0x00	// WRITE and LOAD to DAC-B, CMD(8-bits)=00100100
#define DAC_CMD_WRA_LDA			0x34	// WRITE and LOAD to DAC-A, CMD(8-bits)=00010000
#define DAC_CMD_POWER_DWN_DACA	0x13	
#define DAC_CMD_POWER_DWN_DACB	0x27
#define DAC_CMD_POWER_UP_DACA	0x10
#define DAC_CMD_POWER_UP_DACB	0x24	


// -- DAC specific function prototypes --
Xuint32 DAC_Initialise(void);
Xuint32 DAC_SendDataToRightChannel(Xuint16 data);
Xuint32 DAC_SendDataToLeftChannel(Xuint16 data);
Xuint32 DAC_SendDataToBothChannel(Xuint16 data_A, Xuint16 data_B);
void DAC_PowerDownDAC(void);
void DAC_PowerUpDAC(void);

#endif /* end of protection macro */
