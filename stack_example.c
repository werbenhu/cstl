#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

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
