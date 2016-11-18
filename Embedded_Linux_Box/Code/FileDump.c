#include "stdlib.h"
#include "stdio.h"

#include "syslog.h"
void GetData(void)
{
	unsigned int seed = 1337;
	FILE *file;
	unsigned int fBuffer[65536];
	unsigned int i = 0;
	srand(seed);

	syslog(LOG_MAKEPRI(LOG_LOCAL0, LOG_INFO),"Logger starting");
	for (i = 0; i < 65534; i++)
	{
		fBuffer[i] = rand();
	}
	
	if ((file = fopen("test.txt", "wb")) == NULL)
	{
		printf("Error opening File");
	}
	else
	{
		fwrite(fBuffer, sizeof(*fBuffer), 65535, file);
	}
	
	fclose(file);
}
