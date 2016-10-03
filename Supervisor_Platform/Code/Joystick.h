/*
 * Joystick.h
 *
 *  Created on: Oct 3, 2016
 *      Author: pierril
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#define JOY_DEV "/dev/input/js0"

int MortorValue(int);
void MonitorJoyStick (int[]);

#endif /* JOYSTICK_H_ */
