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

#include "syslog.h"
#include "RS232.h"
#include "Sockets.h"
#include "FileDump.h"
int main() {
	char Command[9];
	char EchoCommand[9];
	int rs232fd, socketfd;
	int iobytes;
	if ((rs232fd = Open_Port(RS232PORT)) != EXIT_FAILURE){
		if (Setup_Port(rs232fd) == EXIT_FAILURE){
			syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"RS-232 port setup failed");
			exit(EXIT_FAILURE);
		}
	}
	else{
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Open access to RS-232 port failed, fd:%d",rs232fd);
		exit(EXIT_FAILURE);
	}

	if ((socketfd = Socket_Init(SERVER)) == EXIT_FAILURE){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Socket setup failed");
		exit(EXIT_FAILURE);
	}

	while (1) {
		//todo check if thread is still running exit and log if not
		if ((Socket_Read(socketfd,Command)) == EXIT_FAILURE) {//busy wait for something to show up
			printf ("socket read error\n");
			syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"socket read failed");
		}
		else{
			if ((Command[FUNCT] == 'X')&(Command[ARG1] == '1')){
				GetData();
			}
			if ((iobytes = write(rs232fd, Command, PACKET_LEN)) < 0){
				printf ("RS-232 write error\n");
				syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"RS-232 write failed");
			}
			syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO),"Command Sent:%s", Command);


		}
	}

	return EXIT_SUCCESS;
}
