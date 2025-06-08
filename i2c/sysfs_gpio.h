/**
  ******************************************************************************
  * @file    sysfs_gpio.h
  * @author  Name, Calixto Firmware Team 
  * @version V1.0.0
  * @date    4-June-2025
  * @brief   This file contains all function prototypes for Linux sysfs GPIO control
  *          including pin direction and state definitions.
  *
  * @details Provides interface for:
  *          - GPIO pin export/unexport
  *          - Direction configuration (input/output)
  *          - Digital read/write operations
  *          - I2C pin management
  ******************************************************************************
  * @defgroup GPIO_Constants GPIO Direction and State Constants
  * @brief Definitions for GPIO configuration
  * @{
  */
#define IN  0
#define OUT 1
 
#define LOW  0
#define HIGH 1

extern int GPIOExport(int pin);
extern int GPIOUnexport(int pin);
extern int GPIODirection(int pin, int dir);
extern int GPIORead(int pin);
extern int GPIOWrite(int pin, int value);

#endif /*__SYSFS_GPIO_H */
