/*
 ============================================================================
 Name        : Platfom.c
 Author      : Riley, Ryan and Josh
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "RS232.h"
#include "Joystick.h"
#include "Pipes.h"
#include "Logger.h"

#define ECHOCHECK_MODE 	0
#define VERBOSE 		4

int main(void) {
	char Echo[10];
	char Command[10];
	int fd = 0;
	int pid;
	int iobytes;
	pthread_t JoystickThread;
	int pipefd[PIPE_ENDS];
	int *(pipefd_p[PIPE_ENDS]);
	pipefd_p[0] = &pipefd[0];

	if (Create_Log(LOG) == FILE_EXISTS_ERROR){
		Clear_Log(LOG);
		Logger(LOG,EVENT,"Log Cleared", getpid());
	}


	if (Create_Pipe(pipefd) == EXIT_FAILURE){
		Logger(LOG,CRASH,"Pipe creation failed",getpid());
		printf("error creating pipe\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&JoystickThread,NULL,&MonitorJoyStick, (void*)pipefd_p) == EXIT_FAILURE){
		Logger(LOG,CRASH,"joystick thread creation failed",getpid());
		printf("error creating joystick thread\n");
		exit(EXIT_FAILURE);
	}

	if ((fd = Open_Port(RS232PORT)) != EXIT_FAILURE){
		Setup_Port(fd);
	}
	else{
		Logger(LOG,CRASH,"Unable to open serial port fd",getpid());
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

		if (ECHOCHECK_MODE){
			iobytes = read(fd, Echo, 6);
		}
		//todo debug only
		printf("Char read:%d \nReply is:%s\n",iobytes, Echo);
		fflush(stdout);//Todo check if needed
	}
	return (0);
}

