#include "stack.h"
#include "list.h"
#include "typedef.h"

// Function to create a new stack
Stack* stack_create(DataDestroyFunc data_destroy, void* ctx) {
    Stack* self = (Stack*)STL_MALLOC(sizeof(Stack));
    if (self != NULL) {
        if ((self->list = list_create(data_destroy, ctx)) == NULL) {
            STL_FREE(self);
        }
    }
    return self;
}

// Function to get the element at the top of the stack without removing it
int stack_top(Stack* self, void** data) {
    return_val_if_fail(self != NULL && data != NULL, ERR_NIL);
    return list_get_by_index(self->list, 0, data);
}

// Function to push an element onto the stack
int stack_push(Stack* self, void* data) {
    return_val_if_fail(self != NULL, ERR_NIL);
    return list_prepend(self->list, data);
}

// Function to pop the top element from the stack
int stack_pop(Stack* self) {
    return_val_if_fail(self != NULL, ERR_NIL);
    return list_delete(self->list, 0);
}

// Function to get the current length (number of elements) in the stack
size_t stack_length(Stack* self) {
    return_val_if_fail(self != NULL, 0);
    return list_length(self->list);
}

// Function to apply a visit function to each element in the stack
int stack_foreach(Stack* self, DataVisitFunc visit, void* ctx) {
    return_val_if_fail(self != NULL && visit != NULL, ERR_NIL);
    return list_foreach(self->list, visit, ctx);
}

// Function to destroy the stack
void stack_destroy(Stack* self) {
    if (self != NULL) {
        list_destroy(self->list);
        self->list = NULL;
        STL_FREE(self);
    }
    return;
}
