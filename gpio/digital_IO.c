/**
  ******************************************************************************
  * @file    gpio.c
  * @author  Name , Calixto Firmware Team 
  * @version V1.0.0
  * @date    dd-Month-Year
   * @brief   This file provides functions to manage the following 
  *          breif the functionalities here: 
  *           - main
  *
  *  @verbatim
  *    
  *                             Working of GPIO
  *          ===================================================================
  *          
  *          GPIO Bank Architecture
  *          =====================  
  *          - Processor organizes GPIOs in banks (32 pins per bank)
  *          - Pin addressing: (bank_number * 32) + pin_number
  *          - Example: GPIO2_25 = (2 * 32) + 25 = Linux pin 89
  *
  *          Sysfs Control Mechanism
  *          =======================  
  *          Kernel exposes GPIO control through virtual files:
  *          /sys/class/gpio/
  *            ├── export        (activate GPIO)
  *            ├── unexport      (release GPIO)
  *            └── gpioX/        (per-pin control)
  *                ├── direction (in/out)
  *                └── value     (read/write)
  *
  *          Operation Sequence
  *          =================  
  *          1. Initialization:
  *             - Write pin# to 'export' file
  *             - Set direction via 'direction' file
  *          2. Input Mode:
  *             - Reads return '1' (HIGH) or '0' (LOW)
  *          3. Output Mode:
  *             - '1' drives pin HIGH (3.3V)
  *             - '0' drives pin LOW (GND)
  *
  *          Timing Characteristics
  *          =====================  
  *          - Exporting pin: ~100ms latency
  *          - Read/Write: ~10ms per operation
  *          - Not suitable for high-speed toggling
  *
  *          Safety Considerations
  *          ====================  
  *          - Always unexport unused pins
  *          - Never exceed max current/pin
  *          - Input voltages must stay within SoC limits
  *          - Avoid concurrent pin access
  * 
  *          
  *
  *          ===================================================================      
  *                              How to use this driver / source
  *          ===================================================================          
  *            - Include the header: #include "sysfs_gpio.h"
  *            - Initialize pins using GPIOInit(bank, pin, direction)
  *            - Read inputs with GPIORead(bank, pin)
  *            - Control outputs with GPIOWrite(bank, pin, value)
  *            - Ensure proper user permissions (typically requires root)
  *            - Compile with: gcc gpio.c -o gpio_app
  *            - Run as: sudo ./gpio_app
  * 
  *  @endverbatim
  *    
  ******************************************************************************
  * 
  * <h2><center>&copy; COPYRIGHT 2025 Calixto Systems Pvt Ltd</center></h2>
  ******************************************************************************  
  */ 



#include <stdlib.h>
#include <stdio.h>
#include "sysfs_gpio.h"


int main()
{

    /* Initialising GPIO2_24 and GPIO2_25 as input */
    GPIOInit(2,24,IN);
    GPIOInit(2,25,IN);

   /* Initialising GPIO2_22 and GPIO2_23 as output */    
    GPIOInit(2,22,OUT);
    GPIOInit(2,23,OUT);

    while(1)
	{
		printf("\r\n GPIO2_24 value = %d",GPIORead(2,24));
		printf("\r\n GPIO2_25 value = %d",GPIORead(2,25));

		GPIOWrite(2,22,LOW);
		GPIOWrite(2,23,LOW);
		sleep(1);

		GPIOWrite(2,22,HIGH);
		GPIOWrite(2,23,HIGH);
		sleep(1);
	}

 return 0;
}

/************** (C) COPYRIGHT 2025 Calixto Systems Pvt Ltd *****END OF FILE****/
