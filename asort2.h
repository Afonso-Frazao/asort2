#ifndef HEAD_H
#define HEAD_H

// If nothing is defined it will defaut to a random distribution
// Also compile with make math when using the math.h library
//#define GAUSSIAN

#include <stdlib.h>

int asort2(int *arr, unsigned int arr_size, float mem_mult);

unsigned int evaluateArray(int *arr, unsigned int arr_size, int *min, int *max);

unsigned int factor_distribution(int cur_value, unsigned int hash_table_size, unsigned int range, int min);

#endif
