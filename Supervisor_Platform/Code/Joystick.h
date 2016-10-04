/*
 * Joystick.h
 *
 *  Created on: Oct 3, 2016
 *      Author: pierril
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#define JOY_DEV "/dev/input/js0"
//todo add mutex
int MortorValue(int);
void* MonitorJoyStick (void*);

#endif /* JOYSTICK_H_ */
