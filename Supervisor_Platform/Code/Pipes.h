/*
 * Pipes.h
 *
 *  Created on: Oct 2, 2016
 *      Author: pierril
 */

#ifndef PIPES_H_
#define PIPES_H_

#define PIPEREAD  0
#define PIPEWRITE 1
#define PIPE_ENDS 2

int Create_Pipe(int[]);
int Piper(int[], char[]);

#endif /* PIPES_H_ */
