/**
  ******************************************************************************
  * @file    adc_test.c
  * @author  Name, Calixto Firmware Team 
  * @version V1.0.0
  * @date    4-June-2025
  * @brief   This file provides functions to manage ADC reading through Linux IIO:
  *           - Single channel ADC reading
  *           - Sysfs interface for IIO devices     
  *           - Continuous monitoring capability
  *
  *  @verbatim
  *    
  *          ===================================================================
  *                             Working of IIO ADC Interface
  *          ===================================================================
  *          
  *          IIO Subsystem Architecture
  *          =====================  
  *          - Uses Linux Industrial I/O subsystem (IIO)
  *          - Reads raw ADC values through sysfs interface
  *          - Path format: /sys/bus/iio/devices/iio:deviceX/in_voltageY_raw
  *          - Returns raw integer values that need scaling to voltage
  *
  *          ADC Conversion Process
  *          =======================  
  *          1. Opens the channel-specific sysfs file
  *          2. Reads the raw ADC value (0-4095 typical)
  *          3. Converts string value to integer
  *          4. Returns raw ADC count or -1 on error
  *  
  *          ===================================================================      
  *                              How to use this driver
  *          ===================================================================          
  *            - Include necessary headers (stdio.h, stdlib.h, fcntl.h etc.)
  *            - Call ADC_Read(channel) to get raw ADC value
  *            - Channel numbers are hardware dependent (check device tree)
  *            - Compile with: gcc adc.c -o adc_app
  *            - Run with: sudo ./adc_app
  *            - Ensure IIO device is enabled in kernel
  * 
  *          Example Usage:
  *            // Read channel 5 continuously
  *            while(1) {
  *                int val = ADC_Read(5);
  *                printf("ADC Value: %d\n", val);
  *                sleep(1);
  *            }
  *
  *  @endverbatim
  *    
  ******************************************************************************
  * 
  * <h2><center>&copy; COPYRIGHT 2025 Calixto Systems Pvt Ltd</center></h2>
  ******************************************************************************  
  */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
ADC_Read(int channel)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[4];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/bus/iio/devices/iio\\:device0/in_voltage%d_raw", channel);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open %s for reading!\n",path);
		return(-1);
	}
 
	if (-1 == read(fd, value_str, 4)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}
 
	close(fd);
 
	return(atoi(value_str));
}

int main()
{

    while(1)
	{
		printf("\r\n adc channel 5 value = %d",ADC_Read(5));
		sleep(1);
		printf("\r\n adc channel 5 value = %d",ADC_Read(5));
		sleep(1);
	}

 return 0;
}

/************** (C) COPYRIGHT 2025 Calixto Systems Pvt Ltd *****END OF FILE****/
