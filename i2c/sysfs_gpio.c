/**
  ******************************************************************************
  * @file    sysfs_gpio.c
  * @author  Name, Calixto Firmware Team 
  * @version V1.0.0
  * @date    4-June-2025
  * @brief   This file provides functions to manage GPIO operations through Linux sysfs:
  *           - GPIO pin export/unexport
  *           - GPIO direction configuration (input/output)     
  *           - GPIO read/write operations
  *           - I2C pin direction control
  *
  *  @verbatim
  *    
  *          ===================================================================
  *                             Working of Sysfs GPIO Interface
  *          ===================================================================
  *          
  *          GPIO Control through Sysfs
  *          =====================  
  *          - Uses Linux sysfs virtual file system interface
  *          - GPIO operations performed through file operations in /sys/class/gpio/
  *          - Requires root privileges for access
  *          - Provides hardware abstraction through filesystem
  *
  *          I2C Pin Direction Control
  *          =======================  
  *          - Special handling for I2C pins when setting direction
  *          - Must ensure proper pull-up/down configuration
  *          - Direction changes may affect I2C bus functionality
  *          - Requires careful timing when switching between input/output
  *  
  *          ===================================================================      
  *                              How to use this driver
  *          ===================================================================          
  *            - Include "sysfs_gpio.h" header file
  *            - Export pin using GPIOExport(pin_number)
  *            - Set direction using GPIODirection(pin_number, direction)
  *            - For I2C pins, ensure bus is idle before changing direction
  *            - Read/write using GPIORead()/GPIOWrite() functions
  *            - Unexport pins when done using GPIOUnexport()
  * 
  *          Example Usage for I2C Pins:
  *            // Export GPIO pin
  *            GPIOExport(123);
  *            
  *            // Set as input for I2C SDA
  *            GPIODirection(123, IN);
  *            
  *            // Read current state
  *            int state = GPIORead(123);
  *            
  *            // Set as output temporarily
  *            GPIODirection(123, OUT);
  *            GPIOWrite(123, HIGH);
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
GPIORead(int pin)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
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
GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
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
/************** (C) COPYRIGHT 2025 Calixto Systems Pvt Ltd *****END OF FILE****/
