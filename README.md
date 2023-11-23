
# Common Datastructure Library for **C language**

<div>
<a href="https://github.com/werbenhu/cstl/actions"><img src="https://img.shields.io/github/actions/workflow/status/werbenhu/cstl/cmake-single-platform.yml?branch=master&label=Build"></a>
</div>

### Dynamic Array

```c
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "array.h"

// Comparison function for sorting
int cmp(void *i, void *j) {
    return *(int*)i - *(int*)j;
}

// Swap function for sorting
int swap(void *arr, size_t i, size_t j) {
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
int data_visit(void* ctx, size_t index, void* data) {
    printf("i:%zu, data:%d\n", index, *(int*)data);
    return OK;
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
    array_sort(array, cmp, swap);

    // Destroy the array, freeing allocated memory
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
```

## Stack
```c
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Comparison function for sorting
int data_cmp(void *i, void *j) {
    return (int)(*(size_t *) i - *(size_t *) j);
}

// Function to destroy data during stack destruction
void data_destroy(void* ctx, void* data) {
    STL_FREE(data);
}

// Function to visit data during stack traversal
int data_visit(void* ctx, size_t index, void* data) {
    printf("Visit index:%zu, Data:%d\n", index, *(int*)data);
    return OK;
}

int main() {
    // Create a stack with a data destruction function
    Stack *stack = stack_create(data_destroy, NULL);

    int elements_size = 10;

    // Populate the stack with random integers
    for (int i = 0; i < elements_size; i++) {
        int *data = (int*) STL_MALLOC(sizeof(int));
        *data = rand();
        stack_push(stack, data);
    }

    printf("Size:%zu\n", stack_length(stack));
    // Traverse and print the stack before deletion
    stack_foreach(stack, data_visit, NULL);

    for (int i = 0; i < elements_size; i++) {
        int* top = NULL;
        stack_top(stack, (void**)&top);
        printf("Top:%d\n", *top);
        stack_pop(stack);
        printf("Size:%zu\n", stack_length(stack));
    }

    // Destroy the stack, freeing allocated memory
    stack_destroy(stack);
    return 0;
}
```

## Queue
```c
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Comparison function for sorting data in the queue
int data_cmp(void *i, void *j) {
    return (int)(*(size_t *) i - *(size_t *) j);
}

// Function to destroy data during list destruction
void data_destroy(void* ctx, void* data) {
    STL_FREE(data);
}

// Function to visit data during list traversal
int data_visit(void* ctx, size_t index, void* data) {
    printf("Visited index:%zu, data:%d\n", index, *(int*)data);
    return OK;
}

int main() {
    // Create a queue with a data destruction function
    Queue *queue = queue_create(data_destroy, NULL);

    int elements_size = 10;

    // Populate the queue with random integers
    for (int i = 0; i < elements_size; i++) {
        int *data = (int*) STL_MALLOC(sizeof(int));
        *data = rand();
        queue_push(queue, data);
    }

    printf("Queue size:%zu\n", queue_length(queue));

    // Traverse and print the queue before dequeueing
    printf("Queue elements before dequeueing:\n");
    queue_foreach(queue, data_visit, NULL);

    // Dequeue and print the head of the queue
    for (int i = 0; i < elements_size; i++) {
        int* head = NULL;
        queue_head(queue, (void**)&head);
        printf("Dequeued head:%d\n", *head);
        queue_pop(queue);
        printf("Queue size after dequeueing:%zu\n", queue_length(queue));
    }

    // Destroy the queue, freeing allocated memory
    queue_destroy(queue);
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