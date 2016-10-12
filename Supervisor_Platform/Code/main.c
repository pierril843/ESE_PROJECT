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
#include "unistd.h"
#include "RS232.h"
#include "Joystick.h"
#include "Pipes.h"
#include "syslog.h"
#include "Sockets.h"

#define ECHOCHECK_MODE 	0//always on if using sockets
#define PRINT_COMMANDS 	1
#define RS_232		1
#define SOCKETS		0
#define CONNECTION  RS_232


int main(void) {
	char Echo[10];
	char Command[9];
	pthread_t JoystickThread;
	int iobytes;
	int fd;
	int pipefd[PIPE_ENDS];


	if (Create_Pipe(pipefd) == EXIT_FAILURE){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Pipe creation failed");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&JoystickThread,NULL,&MonitorJoyStick, (void*)&pipefd) == EXIT_FAILURE){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Joystick thread creation failed");
		exit(EXIT_FAILURE);
	}

	//todo make two ifendif for weather you send it across rs232 or sockets or if
	//could check for access to stack as way to know if rs232 usb port of on this vs pie3
	if (CONNECTION == RS_232){
		if ((fd = Open_Port(RS232PORT)) != EXIT_FAILURE){
			if (Setup_Port(fd) == EXIT_FAILURE){
				syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"RS-232 port setup failed");
				exit(EXIT_FAILURE);
			}
		}
		else{
			syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Open access to RS-232 port failed, fd:%d",fd);
			exit(EXIT_FAILURE);
		}
		while (1) {
			//todo check if thread is still running exit and log if not
			if ((read(pipefd[PIPEREAD], Command, sizeof(Command))) < 0) {//busy wait for something to show up
				printf ("pipe read error\n");
				syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Pipe read failed");
			}
			else{
				if ((iobytes = write(fd, Command, sizeof(Command))) < 0){
					printf ("RS-232 write error\n");
					syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"RS-232 write failed");
				}

				syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO),"Command Sent:%s", Command);

				//Debug only
				// echo sent command onto screen for testing purposes
				if (PRINT_COMMANDS){
					write (1, Command, sizeof(Command));
					fflush(stdout);//Todo check if needed
					fprintf(stdout,"\nJob's Done. Wrote %d bytes pid:%d\n", iobytes, getpid());
					fflush(stdout);//Todo check if needed
				}

				if (ECHOCHECK_MODE){
					iobytes = read(fd, Echo, 6);
				}

				if (PRINT_COMMANDS & ECHOCHECK_MODE){
					printf("Char read:%d \nReply is:%s\n",iobytes, Echo);
					fflush(stdout);//Todo check if needed
				}
			}
		}
	}
	else if (CONNECTION == SOCKETS){
		if ((fd = Socket_Init(CLIENT)) == EXIT_FAILURE){
			syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Socket port setup failed");
			exit(EXIT_FAILURE);
		}
		while (1) {
			//todo check if thread is still running exit and log if not
			if ((read(pipefd[PIPEREAD], Command, sizeof(Command))) < 0) {//busy wait for something to show up
				printf ("pipe read error\n");
				syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Pipe read failed");
			}
			else{
				if (Socket_Write(fd, Command) == EXIT_FAILURE){
					printf ("Socket write error\n");
					syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Socket write failed");
				}
				//Debug only
				// echo sent command onto screen for testing purposes
				if (PRINT_COMMANDS){
					write (1, Command, sizeof(Command));
					fflush(stdout);//Todo check if needed
				}
			}
		}
	}

	return (0);
}

