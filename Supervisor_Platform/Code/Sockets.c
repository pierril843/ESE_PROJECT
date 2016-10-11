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

int Socket_Init(){
    int listenfd = 0;
    struct sockaddr_in serv_addr;

    //Create socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == EXIT_FAILURE){
    	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Failed to create socket");
    	return(EXIT_FAILURE);
    }

    //Setup
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

int Socket_Write(int listenfd, char Message[PACKET_LEN]){
	int connfd = 0;
	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    if (write(connfd, Message, PACKET_LEN) < PACKET_LEN){
    	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_ERR),"Didnt write whole command to socket");
    	return(EXIT_FAILURE);
    }
    close(connfd);
    return(EXIT_SUCCESS);
}
