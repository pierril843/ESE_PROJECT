/*
 * RS232.c
 *
 *  Created on: Oct 2, 2016
 *      Author: pierril
 */
#include "RS232.h"
#include "errno.h"
#include "stdlib.h"
#include "termios.h"
#include "fcntl.h"
#include "stdio.h"
#include "syslog.h"

int Open_Port (const char PortLocaltion[]){

	int fd;
	// ttyS1, open for reading and writing | doesn't make the file a controlling terminal for the process | non-blocking
	fd = open(PortLocaltion, O_RDWR | O_NOCTTY);// | O_NONBLOCK);

    if (fd == -1){
        syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Unable to open %s serial port",PortLocaltion);
        return(fd);
    }

   return fd;
}
int Close_Port (int fd){
	if (close(fd) == EXIT_FAILURE){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Unable to close fd %d serial port",fd);
		return(EXIT_FAILURE);
	}
	return(EXIT_SUCCESS);
}

int Setup_Port (int fd){
	struct termios options;

	if((tcgetattr(fd,&options)) < 0){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Failed to getattr, fd: %d", fd);
		return(EXIT_FAILURE);
	}

	// control mode flags: baud rate | enables RTS/CTS (hardware) flow control
	// eight bits in send & receive chars | ignore any modem status lines | enable the receipt of characters
	options.c_cflag = B38400 | CS8 | CLOCAL | CREAD;// | CRTSCTS;

	// Input mode flags: ignore characters with parity errors | convert a received carriage return to a newline
	options.c_iflag = IGNPAR | ICRNL;

	// Output mode flags: none
	options.c_oflag = 0;

	// Local modes: enable canonical input processing | local echoing of input characters
	options.c_lflag &= ~(ICANON | ECHO);

	// read will wait until min characters can be read and then return that number of characters
	options.c_cc[VMIN] = 6;
	options.c_cc[VTIME] = 1;

	tcflush(fd, TCIFLUSH);

	if((tcsetattr(fd, TCSANOW, &options)) < 0){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Failed to setattr, fd: %d", fd);
		return(EXIT_FAILURE);
	}
	return(EXIT_SUCCESS);
}
