S/**
  ******************************************************************************
  * @file    optiga_test.c
  * @author  Name, Calixto Firmware Team 
  * @version V1.0.0
  * @date    4-June-2025
  * @brief   This file provides functions to test OPTIGA Trust E security chip via I2C:
  *           - I2C register read/write operations
  *           - OPTIGA Trust E initialization sequence     
  *           - OpenApplication APDU command execution
  *           - Device state monitoring
  *
  *  @verbatim
  *    
  *          ===================================================================
  *                             Working of OPTIGA Trust E Communication
  *          ===================================================================
  *          
  *          I2C Communication Protocol
  *          =====================  
  *          - Uses standard Linux I2C-dev interface
  *          - Implements register-based communication with OPTIGA
  *          - Handles I2C bus errors with retry mechanism (100 attempts)
  *          - Follows OPTIGA Trust E I2C protocol specifications
  *
  *          OPTIGA Trust E Initialization
  *          =======================  
  *          1. Sets reset pin high via GPIO control
  *          2. Opens I2C bus device (/dev/i2c-1)
  *          3. Sets I2C slave address (0x30)
  *          4. Checks initial device state
  *          5. Executes multi-stage OpenApplication command
  *  
  *          ===================================================================      
  *                              How to use this test program
  *          ===================================================================          
  *            - Ensure OPTIGA Trust E is properly connected to I2C bus 1
  *            - Verify GPIO89 is connected to OPTIGA reset pin
  *            - Compile with: gcc optiga_test.c -o optiga_test
  *            - Run with: sudo ./optiga_test
  *            - Monitor output for successful completion of all stages
  * 
  *          Test Sequence:
  *            Stage 1: Writes OpenApplication APDU command
  *            Stage 2: Waits for device ready state (0x4900000A)
  *            Stage 3: Reads response data
  *            Stage 4: Writes additional command data
  *
  *  @endverbatim
  *    
  ******************************************************************************
  * 
  * <h2><center>&copy; COPYRIGHT 2025 Calixto Systems Pvt Ltd</center></h2>
  ******************************************************************************  
  */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "sysfs_gpio.h"

#define OPTIGA_I2C_BUS	1
#define OPTIGA_ADDRESS	0x30

#define DATA			0x80
#define DATA_REG_LEN	0x81
#define I2C_STATE		0x82
#define MAX_SCL_FREQU	0x84
#define APP_STATE_0		0x90


int file;
char buf[30];

char data_reg_test_value[] = {0x03,0x00,0x15,0x00,0x70,0x00,0x00,0x10,0xD2,0x76,0x00,0x00,0x04,0x47,0x65,0x6E,0x41,0x75,0x74,0x68,0x41,0x70,0x70,0x6C,0x04,0x1A };

char data_reg_test_value2[] = {0x80,0x00,0x00,0x0C,0xEC};

int 
i2c_read_register(char reg ,char length)
{
	unsigned int count = 0;
	buf[0]= reg;

	while(1) 
	{
	  if (write(file, buf, 1) != 1) {
			/* ERROR HANDLING: i2c transaction failed */
			count = count + 1;
			if(count > 100 ) {
				return (1);
			}
			continue;
	  } else {  
			if (read(file, buf, length) != length) {
				/* ERROR HANDLING: i2c transaction failed */
				count = count + 1;
				if(count > 100 ) {
					return (1);
				}
				continue;
			} else {
				/* return success */
				return (0);		
			}
	  }
	}
	return (0);
}

int 
i2c_write_register(char reg,char *data,char length)
{
	char count =0;
	buf[0]= reg;

	while(count<length)
	{
		buf[count+1] = data[count];
		count = count + 1; 
	}

	count = 0;
	while (1)
	{
		if (write(file, buf, length+1) != length+1) {
			/* ERROR HANDLING: i2c transaction failed */
			count = count + 1;
			if(count > 100 ) {
				return (1);
			}
			continue;
		} else {
			return (0);
		}
	}
	return (0);
}

int main ()
{

	int count =0;
	char i2c_dev[10];

	printf("\r\n*****************************************************");
	printf("\r\nTesting OPTIGA Trust E...\r\n");
	printf("*****************************************************\r\n");

	printf("\r\nSetting Reset Pin of OPTIGA Trust E to high\r\n");
	GPIOExport(89);
	GPIODirection(89,OUT);
	GPIOWrite(89,HIGH);

	sprintf(i2c_dev,"/dev/i2c-%d", OPTIGA_I2C_BUS);

	if ((file = open(i2c_dev, O_RDWR)) < 0){
		printf("\r\nFailed to open i2c-1\r\n");
		return(1);
	}

	if (ioctl(file, I2C_SLAVE,OPTIGA_ADDRESS) < 0){
		printf("\r\nI2C_SLAVE address failed\r\n");
		if (errno == EBUSY) {
			printf("UU\r\n");
		} 
		return(1);
	}

	printf("\r\n-----------------------------------------------------");
	printf("\r\nChecking Status of OPTIGA Trust E...\r\n");
	printf("-----------------------------------------------------\r\n");

	if (i2c_read_register(I2C_STATE,4)) {
		printf("\r\nerror reading I2C_STATE\r\n");
	}	else {	
			printf("\r\nI2C state => %02x %02x %02x %02x\r\n",buf[0],buf[1],buf[2],buf[3]);
	}

	printf("\r\n\r\n-----------------------------------------------------");
	printf("\r\n  Issue OpenApplication APDU command...\r\n");
	printf("-----------------------------------------------------\r\n");

	printf("\r\nStage 1: \r\n");
	if(i2c_write_register(DATA,data_reg_test_value,26)) {
		printf("\r\nerror writing data_reg_test_value to DATA(0x80) register\r\n");
		return (1);
	}	else {
			printf("i2c write to DATA reg(0x80) successful.\r\nCompleted Stage 1..\r\n"); 
	}


	printf("\r\nStage 2:\r\n");
	sleep(2);
	while(1) {

	  if (i2c_read_register(I2C_STATE,4)) {
		printf("\r\nerror reading I2C_STATE\r\n");
	  }	else {	
			printf("I2C state => %02x %02x %02x %02x\r\n",buf[0],buf[1],buf[2],buf[3]);
			if ( (buf[0]==0x49) && (buf[1]==0x00) && (buf[2]==0x00) && (buf[3]==0x0A) ) {
			   	printf("Completed stage 2.. \r\n");
				break;
			}
			sleep(1);
	  }
	}

	printf("\r\nStage 3:\r\n");

	  if (i2c_read_register(DATA,10)) {
		printf("error reading I2C_STATE\r\n");
		return(1);
	  } else {	
	  	printf("DATA REG(0x80) => %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9]);
	  }

	printf("\r\nStage 4:\r\n");
	if(i2c_write_register(DATA,data_reg_test_value2,5)) {
		printf("\r\nerror writing data_reg_test_value to DATA(0x80) register\r\n");
		return(1);
	}	else {
			printf("i2c write to DATA reg(0x80) successful.\r\nCompleted Stage 4..\r\n"); 
	}

	printf("\r\n*****************************************************");
	printf("\r\nTesting of OPTIGA Trust E is completed successfuly\r\n");
	printf("*****************************************************\r\n\r\n");
  
 return (0);
}
/************** (C) COPYRIGHT 2025 Calixto Systems Pvt Ltd *****END OF FILE****/
