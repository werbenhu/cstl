#include "list.h"
#include "map.h"

typedef struct {
	void* key;
    void* value;
} MapKv;

typedef struct {
    DataCompareFunc cmp;
    void* key;
} CmpCtx;

typedef struct {
    MapKvVisitFunc visit;
    void* ctx;
} VisitCtx;

#define MIN_SLOT_SIZE 16

Map *map_create(MapKvDestroyFunc data_destroy, void *ctx, MapHashFunc key_hash) {
    Map *self = NULL;
    return_val_if_fail(key_hash != NULL, NULL);
    self = (Map *)STL_MALLOC(sizeof(Map));

    if (self != NULL) {
        self->hash = key_hash;

        self->slot_n = MIN_SLOT_SIZE;
        self->data_destroy_ctx = ctx;
        self->data_destroy = data_destroy;

        self->prototype = NULL;
        self->load_factor = 0;
        self->threshold = (size_t)((double)self->slot_n * 0.75);

        if ((self->slots = (List **)STL_MALLOC(sizeof(List *) * self->slot_n)) == NULL) {
            STL_FREE(self);
            self = NULL;
        }
    }
    return self;
}

void map_kv_destroy(void* ctx, void* data) {
    MapKv *kv = (MapKv*)data;
    Map* self = (Map*) ctx;
    self->data_destroy(self->data_destroy_ctx, kv->key, kv->value);
    STL_FREE(kv);
}

int map_kv_visit(void* ctx, size_t i, void* data) {
    MapKv *kv = (MapKv*)data;
    VisitCtx *visitCtx = (VisitCtx*)ctx;

    if (visitCtx->visit != NULL) {
        return visitCtx->visit(visitCtx->ctx, kv->key, kv->value);
    }
    return OK;
}

int map_kv_cmp(void* ctx, void* data) {
    CmpCtx* cmp_ctx = (CmpCtx*)ctx;
    MapKv *kv = (MapKv*)data;
    return cmp_ctx->cmp(cmp_ctx->key, kv->key);
}

int map_expand_visit(void* ctx, size_t i, void* data) {
    Map* expanded = (Map*)ctx;
    MapKv *kv = (MapKv*)data;

    size_t index = expanded->hash(kv->key) % expanded->slot_n;
    if (expanded->slots[index] == NULL) {
        expanded->slots[index] = list_create(map_kv_destroy, expanded->prototype);
    } else {
        expanded->load_factor++;
    }
    return list_prepend(expanded->slots[index], kv);
}

void map_expand_destroy(Map *self) {
    if (self != NULL) {
        for (size_t i = 0; i < self->slot_n; i++) {
            if (self->slots[i] != NULL) {
                self->slots[i]->data_destroy = NULL;
                list_destroy(self->slots[i]);
                self->slots[i] = NULL;
            }
        }
        STL_FREE(self->slots);
    }
}

int map_expand_init(Map *self, Map *newly) {
    newly->prototype = self;
    newly->hash = self->hash;
    newly->slot_n = self->slot_n * 2;;
    newly->data_destroy_ctx = self->data_destroy_ctx;
    newly->data_destroy = self->data_destroy;
    newly->load_factor = 0;
    newly->threshold = (size_t)((double)newly->slot_n * 0.75);
    if ((newly->slots = (List **)STL_MALLOC(sizeof(List *) * newly->slot_n)) == NULL) {
        return ERR_OOM;
    }
    return OK;
}

int map_expand(Map *self) {
    Map newly;
    int ret;
    if (ret = map_expand_init(self, &newly) != OK) {
        return ret;
    }
    
    for (size_t i = 0; i < self->slot_n; i++) {
        List *list = self->slots[i];
        if (list != NULL) {
            list_foreach(self->slots[i], map_expand_visit, &newly);
        }
    }

    map_expand_destroy(self);
    self->slots = newly.slots;
    self->slot_n = newly.slot_n;
    self->load_factor = newly.load_factor;
    self->threshold = newly.threshold;
    return OK;
}

int map_set(Map *self, void* key, void *value) {
    return_val_if_fail(self != NULL, -1);

    size_t index = self->hash(key) % self->slot_n;
    MapKv* kv = (MapKv *)STL_MALLOC(sizeof(MapKv));
    kv->key = key;
    kv->value = value;

    if (self->slots[index] == NULL) {
        self->slots[index] = list_create(map_kv_destroy, self);
    } else {
        self->load_factor++;
    }

    if (self->load_factor >= self->threshold) {
        int ret;
        if (ret = map_expand(self) != OK) {
            return ret;
        };
    }
    return list_prepend(self->slots[index], kv);
}

int map_delete(Map *self, DataCompareFunc cmp, void *key) {
    size_t index = 0;
    List *list = NULL;
    return_val_if_fail(self != NULL && cmp != NULL, -1);

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

size_t map_length(Map *self) {
    size_t i = 0;
    size_t nr = 0;
    return_val_if_fail(self != NULL, 0);

    for (i = 0; i < self->slot_n; i++) {
        if (self->slots[i] != NULL) {
            nr += list_length(self->slots[i]);
        }
    }
    return nr;
}

int map_get(Map *self, DataCompareFunc cmp, void *key, void **value) {
    int index = 0;
    List *list = NULL;
    return_val_if_fail(self != NULL && cmp != NULL && value != NULL, -1);

    index = self->hash(key) % self->slot_n;
    list = self->slots[index];

    if (list != NULL) {
        CmpCtx ctx;
        ctx.key = key;
        ctx.cmp = cmp;
        MapKv *kv = NULL;

        index = list_find_data(list, map_kv_cmp, &ctx, (void**)&kv);
        if (index < 0) {
            return ERR_NIL;
        }
        *value = kv->value;
        return OK;
    }
    return ERR_NIL;
}

int map_foreach(Map *self, MapKvVisitFunc visit, void *ctx) {
    size_t i = 0;
    return_val_if_fail(self != NULL && visit != NULL, -1);

    VisitCtx visitCtx;
    visitCtx.ctx = ctx;
    visitCtx.visit = visit;

    for (i = 0; i < self->slot_n; i++){
        if (self->slots[i] != NULL) {
            list_foreach(self->slots[i], map_kv_visit, &visitCtx);
        }
    }
    return OK;
}

void map_destroy(Map *self) {
    if (self != NULL) {
        for (size_t i = 0; i < self->slot_n; i++) {
            if (self->slots[i] != NULL) {
                list_destroy(self->slots[i]);
                self->slots[i] = NULL;
            }
        }
        STL_FREE(self->slots);
        STL_FREE(self);
    }
}