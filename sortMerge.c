#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/sysinfo.h>

//Parent Process
int main(int argc, char *argv[]){

	char c;
	FILE * dataFile;

	dataFile = fopen(argv[1], "w+");
	if (dataFile == NULL)
	{
		
		printf("Cannot Open file\n");
		exit(0);

	}

	printf("Number of cores for this machine: %d\n", get_nprocs());

	c = fgetc(dataFile);
	while(c != EOF){

		printf("| %c", c);
		c = fgetc(dataFile);

	}

	/*

	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	
	pthread_create(&tid,&attr,PointGen,(void *)argv);

	pthread_join(tid,NULL);


	pthread_exit(NULL);
	*/

	fclose(dataFile);
	return 0;
}

/*void *PointGen(void *argv){




	pthread_exit(NULL);
}
*/
