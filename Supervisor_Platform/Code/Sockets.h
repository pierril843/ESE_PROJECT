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

int Socket_Init();
int Socket_Write(int listenfd, char Message[PACKET_LEN]);

#endif /* SOCKETS_H_ */
