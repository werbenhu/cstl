#ifndef STACK_H
#define STACK_H

#include "typedef.h"
#include "list.h"

// Structure representing a stack
typedef struct {
    List* list; // Underlying list to store stack elements
} Stack;

// Function to create a new stack
Stack* stack_create(DataDestroyFunc data_destroy, void* ctx);

// Function to get the element at the top of the stack without removing it
int stack_top(Stack* thiz, void** data);

// Function to push an element onto the stack
int stack_push(Stack* thiz, void* data);

// Function to pop the top element from the stack
int stack_pop(Stack* thiz);

// Function to get the current length (number of elements) in the stack
size_t stack_length(Stack* thiz);

// Function to apply a visit function to each element in the stack
int stack_foreach(Stack* thiz, DataVisitFunc visit, void* ctx);

// Function to destroy the stack
void stack_destroy(Stack* thiz);

#endif
