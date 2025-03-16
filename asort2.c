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

int normalizeArray(int *arr, unsigned int arr_size, int *zero);

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
	printf("seed: %u\n", seed);
	srand(seed);

	// I'm using int's to exeplify the function
	int *arr = (int *) malloc(arr_size * sizeof(int));
	for (unsigned int i = 0; i < arr_size; i++){
		arr[i] = rand() % arr_size;
	}

	int ret = asort2((void *) arr, arr_size, mem_mult);
	if (ret != 0){
		printf("There was an error in the sortting function!\n\n");
		exit(1);
	}

	return 0;
}

// The memory multiplier has to be greatest or equal to 1
int asort2(int *arr, unsigned int arr_size, float mem_mult)
{
	if (mem_mult < 1){
		return 1;
	}

	// Get a flag for if there is a 0 value on the array
	int zero = 0;
	int range = normalizeArray(arr, arr_size, &zero);

	#ifdef RANGE
		unsigned int hash_table_size = (int) (range * mem_mult);
		struct unit *hash_table = (struct unit *) calloc(hash_table_size, sizeof(struct unit));
		int hash = range;
	#else
		int hash_table_size = (int) (arr_size * mem_mult);
		struct unit *hash_table = (struct unit *) calloc(hash_table_size, sizeof(struct unit));
		int hash = arr_size;
	#endif
	if (hash_table == NULL){
		return 1;
	}

	// Number of different values
	unsigned int diff_value_cnt = 0;

	// If there is a zero value on the array it won't be accountted without this condition
	if (zero == 1){
		diff_value_cnt++;
	}

	for (unsigned int i = 0; i < arr_size; i++){
		int cur_value = arr[i];
		int index = cur_value % hash;

		// Search for a empty space to put the next value in
		// This will work because hash is always smaller or equal than hash_table_size
		while(index < hash_table_size){
			if (hash_table[index].value == cur_value){
				hash_table[index].count++;
				break;
			}
			else if (hash_table[index].count == 0){
				hash_table[index].value = cur_value;
				hash_table[index].count = 1;
				diff_value_cnt++;
				break;
			}

			index++;
		}

		// If this condition is true it means that there are no more empty spaces at the right of arr[i] % hash
		if(index == hash_table_size){
			for (index = (cur_value % hash) - 1; index >= 0; index--){
				if (hash_table[index].value == cur_value){
					hash_table[index].count++;
					break;
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
	}

	struct unit *cleared_hash_table = (struct unit *) malloc(diff_value_cnt * sizeof(struct unit));
	for (unsigned int i = 0, j = 0; i < hash_table_size; i++){
		if (hash_table[i].count != 0){
			cleared_hash_table[j] = hash_table[i];
			j++;
		}
	}

	printf("diff value count: %d\n", diff_value_cnt);

	for (int i = 0; i < arr_size; i++){
		printf("%d\t", arr[i]);
	}
	printf("\n\n");

	for (int i = 0; i < hash_table_size; i++){
		printf("%d\t", hash_table[i].value);
	}
	printf("\n\n");

	for (int i = 0; i < diff_value_cnt; i++){
		printf("%d\t", cleared_hash_table[i].value);
	}
	printf("\n\n");

	// Now I'll use insertion sort to sort the hashed and cleared array
	for(unsigned int i = 1; i < diff_value_cnt; i++){
		struct unit cur_value = cleared_hash_table[i];
		for(int j = i-1; j >= 0; j--){
			if(cleared_hash_table[j].value < cur_value.value){
				cleared_hash_table[j+1] = cur_value;
				break;
			}
			cleared_hash_table[j+1] = cleared_hash_table[j];
		}
	}

	for (int i = 0; i < diff_value_cnt; i++){
		printf("%d\t", cleared_hash_table[i].value);
	}
	printf("\n\n");

	return 0;
}

// Returns the array range (max - min) and makes the power value of the array 0
int normalizeArray(int *arr, unsigned int arr_size, int *zero){
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
			if (arr[i] == 0){
				*zero = 1;
			}
		}
	} else {
		for (int i = 0; i < arr_size; i++){
			if (arr[i] == 0){
				*zero = 1;
				break;
			}
		}
	}

	return max - min;
}
