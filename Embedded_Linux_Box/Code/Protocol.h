/*
 * Protocol.h
 *
 *  Created on: Oct 2, 2016
 *      Author: pierril
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

//Protocol
#define PACKET_LEN 	9
#define FUNCT	2
#define ARG1	4
#define ARG2	6
//EX. "~ F 0 0 ?"

//Actions
#define SERVO		'O' //D-pad up-down
#define STEPPER		'S' //D-pad left-right
#define L_MOTOR		'L' //Left stick up-down
#define R_MOTOR 	'R' //Right stick up-down
#define START		'X' //start button / collect data 
#define SELECT 		'Y' //Back button TODO:rename if action is defined
#define A_BUTTON 	'A' //... TODO:rename if action is defined
#define B_BUTTON 	'B' //... TODO:rename if action is defined
#define Y_BUTTON 	'C' //... TODO:rename if action is defined
#define X_BUTTON 	'D' //... TODO:rename if action is defined
#define R_BUTTON 	'E' //... TODO:rename if action is defined
#define R_TRIGGER 	'F' //... TODO:rename if action is defined
#define L_BUTTON 	'G' //... TODO:rename if action is defined
#define L_TRIGGER 	'H' //... TODO:rename if action is defined
#define DEFAULT 	'F' //Unknown command
//Directional values
#define UP			'1'
#define DOWN		'2'
#define LEFT		'1'
#define RIGHT		'2'
#define ZEROED		'0' //Centered
//Speed translation
#define MAX_A_JS 	36410	//Max values under joy stick protocol
#define VAR_SPEEDS	2		//Number of different speeds to translate to
#define BINSIZE 	(MAX_A_JS/VAR_SPEEDS)

#endif /* PROTOCOL_H_ */


