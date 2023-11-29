#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// Comparison function for locating the data.
int data_cmp(void *i, void *j) {
    return (int)(*(size_t *) i - *(size_t *) j);
}

// Function to destroy data during list destruction
void data_destroy(void* ctx, void* data) {
    STL_FREE(data);
}

// Function to visit data during list traversal
BOOL data_visit(void* ctx, size_t index, void* data) {
    printf("visit index:%zu, data:%d\n", index, *(int*)data);
    return TRUE;
}

int main() {
    // Create a list with a data destruction function
    List *list = list_create(data_destroy, NULL);

    // Seed the random number generator
    unsigned int seed = (unsigned int)(time(NULL) + clock());
    srand(seed);

    int elements_size = 100;

    // Populate the list with random integers
    for (int i = 0; i < elements_size; i++) {
        int *data = (int*) STL_MALLOC(sizeof(int));
        *data = i;
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