#include <stdio.h>
#include "asort2.h"

int cmpfunc(const void *a, const void *b);

int main(int argc, char *argv[])
{
	if (argc != 4){
		printf("Usage: './asort2 arr_size mem_mult a/q'\n\n");
		exit(1);
	}

	// Get the size of the random number array
	unsigned int arr_size = atoi(argv[1]);

	// Get the memory multiplier
	float mem_mult = atof(argv[2]);

	FILE *fp;
	fp = fopen("/dev/urandom", "r");
	if (fp == NULL){
		printf("Error opening /dev/urandom\n\n");
		exit(1);
	}

	unsigned int seed;

	// Get a rendom value for the random number generator seed
	fread(&seed, 1, sizeof(seed), fp);
	srand(seed);

	// I'm using int's to exeplify the function
	int *arr = (int *) malloc(arr_size * sizeof(int));
	for (unsigned int i = 0; i < arr_size; i++){
		arr[i] = rand() % arr_size;
	}

	fclose(fp);

	if (argv[3][0] == 'a'){
		int ret = asort2((void *) arr, arr_size, mem_mult);
		if (ret != 0){
			printf("There was an error in the sortting function!\n\n");
			exit(1);
		}
	}

	if (argv[3][0] == 'q'){
		qsort(arr, arr_size, sizeof(*arr), cmpfunc);
	}

	free(arr);

	return 0;
}

int cmpfunc(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}
