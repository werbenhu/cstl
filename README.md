
# Common Datastructure Library for **C language**

### Dynamic Array

```c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "array.h"

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

    array_foreach(array, data_visit, NULL);
    array_sort(array, cmp, swap);
    array_destroy(array);
    return 0;
}

```

## List

```c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "list.h"

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
    return 0;
}
```

## Map

```c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include <string.h>

// Comparison function for keys
int cmp(void *i, void *j) {
    return strcmp((char *) i, (char *) j);
}

// Key-Value pair destruction function
void kv_destroy(void* ctx, void* key, void* value) {
    STL_FREE(key);
    STL_FREE(value);
}

// Key-Value pair visit function
int kv_visit(void* ctx, void* key, void* value) {
    printf("ctx:%s, key:%s, value:%s\n", (char*)ctx, (char*)key, (char*)value);
    return OK;
}

// Hash function for keys
int hash(void* key) {
    int k = *(int*)key;
    return k % 10000;
}

int main() {
    // Create a map with key-value destruction function and hash function
    Map *map = map_create(kv_destroy, NULL, hash);

    // Seed the random number generator
    unsigned int seed = (unsigned int)(time(NULL) + clock());
    srand(seed);

    int elements_size = 18;

    // Populate the map with key-value pairs
    for (int i = 0; i < elements_size; i++) {
        // Create a key
        char *key = (char*) STL_MALLOC(16);
        snprintf(key, 16, "key%d", i);

        // Create a value
        char *value = (char*) STL_MALLOC(16);
        snprintf(value, 16, "value%d", i);

        // Set the key-value pair in the map
        map_set(map, key, value);
    }

    // Example of getting a value by key
    char *k = "key11";
    void* val = NULL;
    if (map_get(map, cmp, k, &val) != OK) {
        printf("can't find key:%s in map\n", k);
    }

    // Example of deleting keys
    for (int i = 0; i < 6; i++) {
        char key[16] = {0};
        snprintf(key, 16, "key%d", i);
        map_delete(map, cmp, key);
    }

    // Example of traversing the map and printing key-value pairs
    char *ctx = "visit-ctx";
    map_foreach(map, kv_visit, ctx);

    printf("val:%s\n", (char*)val);

    // Destroy the map, freeing allocated memory
    map_destroy(map);
    return 0;
}
```