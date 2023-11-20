#include <stdlib.h>
#include "list.h"

// Destroy data using the provided data destruction function
static void list_destroy_data(List *self, void *data) {
    if (self->data_destroy != NULL) {
        self->data_destroy(self->data_destroy_ctx, data);
    }
}

// Create a new list node with the given data
static struct ListNode *list_create_node(List *self, void *data) {
    struct ListNode *node = STL_MALLOC(sizeof(struct ListNode));
    if (node != NULL) {
        node->prev = NULL;
        node->next = NULL;
        node->data = data;
    }
    return node;
}

// Destroy a list node, including data destruction
static void list_destroy_node(List *self, struct ListNode *node) {
    if (node != NULL) {
        node->next = NULL;
        node->prev = NULL;
        list_destroy_data(self, node->data);
        STL_FREE(node);
    }
}

// Create a new list
List *list_create(DataDestroyFunc data_destroy, void *ctx) {
    List *self = STL_MALLOC(sizeof(List));
    if (self != NULL) {
        self->first = NULL;
        self->data_destroy = data_destroy;
        self->data_destroy_ctx = ctx;
        self->size = 0;
    }
    return self;
}

// Get a node at the specified index, with an option to return the last node if index exceeds the list size
static struct ListNode *list_get_node(List *self, size_t index, int fail_return_last) {
    struct ListNode *iter = NULL;
    return_val_if_fail(self != NULL, NULL);
    iter = self->first;

    while (iter != NULL && iter->next != NULL && index > 0) {
        iter = iter->next;
        index--;
    }

    if (!fail_return_last) {
        iter = index > 0 ? NULL : iter;
    }
    return iter;
}

// Insert data at the specified index
int list_insert(List *self, size_t index, void *data) {
    int ret = OK;
    struct ListNode *node = NULL;
    struct ListNode *cursor = NULL;
    return_val_if_fail(self != NULL, ERR_NIL);

    node = list_create_node(self, data);
    return_val_if_fail(node != NULL, ERR_OOM);

    if (self->first == NULL) {
        self->first = node;
        self->size = 1;
        return OK;
    }

    cursor = list_get_node(self, index, 1);
    if (index < list_length(self)) {
        node->next = cursor;
        if (cursor->prev != NULL) {
            cursor->prev->next = node;
        }

        cursor->prev = node;
        if (self->first == cursor) {
            self->first = node;
        }
    } else {
        cursor->next = node;
        node->prev = cursor;
    }
    self->size++;
    return ret;
}

// Prepend data at the beginning of the list
int list_prepend(List *self, void *data) {
    return list_insert(self, 0, data);
}

// Append data at the end of the list
int list_append(List *self, void *data) {
    return list_insert(self, -1, data);
}

// Delete the node at the specified index
int list_delete(List *self, size_t index) {
    struct ListNode *cursor = NULL;

    return_val_if_fail(self != NULL, ERR_NIL);
    cursor = list_get_node(self, index, 0);
    return_val_if_fail(cursor != NULL, ERR_NIL);

    if (cursor == self->first) {
        self->first = cursor->next;
    }

    if (cursor->next != NULL) {
        cursor->next->prev = cursor->prev;
    }

    if (cursor->prev != NULL) {
        cursor->prev->next = cursor->next;
    }
    list_destroy_node(self, cursor);
    self->size--;
    return OK;
}

// Get data from the node at the specified index
int list_get_by_index(List *self, size_t index, void **data) {
    struct ListNode *cursor = NULL;
    return_val_if_fail(self != NULL && data != NULL, ERR_NIL);
    cursor = list_get_node(self, index, 0);

    if (cursor != NULL) {
        *data = cursor->data;
    }
    return cursor != NULL ? 0 : 1;
}

// Set data for the node at the specified index
int list_set_by_index(List *self, size_t index, void *data) {
    struct ListNode *cursor = NULL;
    return_val_if_fail(self != NULL, ERR_NIL);
    cursor = list_get_node(self, index, 0);

    if (cursor != NULL) {
        cursor->data = data;
    }
    return cursor != NULL ? OK : ERR_NIL;
}

// Get the length of the list (number of nodes)
size_t list_length(List *self) {
    return self->size;
}

// Iterate over the list and perform a specified operation on each node
int list_foreach(List *self, DataVisitFunc visit, void *ctx) {
    int ret = 0;
    struct ListNode *iter = NULL;
    return_val_if_fail(self != NULL && visit != NULL, ERR_NIL);
    iter = self->first;
    size_t index = 0;

    while (iter != NULL && ret != 4) {
        ret = visit(ctx, index++, iter->data);
        iter = iter->next;
    }
    return ret;
}

// Find the position of a node that satisfies a condition
int list_find(List *self, DataCompareFunc cmp, void *ctx) {
    int ret = -1;
    int i = 0;
    struct ListNode *iter = NULL;
    return_val_if_fail(self != NULL && cmp != NULL, -1);

    iter = self->first;
    while (iter != NULL) {
        if (cmp(ctx, iter->data) == 0) {
            ret = i;
            break;
        }
        i++;
        iter = iter->next;
    }
    return ret;
}

// Find data in a node that satisfies a condition
int list_find_data(List *self, DataCompareFunc cmp, void *ctx, void** data) {
    int ret = -1;
    int i = 0;
    struct ListNode *iter = NULL;
    return_val_if_fail(self != NULL && cmp != NULL, -1);

    iter = self->first;
    while (iter != NULL) {
        if (cmp(ctx, iter->data) == 0) {
            if (data != NULL) {
                *data = iter->data;
            }
            ret = i;
            break;
        }
        i++;
        iter = iter->next;
    }
    return ret;
}

// Destroy the linked list
void list_destroy(List *self) {
    struct ListNode *iter = NULL;
    struct ListNode *next = NULL;
    if (self != NULL) {
        iter = self->first;
        while (iter != NULL) {
            next = iter->next;
            list_destroy_node(self, iter);
            iter = next;
        }

        self->first = NULL;
        self->size = 0;
        STL_FREE(self);
    }
}
