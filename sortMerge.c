#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/sysinfo.h>

#define KEYSIZE 8
#define DATASIZE 56

//RECSIZE = KEYSIZE+DATASIZE
#define RECSIZE 64

struct Record
{
	char key[KEYSIZE];
	char data[DATASIZE];
};

int main(int argc, char *argv[]){

	int FileSize, nRecs, nRecsPerThread;
	int nThreads = argv[1];
	char c;
	FILE * dataFile;

	if (argc != 3)
	{
		printf("SYNTAX ERROR: sortMege <Number of Threads> <Filename>\n");
		exit(0);
	}
	
	printf("Number of cores for this machine: %d\n", get_nprocs());
	
	dataFile = fopen(argv[2], "r+");
	if (dataFile == NULL)
	{
		
		printf("Cannot Open file\n");
		exit(0);

	}

	fseek(dataFile, 0L, SEEK_END);
	FileSize = ftell(dataFile);
	rewind(dataFile);

	nRecs = FileSize/RECSIZE;


	fclose(dataFile);
	return 0;
}
