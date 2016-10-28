/*
 * Sockets.c
 *
 *  Created on: Oct 6, 2016
 *      Author: pierril
 */
#include "netinet/in.h"
#include "stdlib.h"
#include "syslog.h"
#include "Sockets.h"
#include "Protocol.h"
#include "netdb.h"

#include "errno.h"
#define SERVER_IP "192.168.0.108"

int Socket_Init(int processType){
	if (processType == CLIENT){
		return(Socket_Client_Init());
	}
	else {
		return(Socket_Server_Init());
	}
}

int Socket_Server_Init(){
	int listenfd = 0;
	struct sockaddr_in serv_addr;

	//Create socket
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Failed to create socket");
		return(EXIT_FAILURE);
	}

	//Setup
	memset (&serv_addr, 0, sizeof (serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	//Assign Socket fd with local address
	if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == EXIT_FAILURE){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Failed to bind socket");
		return(EXIT_FAILURE);
	}

	//Start responding to requests
	if (listen(listenfd, REQUEST_QUEUE) == EXIT_FAILURE){
		syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Failed to bind socket");
		return(EXIT_FAILURE);
	}
	return(listenfd);
}

int Socket_Client_Init(){
    int sockfd = 0;
    struct sockaddr_in serv_addr;
	struct hostent *host;
	int addr;
	
	host = gethostbyname (SERVER_IP);

	memcpy(&addr, host->h_addr, host->h_length);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
    	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Failed to create socket");
        return(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    //if(inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr)<=0)//todo might cause problem/ my not be needed
    //{
    //	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"inet_pton failed");
    //    return(EXIT_FAILURE);
    //}

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
    	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Failed to connect socket errno:%d", errno);
    	return(EXIT_FAILURE);
    }
    return(sockfd);
}

int Socket_Read(int listenfd, char (Message[PACKET_LEN])){
	int connfd = 0;
	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    if (read(connfd, Message, PACKET_LEN) < PACKET_LEN){
    	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Didnt read whole command from socket");
    	close(connfd);
    	return(EXIT_FAILURE);
    }
    write(connfd,*Message, PACKET_LEN);
    close(connfd);
    return(EXIT_SUCCESS);
}
int Socket_Write(int sockfd, char Message[PACKET_LEN]){
	char returnMessage[PACKET_LEN];
    if (write(sockfd, Message, PACKET_LEN) < PACKET_LEN){
    	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Didnt write whole command from socket");
    	return(EXIT_FAILURE);
    }

    if (read(sockfd,returnMessage, PACKET_LEN) < PACKET_LEN){
    	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Didnt read whole command from socket");
    	return(EXIT_FAILURE);
    }

    if (strcmp(Message,returnMessage) != EXIT_SUCCESS){
    	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Command sent doesnt equal command received");
    	return(EXIT_FAILURE);
    }
    return(EXIT_SUCCESS);
}
//todo close sockets after clients fails

