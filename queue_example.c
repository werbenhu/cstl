#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

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