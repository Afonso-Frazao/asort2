#include <stdio.h>
#include "asort2.h"

int cmpfunc(const void *a, const void *b);

int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("Usage: './asort2 arr_size mem_mult'\n\n");
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
	int *arr1 = (int *) malloc(arr_size * sizeof(int));
	int *arr2 = (int *) malloc(arr_size * sizeof(int));
	int *arr3 = (int *) malloc(arr_size * sizeof(int));
	for (unsigned int i = 0; i < arr_size; i++){
		arr1[i] = rand() % arr_size;
		arr2[i] = arr1[i];
		arr3[i] = arr1[i];
	}

	int ret = asort2((void *) arr2, arr_size, mem_mult);
	if (ret != 0){
		printf("There was an error in the sortting function!\n\n");
		exit(1);
	}

	qsort(arr3, arr_size, sizeof(*arr3), cmpfunc);

	for (int i = 0; i < arr_size; i++){
		if(arr2[i] != arr3[i]){
			printf("There was an error in the sortting function\n");
			printf("seed: %u\n", seed);
			for (int j = 0; j < arr_size; j++){
				printf("%d %d %d\n", arr1[j], arr2[j], arr3[j]);
			}
		}
	}

	free(arr1);
	free(arr2);
	free(arr3);

	return 0;
}

int cmpfunc(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}
