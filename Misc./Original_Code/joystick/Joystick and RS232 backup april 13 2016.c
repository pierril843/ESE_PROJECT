/*
 ============================================================================
 Name        : Combined.c
 Author      : Riley Pierce, Josh Perry
 Version     : 1.2
 Copyright   :
 Description : Note: you need to fflush(stdout); after every printf because linux is stupid
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <linux/joystick.h>
#include <errno.h>      // Error number definitions
#include <stdint.h>     // C99 fixed data types
#include <string.h>     // String function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <termios.h>    // POSIX terminal control definitions
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#define comport "/dev/ttyUSB0" // where the serial port is

#define JOY_DEV "/dev/input/js0"
#define FUNCT	2
#define ARG1	4
#define ARG2	6
#define BINSIZE 3641
#define SERVO	'O'
#define STEPPER	'S'
#define L_MOTOR	'L'
#define R_MOTOR 'R'
#define START	'X'
#define DEFAULT 'F'
#define UP		'1'
#define DOWN	'2'
#define LEFT	'1'
#define RIGHT	'2'
#define ZEROED	'0'

#define READ 0
#define WRITE 1
#define PIPE_ENDS 2


int openport(void){

	int fd;
	// ttyS1, open for reading and writing | doesn't make the file a controlling terminal for the process | non-blocking
	fd = open(comport, O_RDWR | O_NOCTTY);// | O_NONBLOCK);

    if (fd == -1){
        //fprintf(stderr, "Unable to open serial port %s\n",strerror(errno));
        printf("Unable to open serial port\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

  // int oflags = fcntl(fd, F_GETFL);
  // fcntl(fd, F_SETFL, oflags | O_NONBLOCK);// | O_ASYNC);

   return fd;
}

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

int Piper(int pipefd[PIPE_ENDS], char Message[10]){
	if (write(pipefd[WRITE], Message, 10) != 10) {
		perror ("write error"); return(-1);
	}
	return(0);
}

int MonitorJoyStick (int pipefd[PIPE_ENDS]){
	int Joy_fd;
	struct js_event js;
	char message[10] = "~ F 0 0 ?";
	if( ( Joy_fd = open( JOY_DEV, O_RDONLY ) ) < 0 ){
		printf( "Couldn't open joystick device %s\n", JOY_DEV );
		fflush(stdout);
		return -1;
	}

	while (1) {
		if (read(Joy_fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
			printf("\n error reading");
			fflush(stdout);
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
	char buff[10];

	int fd = 0;
	struct termios options;
	struct termios newoptions;
	int bytes;
	char buffer[10];
	int readbytes;
	int result;

	struct timeval WaitTime;
				int Ready;
				fd_set fdsel;

				WaitTime.tv_sec = 5;
				WaitTime.tv_usec = 0;

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

 			// open RS-232 port
			fd = openport();
			printf("%d", fd);

			FD_ZERO(&fdsel);
			FD_SET(fd,&fdsel);
//			printf("%d", FD_SETSIZE);

			if((tcgetattr(fd,&options)) < 0){
				fprintf(stderr, "Failed to getattr: %d, %s\n", fd, strerror(errno) );
				printf("Failed to getattr\n");
				fflush(stdout);
				exit(EXIT_FAILURE);
			}

			// control mode flags: baud rate | enables RTS/CTS (hardware) flow control \
			| eight bits in send & receive chars | ignore any modem status lines | enable the receipt of characters
			newoptions.c_cflag = B38400 | CS8 | CLOCAL | CREAD;// | CRTSCTS;

			// Input mode flags: ignore characters with parity errors | convert a received carriage return to a newline
			newoptions.c_iflag = IGNPAR | ICRNL;

			// Output mode flags: none
			newoptions.c_oflag = 0;

			// Local modes: enable canonical input processing | local echoing of input characters
			newoptions.c_lflag &= ~(ICANON | ECHO);

			// read will wait until min characters can be read and then return that number of characters
			newoptions.c_cc[VMIN] = 6;
			newoptions.c_cc[VTIME] = 1;

/*			// mikes init
			newoptions.c_cflag     &=  ~PARENB;            //no parity
			newoptions.c_cflag     &=  ~CSTOPB;		//one stop bit,
			newoptions.c_cflag     &=  ~CSIZE;		//clear current size
			newoptions.c_cflag     |=  CS8;		//set 8 bits
			newoptions.c_cflag     &=  ~CRTSCTS;           // no flow control ie. no RTS/CTS
			newoptions.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
			newoptions.c_cc[VMIN]   =  0;                  // read doesn't block, instantly available
			newoptions.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
			newoptions.c_lflag &= ~(ICANON | ECHO | ISIG);
*/
			tcflush(fd, TCIFLUSH);


			if((tcsetattr(fd, TCSANOW, &newoptions)) < 0){
				//fprintf(stderr, "Failed to setattr: %d, %s\n", fd, strerror(errno) );
				printf("Failed to setattr\n");
				fflush(stdout);
				exit(EXIT_FAILURE);
			}

			printf("fd is: %d\n", fd);
			fflush(stdout);

 		while (1) {
 			if ((n = (read(pipefd[READ], buff, sizeof(buff)))) < 0) {//busy wait for something to show up
 		  	    printf ("pipe error");
 				perror ("read error"); //exit(2);
 		  	}

 			//sleep(10);

 			// Classic Warcraft 3 Peasant line
 			printf("More work?\n");
 			fflush(stdout);


 			// send buff over RS-232
 			bytes = write(fd, buff, sizeof(buff));

 			//tcflush(fd,TCIOFLUSH);


 			// echo sent command onto screen for testing purposes
 			write (1, buff, n);
 			fflush(stdout);
 			printf("\n");
 			fflush(stdout);

 			// printf out how many bytes were written, also classic Warcraft 3 Peasant line
 			printf("Job's Done. Wrote %d bytes\n", bytes);
 			fflush(stdout);

/*
 			Ready = select(1, &fdsel, NULL, NULL, &WaitTime);
 						if (Ready == -1)
 							printf("error select");
 						else if (Ready){
 							readbytes = read(fd, buffer, 10);
 						}
 						else
 							printf("error timeout");
*/
 			//sleep(1);
 			// read? doesn't work right now
 			readbytes = read(fd, buffer, 6);

 			printf("Reply is: %s\n", buffer);
 			fflush(stdout);
 			// print out # of bytes read, yet another classic Warcraft 3 Peasant line
 			printf("That's it, I'm dead. (Read %d characters)\n\n", readbytes);
 			fflush(stdout);

 			//printf("That's it, I'm dead. (read characters)\n");

// 			close(fd);

 			//printf("\n");
 		}
 		return (0);
 	}
}

