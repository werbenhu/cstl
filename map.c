#include "list.h"
#include "map.h"

// Structure to represent key-value pairs in the map
typedef struct {
    void* key;
    void* value;
} MapKv;

// Structure to represent context for key comparison
typedef struct {
    DataCompareFunc cmp;
    void* key;
} CmpCtx;

// Structure to represent context for key-value visiting
typedef struct {
    MapKvVisitFunc visit;
    void* ctx;
} VisitCtx;

#define MIN_SLOT_SIZE 16

// Create a new map
Map *map_create(MapKvDestroyFunc data_destroy, void *ctx, MapHashFunc key_hash) {
    Map *self = NULL;
    return_val_if_fail(key_hash != NULL, NULL);
    self = (Map *)STL_MALLOC(sizeof(Map));

    if (self != NULL) {
        // Initialize map attributes
        self->hash = key_hash;
        self->slot_n = MIN_SLOT_SIZE;
        self->data_destroy_ctx = ctx;
        self->data_destroy = data_destroy;
        self->prototype = NULL;
        self->load_factor = 0;
        self->threshold = (size_t)((double)self->slot_n * 0.75);

        // Allocate memory for slots
        if ((self->slots = (List **)STL_MALLOC(sizeof(List *) * self->slot_n)) == NULL) {
            STL_FREE(self);
            self = NULL;
        }
    }
    return self;
}

// Function to destroy key-value pairs
void map_kv_destroy(void* ctx, void* data) {
    MapKv *kv = (MapKv*)data;
    Map* self = (Map*) ctx;
    // Destroy key-value pair
    self->data_destroy(self->data_destroy_ctx, kv->key, kv->value);
    STL_FREE(kv);
}

// Function to visit key-value pairs
int map_kv_visit(void* ctx, size_t i, void* data) {
    MapKv *kv = (MapKv*)data;
    VisitCtx *visitCtx = (VisitCtx*)ctx;

    // Invoke the visit function on the key-value pair
    if (visitCtx->visit != NULL) {
        return visitCtx->visit(visitCtx->ctx, kv->key, kv->value);
    }
    return OK;
}

// Function to compare keys
int map_kv_cmp(void* ctx, void* data) {
    CmpCtx* cmp_ctx = (CmpCtx*)ctx;
    MapKv *kv = (MapKv*)data;
    // Invoke the key comparison function
    return cmp_ctx->cmp(cmp_ctx->key, kv->key);
}

// Function to visit key-value pairs during map expansion
int map_expand_visit(void* ctx, size_t i, void* data) {
    Map* expanded = (Map*)ctx;
    MapKv *kv = (MapKv*)data;

    // Calculate the index in the expanded map
    size_t index = expanded->hash(kv->key) % expanded->slot_n;

    // Create a new list if the slot is empty
    if (expanded->slots[index] == NULL) {
        expanded->slots[index] = list_create(map_kv_destroy, expanded->prototype);
    } else {
        expanded->load_factor++;
    }

    // Prepend the key-value pair to the list
    return list_prepend(expanded->slots[index], kv);
}

// Function to destroy key-value pairs during map expansion
void map_expand_destroy(Map *self) {
    if (self != NULL) {
        for (size_t i = 0; i < self->slot_n; i++) {
            if (self->slots[i] != NULL) {
                // Disable data destruction during list destruction
                self->slots[i]->data_destroy = NULL;
                // Destroy the list
                list_destroy(self->slots[i]);
                self->slots[i] = NULL;
            }
        }
        // Free the array of slots
        STL_FREE(self->slots);
    }
}

// Function to initialize the expanded map during map expansion
int map_expand_init(Map *self, Map *newly) {
    newly->prototype = self;
    newly->hash = self->hash;
    newly->slot_n = self->slot_n * 2;
    newly->data_destroy_ctx = self->data_destroy_ctx;
    newly->data_destroy = self->data_destroy;
    newly->load_factor = 0;
    newly->threshold = (size_t)((double)newly->slot_n * 0.75);

    // Allocate memory for slots in the expanded map
    if ((newly->slots = (List **)STL_MALLOC(sizeof(List *) * newly->slot_n)) == NULL) {
        return ERR_OOM;
    }
    return OK;
}

