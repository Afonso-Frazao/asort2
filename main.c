#include <stdio.h>
#include "asort2.h"

int cmpfunc(const void *a, const void *b);

void insrSort(int *arr, unsigned int arr_size);

void countSort(int inputArray[], int arr_size);

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

	switch (argv[3][0]) {
		case 'a':
			int ret = asort2((void *) arr, arr_size, mem_mult);
			if (ret != 0){
				printf("There was an error in the sortting function!\n\n");
				exit(1);
			}
			break;
		case 'q':
			qsort(arr, arr_size, sizeof(*arr), cmpfunc);
			break;
		case 'i':
			insrSort(arr, arr_size);
			break;
		case 'c':
			countSort(arr, arr_size);
			break;
		default:
			printf("Invalid function\n\n");
			exit(1);
	}

	free(arr);

	return 0;
}

int cmpfunc(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

void insrSort(int *arr, unsigned int arr_size)
{
	for(unsigned int i = 1; i < arr_size; i++){
		unsigned int cur_value = arr[i];
		int j;
		for(j = i-1; j >= 0; j--){
			if(arr[j] < cur_value){
				arr[j+1] = cur_value;
				break;
			}
			arr[j+1] = arr[j];
		}
		if (j == -1){
			arr[0] = cur_value;
		}
	}
}

// Credits to https://www.geeksforgeeks.org/counting-sort/
void countSort(int *inputArray, int arr_size)
{
  
    // Finding the maximum element of 
    // array inputArray[]
    int M = 0;
    for (int i = 0; i < arr_size; i++)
        if (inputArray[i] > M)
            M = inputArray[i];
    
    // Initializing countArray[] with 0
    int* countArray = (int*)calloc(M + 1, sizeof(int));
    
    // Mapping each element of inputArray[] 
    // as an index of countArray[] array
    for (int i = 0; i < arr_size; i++)
        countArray[inputArray[i]]++;
    
    // Calculating prefix sum at every index
    // of array countArray[]
    for (int i = 1; i <= M; i++)
        countArray[i] += countArray[i - 1];
    
    // Creating outputArray[] from countArray[] array
    int* outputArray = (int*)malloc(arr_size * sizeof(int));
    for (int i = arr_size - 1; i >= 0; i--) {
        outputArray[countArray[inputArray[i]] - 1] = inputArray[i];
        countArray[inputArray[i]]--;
    }
    
    // Copying sorted elements back to inputArray[]
    for (int i = 0; i < arr_size; i++)
        inputArray[i] = outputArray[i];
    
    // Freeing dynamically allocated memory
    free(countArray);
    free(outputArray);
}
