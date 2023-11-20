#ifndef SORT_H
#define SORT_H

#include <stdlib.h>

typedef int (*SortGetFunc)(void* arr, size_t index, void** data);
typedef int (*SortSetFunc)(void* arr, size_t index, void* data);
typedef int (*SortSwapFunc)(void* arr, size_t i, size_t j);
typedef int (*SortCmpFunc)(const void* i, const void* j);

void quick_sort(void *arr, size_t low, size_t high, SortGetFunc get, SortSetFunc set, SortCmpFunc cmp, SortSwapFunc swap);

int binary_search(void *arr, size_t low, size_t high, void *data, SortGetFunc get, SortCmpFunc cmp);

#endif /*SORT_H*/