/*
 * Sockets.h
 *
 *  Created on: Oct 6, 2016
 *      Author: pierril
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_
#include "Protocol.h"

#define REQUEST_QUEUE 5
#define SERVER 0
#define CLIENT	1

int Socket_Init(int);
int Socket_Server_Init();
int Socket_Client_Init();
int Socket_Read(int, char([]));
int Socket_Write(int, char[]);

#endif /* SOCKETS_H_ */
//Process type
