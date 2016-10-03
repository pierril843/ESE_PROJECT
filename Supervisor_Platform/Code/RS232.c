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
//#include "sys/stat.h"
#include "fcntl.h"
#include "stdio.h"

int Open_Port (const char PortLocaltion){

	int fd;
	// ttyS1, open for reading and writing | doesn't make the file a controlling terminal for the process | non-blocking
	fd = open(&PortLocaltion, O_RDWR | O_NOCTTY);// | O_NONBLOCK);

    if (fd == -1){
        //fprintf(stderr, "Unable to open serial port %s\n",strerror(errno));
        printf("Unable to open %s serial port\n",(char*)&PortLocaltion);
        //TODO add log message
        fflush(stdout);
        return(fd);
    }

   return fd;
}
int Close_Port (int fd){
	if (close(fd) == EXIT_FAILURE){
		printf("Unable to close fd %d serial port\n",fd);
		//todo add logging
	}
	return(EXIT_SUCCESS);
}
int Setup_Port (int fd){
	struct termios options;

	if((tcgetattr(fd,&options)) < 0){
		fprintf(stderr, "Failed to getattr: %d\n", fd);
		printf("Failed to getattr\n");
		fflush(stdout);
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
		//fprintf(stderr, "Failed to setattr: %d, %s\n", fd, strerror(errno) );
		printf("Failed to setattr\n");
		fflush(stdout);
		return(EXIT_FAILURE);
	}
	return(EXIT_SUCCESS);
}
