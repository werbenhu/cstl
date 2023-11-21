#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "list.h"

// Comparison function for sorting
int cmp(void *i, void *j) {
    return (int)(*(size_t *) i - *(size_t *) j);
}

// Swap function for sorting
int swap(void *arr, size_t i, size_t j) {
    void *i_ptr = NULL;
    void *j_ptr = NULL;

    // Get elements at indices i and j
    list_get_by_index(arr, i, &i_ptr);
    list_get_by_index(arr, j, &j_ptr);

    // Swap the elements
    void *temp = i_ptr;
    list_set_by_index(arr, i, j_ptr);
    list_set_by_index(arr, j, temp);

    return OK;
}

// Function to destroy data during list destruction
void data_destroy(void* ctx, void* data) {
    STL_FREE(data);
}

// Function to visit data during list traversal
int data_visit(void* ctx, size_t index, void* data) {
    printf("visit index:%zu, data:%d\n", index, *(int*)data);
    return OK;
}

int main() {
    // Create a list with a data destruction function
    List *list = list_create(data_destroy, NULL);

    // Seed the random number generator
    unsigned int seed = (unsigned int)(time(NULL) + clock());
    srand(seed);

    int elements_size = 10;

    // Populate the list with random integers
    for (int i = 0; i < elements_size; i++) {
        int *data = (int*) STL_MALLOC(sizeof(int));
        *data = rand();
        list_append(list, data);
    }

    // Traverse and print the list before deletion
    list_foreach(list, data_visit, NULL);

    // Delete the element at index 0
    list_delete(list, 0);
    printf("size:%zu\n", list_length(list));

    // Destroy the list, freeing allocated memory
    list_destroy(list);
    return 0;
}