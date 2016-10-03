/*
 * RS232.h
 *
 *  Created on: Oct 2, 2016
 *      Author: pierril
 */

#ifndef RS232_H_
#define RS232_H_

#define RS232PORT "/dev/ttyUSB0" // where the serial port is

int Setup_Port (int);
int Open_Port (const char);
int Close_Port (int);

#endif /* RS232_H_ */
