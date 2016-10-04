/*
 * Logger.c
 *
 *  Created on: Oct 3, 2016
 *      Author: pierril
 */
//log protocol "T"	"mess"			"pid"		"date/time"
// T - log type. E = Event, D = DataDump, C = Crash
// mess - message stating what happened or the data
// pid - procces id of the procces calling
// date/time - date and time of log
//separate all by 1,2,and 2 tabs

#include "Logger.h"
#include "unistd.h"
#include "errno.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

//Create log file
int Create_Log(const char FilePath[]){
	FILE *File;
	char *Time;
	if (access(FilePath, F_OK) != -1 ){
		return(FILE_EXISTS_ERROR);
	}
	File = fopen(FilePath,"w");
	fprintf(File,"Log Location: \t%s\n", FilePath);
	fprintf(File,"Time Created: \t");
	Time_Stamp(File);
	fclose(File);
	return(EXIT_SUCCESS);
}

int Time_Stamp(FILE *File){
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	fprintf (File, "%s", asctime(timeinfo));
	return EXIT_SUCCESS;
}

//write log to log file
//procces id, date/time, type Event Error Crash
int Logger(const char FilePath[],char Type, const char* LogMesg, int pid){
	FILE *File;
	const char *Time;
	if (access(FilePath, W_OK) == EXIT_SUCCESS){
		File = fopen(FilePath,"a");
		if (fprintf(File, "%c\t%s\t\t%d\t\t", Type, LogMesg, pid) != EXIT_FAILURE){
			Time_Stamp(File);
			fclose(File);
			return(EXIT_SUCCESS);
		}
	}
	return(ACCESS_ERROR);
}
//clear log file
int Clear_Log (const char FilePath[]){
	FILE *File;
		char *Time;
		if (access(FilePath, F_OK) != -1 ){
			File = fopen(FilePath,"w");
			fprintf(File,"Log Location: \t%s\n", FilePath);
			fprintf(File,"Time Created: \t");
			Time_Stamp(File);
			fclose(File);
			return(EXIT_SUCCESS);
		}
		return(FILE_ERROR);
}

