/*
 ============================================================================
 Name        : joy2.c
 Author      : 
 Version     :
 Copyright   :
 Description :
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/joystick.h>
#define JOY_DEV "/dev/input/js0"
#define FUNCT	2
#define ARG1	4
#define ARG2	6
#define BINSIZE 3641
#define SERVO	'O'
#define STEPPER	'S'
#define L_MOTOR	'L'
#define R_MOTOR 'R'
#define DEFAULT 'F'
#define START	'X'
#define UP		'1'
#define DOWN	'2'
#define LEFT	'1'
#define RIGHT	'2'
#define ZEROED	'0'

#define READ 0
#define WRITE 1
#define PIPE_ENDS 2




int MotorValue(__s16 value) {
	int bin = 0;
	int BinSize = BINSIZE;

	if (value == 0){
		return (bin);
	}
	else if (value > 0){
		for (bin=1 ; bin < 10;bin++){
			if (value < BinSize)
				return (bin);
			BinSize= BinSize+BINSIZE;
		}
	}
	else{
		value = value*(-1);
		for (bin=1 ; bin < 10;bin++){
			if (value < BinSize)
				return (bin);
			BinSize= BinSize+BINSIZE;
		}
	}
	return (0);
}

int Piper(int pipefd[PIPE_ENDS], char Message[9]){
	if (write(pipefd[WRITE], Message, 9) != 9) {
		perror ("write error"); return(-1);
	}
	return(0);
}

int MonitorJoyStick (int pipefd[PIPE_ENDS]){
	int Joy_fd;
	struct js_event js;
	char message[9] = "~ F 0 0 ?";
	if( ( Joy_fd = open( JOY_DEV, O_RDONLY ) ) < 0 ){
		printf( "Couldn't open joystick device %s\n", JOY_DEV );
		return -1;
	}

	while (1) {
		if (read(Joy_fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
			printf("\n error reading");
			exit (1);
	 	}
		//printf("Event: type %d, time %d, number %d, value %d\n",js.type, js.time, js.number, js.value);
		if (js.type == 2){
	 		switch (js.number)
	 		{
				case (5) :
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
				case (4) :
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
				case (1) :
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
				case (3) :
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
				default :
					message[FUNCT] = DEFAULT;
					message[ARG1] = ZEROED;
					message[ARG2] = ZEROED;
					break;
	 		}
	 	}
		///////////////////////////////////////////////////////
	 	else if(js.type == 1){
	 		switch (js.number)
	 		{
	 			case (9) :
	 				message[FUNCT] = START;
	 				if (js.value > 0)
	 					message[ARG1] = '1';
	 				else
	 					message[ARG1] = ZEROED;
	 				message[ARG2] = ZEROED;
	 				Piper(pipefd,message);
	 				break;
	 			default :
	 				message[FUNCT] = DEFAULT;
	 				message[ARG1] = ZEROED;
	 				message[ARG2] = ZEROED;
	 				break;
	 		}
	 	}
		//////////////////////////////////////////////////////
	 	else{
	 		//printf("undocumented command\n");
	 		message[FUNCT] = DEFAULT;
	 		message[ARG1] = ZEROED;
	 		message[ARG2] = ZEROED;
	 	}

	}
	close(Joy_fd);
}
int main(void) {
	int Parentpid;
	int pipefd[PIPE_ENDS], n;
	char buff[9];

	 if (pipe(pipefd) < 0) {
	      perror ("pipe error");
	      exit(-1);
	 }


	if((Parentpid = fork()) == 0){
  	    /*child*/
		MonitorJoyStick(pipefd);
		exit(0);
	}
 	else {
  	    /*parent*/
 		while (1) {
 			if ((n = (read(pipefd[READ], buff, sizeof(buff)))) < 0) {//busy wait for something to show up
 		  	    perror ("read error"); exit(2);
 		  	}
 			write (1, buff, n);
 			printf("\n");
 		}
 		return (0);
 	}
}




