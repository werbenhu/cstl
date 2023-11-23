#include "queue.h"
#include "typedef.h"
#include "list.h"

// Function to create a new queue
Queue* queue_create(DataDestroyFunc data_destroy, void* ctx) {
    Queue* self = (Queue*)STL_MALLOC(sizeof(Queue));
    if (self != NULL) {
        if ((self->list = list_create(data_destroy, ctx)) == NULL) {
            STL_FREE(self);
            self = NULL;
        }
    }
    return self;
}

// Function to get the element at the head of the queue without removing it
int queue_head(Queue* self, void** data) {
    return_val_if_fail(self != NULL && data != NULL, ERR_NIL);
    return list_get_by_index(self->list, 0, data);
}

// Function to push (enqueue) an element to the end of the queue
int queue_push(Queue* self, void* data) {
    return_val_if_fail(self != NULL, ERR_NIL);
    return list_append(self->list, data);
}

// Function to pop (dequeue) an element from the front of the queue
int queue_pop(Queue* self) {
    return_val_if_fail(self != NULL, ERR_NIL);
    return list_delete(self->list, 0);
}

// Function to get the current length (number of elements) in the queue
size_t queue_length(Queue* self) {
    return_val_if_fail(self != NULL, 0);
    return list_length(self->list);
}

// Function to apply a visit function to each element in the queue
int queue_foreach(Queue* self, DataVisitFunc visit, void* ctx) {
    return_val_if_fail(self != NULL && visit != NULL, ERR_NIL);
    return list_foreach(self->list, visit, ctx);
}

// Function to destroy the queue
void queue_destroy(Queue* self) {
    if (self != NULL) {
        list_destroy(self->list);
        self->list = NULL;
        STL_FREE(self);
    }
    return;
}
