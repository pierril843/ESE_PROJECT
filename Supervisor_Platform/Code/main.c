/*
 ============================================================================
 Name        : Platfom.c
 Author      : Riley, Ryan and Josh
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */



//#include <errno.h>      // Error number definitions
//#include <stdint.h>     // C99 fixed data types
//#include <string.h>     // String function definitions
//#include <unistd.h>     // UNIX standard function definitions
//#include <fcntl.h>      // File control definitions
//#include <termios.h>    // POSIX terminal control definitions
//#include <sys/types.h>
//#include <sys/time.h>
//#include <sys/ioctl.h>
//#include <sys/select.h>

#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

#include "RS232.h"
#include "Joystick.h"
#include "Pipes.h"


int main(void) {
	char Echo[10];
	char Command[10];
	int fd = 0;
	int iobytes;
	pthread_t JoystickThread;
	int pipefd[PIPE_ENDS];
	int *(pipefd_p[PIPE_ENDS]);
	pipefd_p[0] = &pipefd[0];

	if (Create_Pipe(pipefd) != EXIT_FAILURE){



			pthread_create(&JoystickThread,NULL,MonitorJoyStick, (void*)pipefd_p);
		}
		else{
			//todo log error
			printf("error creating pipe\n");
			exit(EXIT_FAILURE);
		}

	if ((fd = Open_Port(RS232PORT)) != EXIT_FAILURE){
		Setup_Port(fd);
	}
	else{
		//todo log error
		printf("error opening %s\n", RS232PORT);
		exit(EXIT_FAILURE);
	}


	while (1) {
		if ((read(pipefd[PIPEREAD], Command, sizeof(Command))) < 0) {//busy wait for something to show up
			printf ("pipe read error\n");
			//todo: add log for error
		}

		iobytes = write(fd, Command, sizeof(Command));

		//todo Debug only
		// echo sent command onto screen for testing purposes
		write (1, Command, sizeof(Command));
		fflush(stdout);//Todo check if needed
		//todo Debug only
		// printf out how many bytes were written, also classic Warcraft 3 Peasant line
		printf("Job's Done. Wrote %d bytes\n", iobytes);
		fflush(stdout);//Todo check if needed

		iobytes = read(fd, Echo, 6);
		//todo debug only
		printf("Char read:%d \nReply is:%s\n",iobytes, Echo);
		fflush(stdout);//Todo check if needed
	}
	return (0);
}

