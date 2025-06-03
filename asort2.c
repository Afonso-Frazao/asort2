/*
 * MIT License
 * Copyright (c) 2025 Afonso Frazão
 *
 * This file is part of asort.
*/

#include "asort2.h"
#include <stdio.h>

#ifdef GAUSSIAN
	#include <math.h>
#endif

struct unit {
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
	unsigned int range = max - min;

	unsigned int hash_table_size;
	if (range + 1 <= arr_size) {
		hash_table_size = (range + 1) * mem_mult;
	} else {
		hash_table_size = arr_size * mem_mult;
	}
	struct unit *hash_table = (struct unit *) calloc(hash_table_size, sizeof(struct unit));
	if (hash_table == NULL) {
		return 1;
	}

	for (unsigned int i = 0; i < arr_size; i++) {
		int cur_value = arr[i];

		// Search for a empty space to put the next value in
		// The factor is choosen based on the expected distribution of the array values
		unsigned int factor = factor_distribution(cur_value, hash_table_size, range, min);
		for (unsigned int index = (cur_value - min) * factor, search_depth = 1; index < hash_table_size; index += search_depth, search_depth = -(search_depth + 1)) {
			if (hash_table[index].value == cur_value) {
				hash_table[index].count++;
				break;
			}
			else if (hash_table[index].count == 0) {
				hash_table[index].value = cur_value;
				hash_table[index].count = 1;
				diff_value_cnt++;
				break;
			}
		}
	}

	// Make the used hash table indexes all followed in memory
	for (int i = 0, empty = 0; i < diff_value_cnt; i++) {
		while(hash_table[i + empty].count == 0) {
			empty++;
		}
		hash_table[i] = hash_table[i + empty];
	}

	// Now I'll use insertion sort to sort the hashed and cleared array
	for(unsigned int i = 1; i < diff_value_cnt; i++) {
		struct unit cur_value = hash_table[i];
		int j;
		for(j = i-1; j >= 0; j--){
			if(hash_table[j].value < cur_value.value) {
				hash_table[j+1] = cur_value;
				break;
			}
			hash_table[j+1] = hash_table[j];
		}
		if (j == -1){
			hash_table[0] = cur_value;
		}
	}

	// Copy the sorted values from the hash table to the original array
	for (int i = 0, j = 0; i < diff_value_cnt; i++) {
		for (int k = 0; k < hash_table[i].count; k++) {
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

unsigned int factor_distribution(int cur_value, unsigned int hash_table_size, unsigned int range, int min)
{
	unsigned int factor;

	#ifdef GAUSSIAN
		// The erf function is the primite of the pdf
		factor = hash_table_size / erf((double) cur_value - min);
	#else
		factor = hash_table_size / range;
	#endif

	return factor;
}
