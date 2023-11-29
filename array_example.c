#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "array.h"

// Comparison function for sorting and locating the data.
int data_cmp(void *i, void *j) {
    return *(int*)i - *(int*)j;
}

// Swap function for sorting
int data_swap(void *arr, size_t i, size_t j) {
    void *i_ptr = NULL;
    void *j_ptr = NULL;

    // Get elements at indices i and j
    array_get_by_index(arr, i, &i_ptr);
    array_get_by_index(arr, j, &j_ptr);

    // Swap the elements
    void *temp = i_ptr;
    array_set_by_index(arr, i, j_ptr);
    array_set_by_index(arr, j, temp);
    return OK;
}

// Function to destroy data during array destruction
void data_destroy(void* ctx, void* data) {
    STL_FREE(data);
}

// Function to visit data during array traversal
BOOL data_visit(void* ctx, size_t index, void* data) {
    printf("i:%zu, data:%d\n", index, *(int*)data);
    return TRUE;
}

int main() {
    // Create an array with a data destruction function
    Array *array = array_create(data_destroy, NULL);

    // Seed the random number generator
    unsigned int seed = (unsigned int)(time(NULL) + clock());
    srand(seed);

    int elements_size = 1000000;

    // Populate the array with random integers
    for (int i = 0; i < elements_size; i++) {
        int* value = (int*) STL_MALLOC(sizeof(int));
        *value = rand();
        array_append(array, value);
    }

    // Traverse and print the array before sorting
    array_foreach(array, data_visit, NULL);

    // Sort the array using the defined comparison and swap functions
    array_sort(array, data_cmp, data_swap);

    // Destroy the array, freeing allocated memory
    array_destroy(array);

    return 0;
}
