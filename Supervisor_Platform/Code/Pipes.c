/*
 * Pipes.c
 *
 *  Created on: Oct 2, 2016
 *      Author: pierril
 */

#include "Pipes.h"
#include "stdlib.h"

int Create_Pipe(int fd[PIPE_ENDS]){
	if (pipe(fd) < 0) {
			perror ("pipe error");
			exit(EXIT_FAILURE);
		}
	return(EXIT_SUCCESS);
}

int Piper(int pipefd[PIPE_ENDS], char Message[10]){
	if (write(pipefd[PIPEWRITE], Message, 10) != 10) {
		perror ("write error"); return(-1);
	}
	return(0);
}


