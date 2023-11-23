#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include "typedef.h"
#include "list.h"

// Structure representing a Queue
typedef struct {
    List* list; // The underlying list to store queue elements
} Queue;

// Function to create a new queue
Queue* queue_create(DataDestroyFunc data_destroy, void* ctx);

// Function to get the element at the head of the queue without removing it
int queue_head(Queue* thiz, void** data);

// Function to push (enqueue) an element to the end of the queue
int queue_push(Queue* thiz, void* data);

// Function to pop (dequeue) an element from the front of the queue
int queue_pop(Queue* thiz);

// Function to get the current length (number of elements) in the queue
size_t queue_length(Queue* thiz);

// Function to apply a visit function to each element in the queue
int queue_foreach(Queue* thiz, DataVisitFunc visit, void* ctx);

void queue_destroy(Queue* self);

#endif