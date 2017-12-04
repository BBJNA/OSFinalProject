//gcc -o sortMerge sortMerge.c -lpthread
//./sortMerge 8 data_128

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
void printRecords(Record *recptr, int nRecs);
void *SortData(void *thrdArg);

int main(int argc, char *argv[]){
	clock_t start = clock();
	int FileSize, nRecs, nRecsPerThread, count, tThreadCount=0, level=0;
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

	printf("Records per Thread: %d\n", nRecsPerThread);


	count = nThreads;
	while(count != 0){

		tThreadCount += count;
		count = count/2;

	}

	printf("%d\n", level);

	pthread_t threads[tThreadCount];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	thrdArg SortThreads[tThreadCount];

/////////////////////////////////////////////////////////////////////////////////////////
	//MODIFY nRecsPerThread?????????????
/////////////////////////////////////////////////////////////////////////////////////////
	
	for (int i = 0; i < tThreadCount; i++)
	{
		
		if ( tThreadCount == nThreads)
		{
			/* code */
		}

		SortThreads[i].lowRec = (Record *)(RecStartPtr+i*(nRecsPerThread));
		SortThreads[i].hiRec = (Record *)(RecStartPtr+(i+1)*nRecsPerThread);
	
	}
		
	for (int i = 0; i < tThreadCount; i++)
	{

		pthread_create(&threads[i], &attr, SortData, &SortThreads[i]);
		pthread_join(threads[i], NULL);

	}

	//printRecords(RecStartPtr, nRecs);

	fclose(dataFile);
	free(RecStartPtr);
	clock_t stop = clock();
	double TimeTaken = (double)(stop-start)/CLOCKS_PER_SEC;
	printf("\nThe sort took %f seconds\n", TimeTaken);

	pthread_exit(NULL);

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

void printRecords(Record *recptr, int nRecs){

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
	qsort((*temp).lowRec, nRecords, RECSIZE, cmpfunc);

}