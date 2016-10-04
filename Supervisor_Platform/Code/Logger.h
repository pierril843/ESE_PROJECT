/*
 * Logger.h
 *
 *  Created on: Oct 3, 2016
 *      Author: pierril
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include "stdio.h"

//log protocol "T"	"mess"			"pid"		"date/time"
// T - log type. E = Event, D = DataDump, C = Crash
// mess - message stating what happened or the data
// pid - procces id of the procces calling
// date/time - date and time of log
//separate all by 1,2,and 2 tabs

#define LOG "/home/pierril/Documents/gitfolder/ESE_PROJECT/Supervisor_Platform/Documentation/Superviser_Platform.log"

//log Type
#define EVENT 	'E'
#define DATA  	'D'
#define CRASH	'C'

#define PROCESS_LEN 10
#define FILE_ERROR 3
#define FILE_EXISTS_ERROR 5
#define ACCESS_ERROR 7

int Create_Log(const char[]);
int Time_Stamp(FILE *File);
int Logger(const char[],char, const char*, int);

#endif /* LOGGER_H_ */
