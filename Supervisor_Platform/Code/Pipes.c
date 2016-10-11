/*
 * Pipes.c
 *
 *  Created on: Oct 2, 2016
 *      Author: pierril
 */

#include "Pipes.h"
#include "stdlib.h"
#include "syslog.h"

int Create_Pipe(int fd[PIPE_ENDS]){
	if (pipe(fd) < 0) {
			syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR), "pipe() failed");
			return(EXIT_FAILURE);
		}
	return(EXIT_SUCCESS);
}

int Piper(int pipefd[PIPE_ENDS], char Message[10]){
	int state;
	if ((state = write(pipefd[PIPEWRITE], Message, 10)) != 10) {
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR), "Write to pipe failed");
		return(EXIT_FAILURE);
	}
	return(EXIT_SUCCESS);
}


