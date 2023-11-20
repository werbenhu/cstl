#ifndef SORT_H
#define SORT_H

#include <stdlib.h>

// Function pointer type for getting data at a specific index in an array
typedef int (*SortGetFunc)(void* arr, size_t index, void** data);

// Function pointer type for setting data at a specific index in an array
typedef int (*SortSetFunc)(void* arr, size_t index, void* data);

// Function pointer type for swapping elements at two indices in an array
typedef int (*SortSwapFunc)(void* arr, size_t i, size_t j);

// Function pointer type for comparing two elements in an array
typedef int (*SortCmpFunc)(const void* i, const void* j);

// Function to perform quick sort on an array
void quick_sort(void *arr, size_t low, size_t high, SortGetFunc get, SortSetFunc set, SortCmpFunc cmp, SortSwapFunc swap);

// Function to perform binary search on a sorted array
int binary_search(void *arr, size_t low, size_t high, void *data, SortGetFunc get, SortCmpFunc cmp);

#endif /*SORT_H*/
