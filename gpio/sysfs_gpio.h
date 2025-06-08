/**
  ******************************************************************************
  * @file    sysfs_gpio.h
  * @author  Name, Calixto Firmware Team 
  * @version V1.0.0
  * @date    4-June-2025
  * @brief   This file contains all the functions prototypes for the GPIO driver
  *          using Linux sysfs interface. Includes pin direction and state macros.
  *
  * @details Provides the following functionality:
  *          - GPIO pin initialization and configuration
  *          - Digital input reading
  *          - Digital output control
  *          - Bank-based GPIO addressing
  ******************************************************************************
  * @defgroup GPIO_Macros GPIO Direction and State Macros
  * @brief Constants for GPIO direction and output state
  * @{
  */
 
#define IN  0
#define OUT 1
 
#define LOW  0
#define HIGH 1

extern int GPIOInit(int bank,int gpio,int dir);
extern int GPIORead(int bank,int gpio);
extern int GPIOWrite(int bank,int gpio, int value);


#endif /*__SYSFS_GPIO_H */
