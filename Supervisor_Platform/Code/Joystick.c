/*
 * Joystick.c
 *
 *  Created on: Oct 3, 2016
 *      Author: pierril
 */
#include "stdio.h"
#include "Joystick.h"
#include "Pipes.h"
#include "Protocol.h"
#include "stdlib.h"
#include "linux/joystick.h"
#include "fcntl.h"
#include "syslog.h"
//todo add signal catcher for when parent thread closes
int MotorValue(int value) {
	int bin = 0;
	int BinSize = BINSIZE;

	if (value == 0){
		return (bin);
	}
	//find |value|
	if (value < 0){
		value = value*(-1);
	}

	for (bin=1 ; bin < 10;bin++){
		if (value < BinSize)
			return (bin);
		BinSize= BinSize+BINSIZE;
	}
	return(EXIT_FAILURE);
}

void* MonitorJoyStick (void * args){
	int Joy_fd;
	struct js_event js;
	char message[PACKET_LEN] = "~ F 0 0 ?";
	int pipefd[PIPE_ENDS];
	pipefd[PIPEREAD] = ((int*)args)[PIPEREAD];
	pipefd[PIPEWRITE] = ((int*)args)[PIPEWRITE];





	//open Joystick
	if( ( Joy_fd = open( JOY_DEV, O_RDONLY ) ) < 0 ){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Couldn't open joystick device %s", JOY_DEV);
		exit(EXIT_FAILURE);
	}

	while (1) {
		//Read Joystick
		if (read(Joy_fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
			printf("\n error reading");
			fflush(stdout);
			exit(EXIT_FAILURE);
		}

		//type %d, time %d, number %d, value %d\n",js.type, js.time, js.number, js.value
		if (js.type == 2){
			switch (js.number){
			case (5) :{		//D-PAD UP-DOWN
				message[FUNCT] = SERVO;
				if (js.value > 0)
					message[ARG1] = DOWN;
				else if (js.value < 0)
					message[ARG1] = UP;
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (4) :{		//D-PAD LEFT-RIGHT
				message[FUNCT] = STEPPER;
				if (js.value > 0)
					message[ARG1] = RIGHT;
				else if (js.value < 0)
					message[ARG1] = LEFT;
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (1) :{		//L-ANALOG UP-DOWN
				message[FUNCT] = L_MOTOR;
				message[ARG1] = (char)(MotorValue(js.value)+48);
				if (js.value > 0)
					message[ARG2] = DOWN;
				else if (js.value < 0)
					message[ARG2] = UP;
				else
					message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (3) :{		//R-ANALOG UP-DOWN
				message[FUNCT] = R_MOTOR;
				message[ARG1] = (char)(MotorValue(js.value)+48);
				if (js.value > 0)
					message[ARG2] = DOWN;
				else if (js.value < 0)
					message[ARG2] = UP;
				else
					message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			default :
				message[FUNCT] = DEFAULT;
				message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				break;
			}
		}
		else if(js.type == 1){
			switch (js.number){
			case (0) :{
				message[FUNCT] = X_BUTTON;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (1) :{
				message[FUNCT] = A_BUTTON;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (2) :{
				message[FUNCT] = B_BUTTON;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (3) :{
				message[FUNCT] = Y_BUTTON;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (4) :{
				message[FUNCT] = L_BUTTON;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (5) :{
				message[FUNCT] = R_BUTTON;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (6) :{
				message[FUNCT] = L_TRIGGER;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (7) :{
				message[FUNCT] = R_TRIGGER;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (8) :{
				message[FUNCT] = SELECT;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			case (9) :{
				message[FUNCT] = START;
				if (js.value > 0)
					message[ARG1] = '1';
				else
					message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				Piper(pipefd,message);
				break;
			}
			default :
				message[FUNCT] = DEFAULT;
				message[ARG1] = ZEROED;
				message[ARG2] = ZEROED;
				break;
			}
		}
		else{
			//printf("undocumented command\n");
			syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Undocumented joystick command entered");
			message[FUNCT] = DEFAULT;
			message[ARG1] = ZEROED;
			message[ARG2] = ZEROED;
		}

	}
	//Close Joystick
	close(Joy_fd);

	exit(EXIT_FAILURE);
}

