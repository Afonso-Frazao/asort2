#include "asort2.h"
#include <stdio.h>

struct unit{
	int value;
	unsigned int count;
};

// The memory multiplier has to be greatest or equal to 1
int asort2(int *arr, unsigned int arr_size, float mem_mult)
{
	if (mem_mult < 1){
		return 1;
	}

	int min, max;

	// Number of different values
	unsigned int diff_value_cnt = evaluateArray(arr, arr_size, &min, &max);
	int range = max - min;

	unsigned int hash_table_size;
	if (range + 1 <= arr_size){
		hash_table_size = (range + 1) * mem_mult;
	} else {
		hash_table_size = arr_size * mem_mult;
	}
	struct unit *hash_table = (struct unit *) calloc(hash_table_size, sizeof(struct unit));
	if (hash_table == NULL){
		return 1;
	}

	unsigned int factor = hash_table_size / range;

	for (unsigned int i = 0; i < arr_size; i++){
		int cur_value = arr[i];
		int index;

		// Search for a empty space to put the next value in
		for (index = (cur_value - min) * factor; index < hash_table_size; index++){
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

		// If this condition is true it means that there are no more empty spaces at the right of the index
		if(index == hash_table_size){
			for (index = (cur_value - min) * factor; index >= 0; index--){
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
		/*if (index < 0){
			return 1;
		}*/
	}

	// Make the used hash table indexes all followed in memory
	for (int i = 0, empty = 0; i < diff_value_cnt; i++){
		while(hash_table[i + empty].count == 0){
			empty++;
		}
		hash_table[i] = hash_table[i + empty];
	}

	// Now I'll use insertion sort to sort the hashed and cleared array
	for(unsigned int i = 1; i < diff_value_cnt; i++){
		struct unit cur_value = hash_table[i];
		int j;
		for(j = i-1; j >= 0; j--){
			if(hash_table[j].value < cur_value.value){
				hash_table[j+1] = cur_value;
				break;
			}
			hash_table[j+1] = hash_table[j];
		}
		if (j == -1){
			hash_table[0] = cur_value;
		}
	}

	for (int i = 0, j = 0; i < diff_value_cnt; i++){
		for (int k = 0; k < hash_table[i].count; k++){
			arr[j] = hash_table[i].value;
			j++;
		}
	}

	free(hash_table);

	return 0;
}

// Return by reference the min and max values of the array
// Return 1 if there is a zero
unsigned int evaluateArray(int *arr, unsigned int arr_size, int *min, int *max)
{
	int min_aux = arr[0];
	int max_aux = arr[0];
	unsigned int zero = 0;
	for (int i = 1; i < arr_size; i++){
		int cur_value = arr[i];
		if (cur_value < min_aux){
			min_aux = cur_value;
		} else if (cur_value > max_aux){
			max_aux = cur_value;
		}
		if (cur_value == 0){
			zero = 1;
		}
	}

	*min = min_aux;
	*max = max_aux;

	return zero;
}
