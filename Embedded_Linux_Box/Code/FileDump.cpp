//Hello World it's me Mario
#include <stdio.h>
#include <stdlib.h>



void main(void)
{
	unsigned int seed = 1337;
	FILE *file;
	unsigned int fBuffer[65536];
	unsigned int i = 0;
	srand(seed);

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
	system("PAUSE");
}