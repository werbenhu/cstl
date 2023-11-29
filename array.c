#include <stdlib.h>
#include "array.h"
#include "sort.h"

// Static function: Destroy data
static void array_destroy_data(Array *self, void *data) {
    if (self->data_destroy != NULL) {
        self->data_destroy(self->data_destroy_ctx, data);
    }
}

#define MIN_SIZE 16

// Create a new dynamic array
Array *array_create(DataDestroyFunc data_destroy, void *ctx) {
    Array *self = STL_MALLOC(sizeof(Array));
    if (self != NULL) {
        self->data = STL_MALLOC(MIN_SIZE * sizeof(void *));
        self->size = 0;
        self->alloc_size = MIN_SIZE;
        self->data_destroy = data_destroy;
        self->data_destroy_ctx = ctx;
    }
    return self;
}

// Static function: Expand the memory capacity of the array
static int array_expand(Array *self, size_t need) {
    return_val_if_fail(self != NULL, ERR_NIL);

    size_t alloc_size = self->alloc_size;
    while ((self->size + need) > alloc_size) {
        if (alloc_size == 0) {
            alloc_size = MIN_SIZE;
        } else if (alloc_size < 1024) {
            alloc_size = alloc_size << 1;
        } else {
            alloc_size = alloc_size + (alloc_size >> 3);
        }
    }

    void **data = (void **) realloc(self->data, sizeof(void *) * alloc_size);
    if (data != NULL) {
        self->data = data;
        self->alloc_size = alloc_size;
        return OK;
    }
    return ERR_OOM;
}

// Static function: Shrink the memory capacity of the array
static int array_shrink(Array *self) {
    return_val_if_fail(self != NULL, ERR_NIL);

    if ((self->size < (self->alloc_size >> 1)) && (self->alloc_size > MIN_SIZE)) {
        size_t alloc_size = self->size + (self->size >> 1);

        void **data = (void **) realloc(self->data, sizeof(void *) * alloc_size);
        if (data != NULL) {
            self->data = data;
            self->alloc_size = alloc_size;
        }
    }
    return OK;
}

// Insert data at the specified index
int array_insert(Array *self, size_t index, void *data) {
    size_t cursor = index;
    return_val_if_fail(self != NULL, ERR_NIL);
    cursor = cursor < self->size ? cursor : self->size;

    if (array_expand(self, 1) == 0) {
        size_t i = 0;
        for (i = self->size; i > cursor; i--) {
            self->data[i] = self->data[i - 1];
        }
        self->data[cursor] = data;
        self->size++;
        return OK;
    }
    return ERR_OOM;
}

// Insert data at the beginning of the array
int array_prepend(Array *self, void *data) {
    return array_insert(self, 0, data);
}

// Append data to the end of the array
int array_append(Array *self, void *data) {
    return array_insert(self, -1, data);
}

// Delete data at the specified index
int array_delete(Array *self, size_t index) {
    size_t i = 0;
    return_val_if_fail(self != NULL && self->size > index, ERR_NIL);

    array_destroy_data(self, self->data[index]);
    for (i = index; (i + 1) < self->size; i++) {
        self->data[i] = self->data[i + 1];
    }

    self->size--;
    array_shrink(self);
    return OK;
}

// Get data by index
int array_get_by_index(Array *self, size_t index, void **data) {
    return_val_if_fail(self != NULL && data != NULL && index < self->size, ERR_NIL);
    *data = self->data[index];
    return OK;
}

// Set data at the specified index
int array_set_by_index(Array *self, size_t index, void *data) {
    return_val_if_fail(self != NULL && index < self->size, ERR_NIL);
    self->data[index] = data;
    return OK;
}

// Get the length of the array
size_t array_length(Array *self) {
    return_val_if_fail(self != NULL, 0);
    return self->size;
}

// Iterate over the array and apply a data visiting function
int array_foreach(Array *self, DataVisitFunc visit, void *ctx) {
    size_t i = 0;
    return_val_if_fail(self != NULL && visit != NULL, ERR_NIL);

    for (i = 0; i < self->size; i++) {
        if (!visit(ctx, i, self->data[i])) {
            break;
        };
    }
    return OK;
}

// Find the index of data that satisfies a condition
int array_find(Array *self, DataCompareFunc cmp, void *ctx) {
    int i = -1;
    return_val_if_fail(self != NULL && cmp != NULL, ERR_NIL);

    for (i = 0; i < self->size; i++) {
        if (cmp(ctx, self->data[i]) == 0) {
            break;
        }
    }
    return i;
}

// Sort the data in the array
int array_sort(Array *self, DataCompareFunc cmp, DataSwapFunc swap) {
    return_val_if_fail(self != NULL && swap != NULL && cmp != NULL, ERR_NIL);
    quick_sort(self,
               0,
               self->size - 1,
               (SortGetFunc)array_get_by_index,
               (SortSetFunc)array_set_by_index,
               (SortCmpFunc)cmp,
               swap);
    return OK;
}

// Destroy the dynamic array and release resources
void array_destroy(Array *self) {
    size_t i = 0;

    if (self != NULL) {
        for (i = 0; i < self->size; i++) {
            array_destroy_data(self, self->data[i]);
        }
        STL_FREE(self->data);
        STL_FREE(self);
    }
}