// Function to expand the map
int map_expand(Map *self) {
    Map newly;
    int ret;
    // Initialize the expanded map
    if (ret = map_expand_init(self, &newly) != OK) {
        return ret;
    }

    // Iterate over existing key-value pairs and visit them during map expansion
    for (size_t i = 0; i < self->slot_n; i++) {
        List *list = self->slots[i];
        if (list != NULL) {
            list_foreach(self->slots[i], map_expand_visit, &newly);
        }
    }

    // Destroy the existing map
    map_expand_destroy(self);
    // Update the map with the expanded map's attributes
    self->slots = newly.slots;
    self->slot_n = newly.slot_n;
    self->load_factor = newly.load_factor;
    self->threshold = newly.threshold;
    return OK;
}

// Function to set a key-value pair in the map
int map_set(Map *self, void* key, void *value) {
    return_val_if_fail(self != NULL, -1);

    // Calculate the index for the key in the map
    size_t index = self->hash(key) % self->slot_n;
    MapKv* kv = (MapKv *)STL_MALLOC(sizeof(MapKv));
    kv->key = key;
    kv->value = value;

    // Create a new list if the slot is empty
    if (self->slots[index] == NULL) {
        self->slots[index] = list_create(map_kv_destroy, self);
    } else {
        self->load_factor++;
    }

    // Check if map expansion is needed
    if (self->load_factor >= self->threshold) {
        int ret;
        // Expand the map
        if (ret = map_expand(self) != OK) {
            return ret;
        };
    }
    // Prepend the key-value pair to the list
    return list_prepend(self->slots[index], kv);
}

// Function to delete a key-value pair from the map
int map_delete(Map *self, DataCompareFunc cmp, void *key) {
    size_t index = 0;
    List *list = NULL;
    return_val_if_fail(self != NULL && cmp != NULL, -1);

    // Calculate the index for the key in the map
    index = self->hash(key) % self->slot_n;
    list = self->slots[index];

    if (list != NULL){
        CmpCtx ctx;
        ctx.key = key;
        ctx.cmp = cmp;
        index = list_find(list, map_kv_cmp, &ctx);
        return list_delete(list, index);
    }
    return ERR_NIL;
}

// Function to get the number of key-value pairs in the map
size_t map_length(Map *self) {
    size_t i = 0;
    size_t nr = 0;
    return_val_if_fail(self != NULL, 0);

    // Iterate over all slots and count the number of key-value pairs
    for (i = 0; i < self->slot_n; i++) {
        if (self->slots[i] != NULL) {
            nr += list_length(self->slots[i]);
        }
    }
    return nr;
}

// Function to get the value associated with a key in the map
int map_get(Map *self, DataCompareFunc cmp, void *key, void **value) {
    int index = 0;
    List *list = NULL;
    return_val_if_fail(self != NULL && cmp != NULL && value != NULL, -1);

    // Calculate the index for the key in the map
    index = self->hash(key) % self->slot_n;
    list = self->slots[index];

    if (list != NULL) {
        CmpCtx ctx;
        ctx.key = key;
        ctx.cmp = cmp;
        MapKv *kv = NULL;

        // Find the key-value pair in the list
        index = list_find_data(list, map_kv_cmp, &ctx, (void**)&kv);
        if (index < 0) {
            return ERR_NIL;
        }
        *value = kv->value;
        return OK;
    }
    return ERR_NIL;
}

// Function to iterate over key-value pairs in the map
int map_foreach(Map *self, MapKvVisitFunc visit, void *ctx) {
    size_t i = 0;
    return_val_if_fail(self != NULL && visit != NULL, -1);

    // Context for visiting key-value pairs
    VisitCtx visitCtx;
    visitCtx.ctx = ctx;
    visitCtx.visit = visit;

    // Iterate over all slots and visit key-value pairs in each list
    for (i = 0; i < self->slot_n; i++){
        if (self->slots[i] != NULL) {
            list_foreach(self->slots[i], map_kv_visit, &visitCtx);
        }
    }
    return OK;
}

// Function to destroy the map
void map_destroy(Map *self) {
    if (self != NULL) {
        for (size_t i = 0; i < self->slot_n; i++) {
            if (self->slots[i] != NULL) {
                // Destroy each list in the slot
                list_destroy(self->slots[i]);
                self->slots[i] = NULL;
            }
        }
        // Free the array of slots
        STL_FREE(self->slots);
        // Free the map structure
        STL_FREE(self);
    }
}
