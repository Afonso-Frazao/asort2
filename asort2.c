#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Comment this out for possibly less ram usage for arrays with a wide value range
#define RANGE

struct unit{
	int value;
	unsigned int count;
};

int asort2(int *arr, unsigned int arr_size, float mem_mult);

int normalizeArray(int *arr, unsigned int arr_size);

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

	unsigned int seed;

	// Get a rendom value for the random number generator seed
	fscanf(fp, "%d", &seed);
	srand(seed);

	// I'm using int's to exeplify the function
	int *arr = (int *) malloc(arr_size * sizeof(int));
	for (unsigned int i = 0; i < arr_size; i++){
		arr[i] = rand();
	}

	int ret = asort2((void *) arr, arr_size, mem_mult);
	if (ret != 0){
		printf("There was an error in the sortting function!\n\n");
		exit(1);
	}

	return 0;
}

int asort2(int *arr, unsigned int arr_size, float mem_mult)
{
	if (mem_mult <= 0){
		return 1;
	}

	int range = normalizeArray(arr, arr_size);

	#ifdef RANGE
		struct unit *hash_table = (struct unit *) calloc((int)(range * mem_mult), sizeof(struct unit));
		int hash_table_size = range * mem_mult;
		int hash = range;
	#else
		struct unit *hash_table = (struct unit *) calloc((int)(arr_size * mem_mult), sizeof(struct unit));
		int hash_table_size = arr_size * mem_mult;
		int hash = arr_size;
	#endif
	if (hash_table == NULL){
		return 1;
	}

	// Number of different values
	unsigned int diff_value_cnt = 0;
	for (unsigned int i = 0; i < arr_size; i++){
		int cur_value = arr[i];
		int index = cur_value % hash;
		if (hash_table[index].value == cur_value){
			hash_table[index].count++;
		} else if (hash_table[index].count != 0){

			// Search for a empty space to put the next value in
			for(index++ ; index < hash_table_size; index++){
				if (hash_table[index].value == cur_value){
					hash_table[index].count++;
				}
				else if (hash_table[index].count == 0){
					hash_table[index].value = cur_value;
					hash_table[index].count = 1;
					diff_value_cnt++;
					break;
				}
			}

			// If this condition is true it means that there are no more empty spaces at the right of arr[i] % hash
			if(index == hash_table_size){
				for (index = (cur_value % hash) - 1; index >= 0; index--){
					if (hash_table[index].value == cur_value){
						hash_table[index].count++;
					}
					else if (hash_table[index].count == 0){
						hash_table[index].value = cur_value;
						hash_table[index].count = 1;
						diff_value_cnt++;
						break;
					}
				}
			}

			// If this happens the allocated space was not enouth
			if (index < 0){
				return 1;
			}
		} else {

			// if the cell is empty just put the value there
			hash_table[index].value = cur_value;
			hash_table[index].count = 1;
			diff_value_cnt++;
		}
	}

	return 0;
}

// Returns the array range (max - min) and makes the power value of the array 0
int normalizeArray(int *arr, unsigned int arr_size){
	int min = arr[0];
	int max = arr[0];
	for (int i = 1; i < arr_size; i++){
		int cur_value = arr[i];
		if (cur_value < min){
			min = cur_value;
		} else if (cur_value > max){
			max = cur_value;
		}
	}

	// No need to normalize if the minimum value is already 0
	if (min != 0){
		for (int i = 0; i < arr_size; i++){
			arr[i] -= min;
		}
	}

	return max - min;
}
