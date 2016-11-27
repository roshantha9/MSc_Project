#include "xparameters.h"
#include "xbasic_types.h"
#include "xspi.h"

#include "DAC.h"

XSpi mySPI;
Xuint32 status;


/***********************************************
 * Name: DAC_Initialise
 * Input: None
 * Output: None
 * Description: Initialis the DAC
 ***********************************************/
Xuint32 DAC_Initialise(){
	
	//Initialise mySPI structure
	status = XSpi_Initialize(&mySPI, XPAR_XPS_SPI_0_DEVICE_ID); //Initialise
	
	if (status != XST_SUCCESS){
		xil_printf("DAC_SPI initialisation failed!\r\n");
		return XST_FAILURE;
	}
	
	//Set options
	Xuint32 options = (XSP_MASTER_OPTION|XSP_LOOPBACK_OPTION);	
	status = XSpi_SetOptions(&mySPI, options);  
	if (status != XST_SUCCESS){
		xil_printf("DAC_SPI setopts failed!\r\n");
		return XST_FAILURE;
	}
	
	//Start the SPI IP Core
	status = XSpi_Start(&mySPI); //Start SPI 
	if (status != XST_SUCCESS){
		xil_printf("DAC_SPI start failed!\r\n");
		return XST_FAILURE;
	}
  
	// Disable all interrupts
	XSpi_mIntrGlobalDisable(&mySPI); 
  
	// Select the first slave
	XSpi_SetSlaveSelect(&mySPI, 1); 

	return status;
}


/***********************************************
 * Name: DAC_SendDataToBothChannel
 * Input: Xuint16 data
 * Output: Xuint32 status 
 * Description: Sending data to both of channels
 ***********************************************/
Xuint32 DAC_SendDataToBothChannel(Xuint16 data_A, Xuint16 data_B)
{
	//Initialise the variables
	Xuint8 inBuffer_B[] = {0x00, 0x00, 0x00, 0x00};
	Xuint8 inBuffer_A[] = {0x00, 0x00, 0x00, 0x00};
	Xuint8 outBuffer_B[] = {DAC_CMD_WRB_LDB, 0x00, 0x00, 0x00};
	Xuint8 outBuffer_A[] = {DAC_CMD_WRA_LDA, 0x00, 0x00, 0x00};
	Xuint32 status;
	
//xil_printf("dac fun ------ \r\n");  
	
	//Saperate the 16-bit input data into 8-bit
	outBuffer_B[1] = data_B >> 8;
	outBuffer_B[2] = data_B;
	
	outBuffer_A[1] = data_A >> 8;
	outBuffer_A[2] = data_A;
	
	//Transfer the data to right channel(DAC B)
	status = XSpi_Transfer(&mySPI, outBuffer_B, inBuffer_B, 4);

	asm volatile ("nop"); // wait for 1 or 2 clock cycles (t9 in DAC datasheet)
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	//Transfer the data to left channel(DAC A)
	status = XSpi_Transfer(&mySPI, outBuffer_A, inBuffer_A, 4);
	
	//asm volatile ("nop"); // wait for 1 or 2 clock cycles (t9 in DAC datasheet)
	
	return status;
}


/***********************************************
 * Name: DAC_SendDataToRightChannel
 * Input: Xuint16 data
 * Output: Xuint32 status 
 * Description: right channel is DAC B
 ***********************************************/
Xuint32 DAC_SendDataToRightChannel(Xuint16 data)
{
	//Initialise the variables
	Xuint8 inBuffer_B[] = {0x00, 0x00, 0x00, 0x00};
	Xuint8 outBuffer_B[] = {DAC_CMD_WRB_LDB, 0x00, 0x00, 0x00};
	Xuint32 status;
	
	//Saperate the 16-bit input data into 8-bit
	outBuffer_B[1] = data >> 8;
	outBuffer_B[2] = data;
	
	//Transfer the data to right channel(DAC B)
	status = XSpi_Transfer(&mySPI, outBuffer_B, inBuffer_B, 4);
	
	asm volatile ("nop"); // wait for 1 or 2 clock cycles (t9 in DAC datasheet)
	
	return status;	
}


/***********************************************
 * Name: DAC_SendDataToRightChannel
 * Input: Xuint16 data
 * Output: Xuint32 status 
 * Description: left channel is DAC A
 ***********************************************/
Xuint32 DAC_SendDataToLeftChannel(Xuint16 data)
{
	//Initialise the variables
	Xuint8 inBuffer_A[] = {0x00, 0x00, 0x00, 0x00};
	Xuint8 outBuffer_A[] = {DAC_CMD_WRA_LDA, 0x00, 0x00, 0x00};
	Xuint32 status;

	//Saperate the 16-bit input data into 8-bit
	outBuffer_A[1] = data >> 8;
	outBuffer_A[2] = data;	
	
	//Transfer the data to left channel(DAC A)
	status = XSpi_Transfer(&mySPI, outBuffer_A, inBuffer_A, 4);
	
	asm volatile ("nop"); // wait for 1 or 2 clock cycles (t9 in DAC datasheet)
	
	return status;
}


/***********************************************
 * Name: DAC_PowerDownDAC
 * Input: None
 * Output: None 
 * Description: Powering down the DAC
 ***********************************************/
void DAC_PowerDownDAC()
{
	//Initialise the variables
	Xuint8 inBuffer_A[] = {0x00, 0x00, 0x00, 0x00};
	Xuint8 outBuffer_A[] = {DAC_CMD_POWER_DWN_DACA, 0x00, 0x00, 0x00};
	Xuint8 outBuffer_B[] = {DAC_CMD_POWER_DWN_DACB, 0x00, 0x00, 0x00};  
	Xuint32 status;
	
	status = XSpi_Transfer(&mySPI, outBuffer_A, inBuffer_A, 4);
	
	asm volatile ("nop"); // wait for 1 or 2 clock cycles (t9 in DAC datasheet)
	
	status = XSpi_Transfer(&mySPI, outBuffer_B, inBuffer_A, 4);
	
	asm volatile ("nop"); // wait for 1 or 2 clock cycles (t9 in DAC datasheet)
	

}


/***********************************************
 * Name: DAC_PowerUpDAC
 * Input: None
 * Output: None 
 * Description: Powering up the DAC
 ***********************************************/
void DAC_PowerUpDAC()
{
	//Initialise the variables
	Xuint8 inBuffer_A[] = {0x00, 0x00, 0x00, 0x00};
	Xuint8 outBuffer_A[] = {DAC_CMD_POWER_UP_DACA, 0x00, 0x00, 0x00};
	Xuint8 outBuffer_B[] = {DAC_CMD_POWER_UP_DACB, 0x00, 0x00, 0x00};
	Xuint32 status;
	
	status = XSpi_Transfer(&mySPI, outBuffer_A, inBuffer_A, 4);
	
	asm volatile ("nop"); // wait for 1 or 2 clock cycles (t9 in DAC datasheet)
	
	status = XSpi_Transfer(&mySPI, outBuffer_B, inBuffer_A, 4);

	asm volatile ("nop"); // wait for 1 or 2 clock cycles (t9 in DAC datasheet)
}
