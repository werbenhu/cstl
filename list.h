#include <stdio.h>
#include "typedef.h"

#ifndef LIST_H
#define LIST_H

struct ListNode{
    struct ListNode *prev;
    struct ListNode *next;
    void *data;
};

typedef struct {
    struct ListNode *first;
    void *data_destroy_ctx;
    size_t size;
    DataDestroyFunc data_destroy;
} List;

List *list_create(DataDestroyFunc data_destroy, void *ctx);

int list_insert(List *self, size_t index, void *data);
int list_prepend(List *self, void *data);
int list_append(List *self, void *data);
int list_delete(List *self, size_t index);
int list_get_by_index(List *self, size_t index, void **data);
int list_set_by_index(List *self, size_t index, void *data);
size_t list_length(List *self);
int list_find(List *self, DataCompareFunc cmp, void *ctx);
int list_find_data(List *self, DataCompareFunc cmp, void *ctx, void** data);
int list_foreach(List *self, DataVisitFunc visit, void *ctx);

void list_destroy(List *self);

#endif /*LIST_H*/