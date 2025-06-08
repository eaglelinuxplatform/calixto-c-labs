/**
  ******************************************************************************
  * @file    uart_example.c
  * @author  Name, Calixto Firmware Team 
  * @version V1.0.0
  * @date    4-June-2025
  * @brief   This file provides functions to demonstrate UART communication:
  *           - Serial port initialization and configuration
  *           - Raw data transmission     
  *           - Terminal settings management
  *           - Basic flow control
  *
  *  @verbatim
  *    
  *          ===================================================================
  *                             UART Communication Implementation
  *          ===================================================================
  *          
  *          Serial Port Configuration
  *          =====================  
  *          - Uses termios structure for port settings
  *          - Configures baud rate (B115200)
  *          - Sets 8N1 format (8 data bits, no parity, 1 stop bit)
  *          - Enables local connection (CLOCAL) and receiver (CREAD)
  *          - Ignores parity errors (IGNPAR)
  *          - Disables software flow control (IXON/IXOFF)
  *
  *          Data Transmission
  *          =======================  
  *          - Implements raw I/O mode (non-canonical)
  *          - Disables echo and signal processing
  *          - Minimum characters to read set to 1 (VMIN = 1)
  *          - No inter-character timeout (VTIME = 0)
  *          - Uses blocking write operations
  *          - Includes flush operations (TCIOFLUSH/TCOFLUSH)
  *  
  *          ===================================================================      
  *                              How to use this example
  *          ===================================================================          
  *            - Connect to /dev/ttySC3 serial port (Calixto EVM)
  *            - Ensure matching baud rate on receiving device
  *            - Compile with: gcc uart_example.c -o uart_test
  *            - Run with: sudo ./uart_test
  *            - Program will continuously send "Hello World" messages
  * 
  *          Key Settings:
  *            - Change BAUDRATE definition as needed (keep 'B' prefix)
  *            - Modify MODEMDEVICE for different serial port
  *            - Adjust VMIN/VTIME for different read behavior
  *            - Enable ICANON for line-by-line processing
  *
  *  @endverbatim
  *    
  ******************************************************************************
  * 
  * <h2><center>&copy; COPYRIGHT 2025 Calixto Systems Pvt Ltd</center></h2>
  ******************************************************************************  
  */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>

#include <stdlib.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B115200   // Change as needed, keep B

/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttySC3" //Calixto EVM serial port

#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

main()
{
    int fd, c, res,count;
    struct termios oldtio, newtio;

    /* Open modem device for reading and writing and not as controlling tty
       because we don't want to get killed if linenoise sends CTRL-C. */
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
    if (fd < 0) { perror(MODEMDEVICE); exit(-1); }

    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

    /* BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
       CS8     : 8n1 (8bit,no parity,1 stopbit)
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters */
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

    /* IGNPAR  : ignore bytes with parity errors
       otherwise make device raw (no other input processing) */
    newtio.c_iflag = IGNPAR;
    newtio.c_iflag &= ~(IXOFF|IXON) ;


    /*  Raw output  */
    newtio.c_oflag = 0;

    /* ICANON  : enable canonical input
       disable all echo functionality, and don't send signals to calling program */
    //newtio.c_lflag = ICANON;

	//
	// No line processing
	//
	// echo off, echo newline off, canonical mode off, 
	// extended input processing off, signal chars off
	//
	newtio.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON | ISIG);

	 //
	 // One input byte is enough to return from read()
	 // Inter-character timer off
	 //
	 newtio.c_cc[VMIN]  = 1;
	 newtio.c_cc[VTIME] = 0;


    /* now clean the modem line and activate the settings for the port */
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);

    printf("UART EXAMPLE\r\n");
    /* terminal settings done, now handle input*/
    while (TRUE) {     /* loop continuously */

	// Note that this code & format values in manual are hexadecimal
	
	printf("--------------------------------------------------------\r\n");

	tcflush(fd, TCIOFLUSH);
    	write(fd,"Hello World ",12);
	printf("Sent \r\n");
	tcflush(fd, TCOFLUSH);
	sleep(1);
   
    }
    //tcsetattr(fd, TCSANOW, &oldtio);
}
/************** (C) COPYRIGHT 2025 Calixto Systems Pvt Ltd *****END OF FILE****/
