#include <stdio.h>
#include "typedef.h"

#ifndef LIST_H
#define LIST_H

// Structure for a node in the linked list
struct ListNode {
    struct ListNode *prev; // Pointer to the previous node
    struct ListNode *next; // Pointer to the next node
    void *data; // Data stored in the node
};

// Structure for the linked list
typedef struct {
    struct ListNode *first; // Pointer to the first node in the list
    void *data_destroy_ctx; // Context for data destruction function
    size_t size; // Number of nodes in the list
    DataDestroyFunc data_destroy; // Function pointer for destroying node data
} List;

// Create a new linked list
List *list_create(DataDestroyFunc data_destroy, void *ctx);

// Insert data at a specified position
int list_insert(List *self, size_t index, void *data);

// Prepend data at the beginning of the list
int list_prepend(List *self, void *data);

// Append data at the end of the list
int list_append(List *self, void *data);

// Delete the node at the specified position
int list_delete(List *self, size_t index);

// Get data from the node at the specified position
int list_get_by_index(List *self, size_t index, void **data);

// Set data for the node at the specified position
int list_set_by_index(List *self, size_t index, void *data);

// Get the length of the list (number of nodes)
size_t list_length(List *self);

// Find the position of a node that satisfies a condition
int list_find(List *self, DataCompareFunc cmp, void *ctx);

// Find data in a node that satisfies a condition
int list_find_data(List *self, DataCompareFunc cmp, void *ctx, void **data);

// Traverse the list and perform a specified operation on each node
int list_foreach(List *self, DataVisitFunc visit, void *ctx);

// Destroy the linked list
void list_destroy(List *self);

#endif /* LIST_H */
