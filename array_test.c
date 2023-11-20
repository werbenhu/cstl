#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "array.h"

int malloc_n = 0;

int cmp(void *i, void *j) {
    return *(int*)i-*(int*)j;
}

int swap(void *arr, size_t i, size_t j) {
    void *i_ptr = NULL;
    void *j_ptr = NULL;

    array_get_by_index(arr, i, &i_ptr);
    array_get_by_index(arr, j, &j_ptr);

    void *temp = i_ptr;
    array_set_by_index(arr, i, j_ptr);
    array_set_by_index(arr, j, temp);
    return OK;
}

void data_destroy(void* ctx, void* data) {
    STL_FREE(data);
}

int data_visit(void* ctx, size_t index, void* data) {
    printf("i:%zu, data:%d\n", index, *(int*)data);
    return OK;
}

int main() {
    Array *array = array_create(data_destroy, NULL);
    unsigned int seed = (unsigned int)(time(NULL) + clock());
    srand(seed);

    int elements_size = 1000000;
    for (int i = 0; i < elements_size; i++) {
        int* value = (int*) STL_MALLOC(sizeof(int));
        *value = rand();
        array_append(array, value);
    }
//    array_foreach(array, data_visit, NULL);

    clock_t start_time = clock();
    array_sort(array, cmp, swap);
    clock_t end_time = clock();

    double duration = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    printf("sorted in: %f ms\n", duration);

//    printf("print sorted elements");
//    array_foreach(array, data_visit, NULL);
    array_destroy(array);

    printf("malloc_n:%d\n", malloc_n);
    return 0;
}
