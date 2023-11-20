#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "list.h"

int malloc_n = 0;

int cmp(void *i, void *j) {
    return (int)(*(size_t *) i - *(size_t *) j);
}

int swap(void *arr, size_t i, size_t j) {
    void *i_ptr = NULL;
    void *j_ptr = NULL;

    list_get_by_index(arr, i, &i_ptr);
    list_get_by_index(arr, j, &j_ptr);

    void *temp = i_ptr;
    list_set_by_index(arr, i, j_ptr);
    list_set_by_index(arr, j, temp);

    return OK;
}

void data_destroy(void* ctx, void* data) {
    STL_FREE(data);
}

int data_visit(void* ctx, size_t index, void* data) {
    printf("visit index:%zu, data:%d\n", index, *(int*)data);
    return OK;
}

int main() {
    List *list = list_create(data_destroy, NULL);
    unsigned int seed = (unsigned int)(time(NULL) + clock());
    srand(seed);

    int elements_size = 10;
    for (int i = 0; i < elements_size; i++) {
        int *data = (int*) STL_MALLOC(sizeof(int));
        *data = rand();
        list_append(list, data);
    }
    list_foreach(list, data_visit, NULL);

    list_delete(list, 0);
    printf("size:%zu\n", list_length(list));
    list_destroy(list);

    printf("malloc_n:%d\n", malloc_n);
    return 0;
}
