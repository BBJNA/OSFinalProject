#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/sysinfo.h>

#define KEYSIZE 8
#define DATASIZE 56
#define RECSIZE 64

typedef struct
{
	char key[KEYSIZE];
	char data[DATASIZE];
}Record;

typedef struct
{
		
		int thdNum;
		Record * lowRec;
		Record * hiRec;

}thrdArg;

int cmpfunc(const void *a, const void *b);
void printRecord(Record *recptr, int nRecs);
void *SortData(void *thrdArg);

int main(int argc, char *argv[]){
	clock_t start = clock();
	int FileSize, nRecs, nRecsPerThread;
	char c;
	FILE * dataFile;

	if (argc != 3)
	{
		printf("SYNTAX ERROR: sortMege <Number of Threads> <Filename>\n");
		exit(0);
	}

	///////////////////////////////////////////////////
	//         Preparing the Data for Sorting        //
	///////////////////////////////////////////////////
	
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

	Record *RecStartPtr = (Record *)malloc(nRecs*sizeof(Record));
	char *RecArrPtr = RecStartPtr;


	for (int i = 0; i < nRecs*sizeof(Record); i++)
	{	
		*(RecArrPtr++) = fgetc(dataFile);
	}
	
	//printRecord(RecStartPtr, nRecs);

	///////////////////////////////////////////////////
	//        Sorting with Multiple Threads          //
	///////////////////////////////////////////////////
	
	int nThreads = atoi(argv[1]);

	nRecsPerThread = nRecs/nThreads;

	printf("%d\n", nRecsPerThread);

	thrdArg *SortThreads[nThreads];
	pthread_t *p_tids[nThreads];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	(*SortThreads[0]).lowRec = (Record *)RecStartPtr;
	(*SortThreads[0]).hiRec = (Record *)RecStartPtr+1;

	SortData(SortThreads[0]);

	//printRecord(RecStartPtr, nRecs);

	fclose(dataFile);
	free(RecStartPtr);
	clock_t stop = clock();
	double TimeTaken = (double)(stop-start)/CLOCKS_PER_SEC;
	printf("\nThe sort took %f seconds\n", TimeTaken);
	return 0;
}

int cmpfunc(const void *a, const void *b){
	
	Record *TempA = (Record *)a;
	Record *TempB = (Record *)b;
	char tempStrA[KEYSIZE+1], tempStrB[KEYSIZE+1];
	
	tempStrA[KEYSIZE+1] = '\0';
	tempStrB[KEYSIZE+1] = '\0';
	
	memcpy(tempStrA, (*TempA).key, KEYSIZE);
	memcpy(tempStrB, (*TempB).key, KEYSIZE);

	return strcmp(tempStrA,tempStrB);

}

void printRecord(Record *recptr, int nRecs){

	for (int i = 0; i < nRecs; ++i)
	{
		for (int j = 0; j < RECSIZE; ++j)
		{
			printf("%c", (*(recptr+i)).key[j]);
		}
	}
}
void *SortData(void *storeArg){

	thrdArg *temp = (thrdArg *)storeArg;
	int nRecords = ((int)((*temp).hiRec)-(int)((*temp).lowRec))/sizeof(Record);
	printf("%d\n", nRecords);

	//qsort(, nRecords, RECSIZE, cmpfunc);

}