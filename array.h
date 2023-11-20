#include <stdio.h>
#include "typedef.h"

#ifndef ARRAY_H
#define ARRAY_H

// Define the structure for a dynamic array
typedef struct {
    void **data;
    size_t size;
    size_t alloc_size;

    void *data_destroy_ctx;
    DataDestroyFunc data_destroy;
} Array;

// Create a new dynamic array
Array* array_create(DataDestroyFunc data_destroy, void* ctx);

// Insert data at the specified index
int array_insert(Array* self, size_t index, void* data);

// Insert data at the beginning of the array
int array_prepend(Array* self, void* data);

// Append data to the end of the array
int array_append(Array* self, void* data);

// Delete data at the specified index
int array_delete(Array* self, size_t index);

// Get data by index
int array_get_by_index(Array* self, size_t index, void** data);

// Set data at the specified index
int array_set_by_index(Array* self, size_t index, void* data);

// Get the length of the array
size_t array_length(Array* self);

// Find the index of data that satisfies a condition
int array_find(Array* self, DataCompareFunc cmp, void* ctx);

// Apply a visiting function to the data in the array
int array_foreach(Array* self, DataVisitFunc visit, void* ctx);

// Sort the data in the array
int array_sort(Array* self, DataCompareFunc cmp, DataSwapFunc swp);

// Destroy the dynamic array and release resources
void array_destroy(Array* self);

#endif /*ARRAY_H*/