/**
  ******************************************************************************
  * @file    sysfs_gpio.c
  * @author  Name , Calixto Firmware Team 
  * @version V1.0.0
  * @date    4-June-2025
  * @brief   This file provides functions to manage GPIO operations through sysfs:
  *           - GPIO initialization and configuration
  *           - GPIO direction control (input/output)     
  *           - GPIO read/write operations
  *           - GPIO pin export/unexport management
  *
  *  @verbatim
  *    
  *          ===================================================================
  *                             Working of Sysfs GPIO Interface
  *          ===================================================================
  *          
  *          GPIO Bank Architecture
  *          =====================  
  *          - Processor organizes GPIOs in 32-pin banks
  *          - Each pin addressed as: (bank_number * 32) + pin_number
  *          - Example: GPIO2_25 = (2 * 32) + 25 = Linux pin 89
  *
  *          Sysfs Control Mechanism
  *          =======================  
  *          - Uses Linux sysfs virtual file system interface
  *          - Control files located at /sys/class/gpio/
  *          - Each operation involves file operations (open/read/write/close)
  *          - Provides hardware abstraction through filesystem
  *  
  *          ===================================================================      
  *                              How to use this driver
  *          ===================================================================          
  *            - Include "sysfs_gpio.h" in your application
  *            - Initialize pins using GPIOInit(bank, pin, direction)
  *            - Read inputs with GPIORead(bank, pin)
  *            - Control outputs with GPIOWrite(bank, pin, value)
  *            - Compile with: gcc gpio.c your_app.c -o gpio_app
  *            - Run with root privileges: sudo ./gpio_app
  *            - Ensure proper permissions on /sys/class/gpio/
  * 
  *          Example Usage:
  *            // Initialize GPIO2_22 as output, GPIO2_24 as input
  *            GPIOInit(2, 22, OUT);
  *            GPIOInit(2, 24, IN);
  *            
  *            // Read input and set output
  *            int val = GPIORead(2, 24);
  *            GPIOWrite(2, 22, val);
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
#include "sysfs_gpio.h" 
 
int
GPIOExport(int pin)
{
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
int
GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
int
GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";
 
#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;
 
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}
 
	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}
 
int
GPIORead(int bank,int gpio)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
	
	int pin;
	pin = bank * 32 + gpio;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}
 
	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}
 
	close(fd);
 
	return(atoi(value_str));
}
 
int
GPIOWrite(int bank,int gpio, int value)
{
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
	int pin;
	pin = bank * 32 + gpio;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}
 
	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}
 
	close(fd);
	return(0);
}

int 
GPIOInit(int bank,int gpio,int dir)
{
	int pin;
	pin = bank * 32 + gpio;

	if (GPIOExport(pin))
		return(-1);

	if (GPIODirection(pin,dir))
		return(-1);

	return(0);
}

/************** (C) COPYRIGHT 2025 Calixto Systems Pvt Ltd *****END OF FILE****/
