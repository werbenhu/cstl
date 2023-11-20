#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "typedef.h"
#include "list.h"

typedef int (*MapHashFunc)(void* key);
typedef void (*MapKvDestroyFunc)(void* ctx, void* key, void* value);
typedef int (*MapKvVisitFunc)(void* ctx, void* key, void* value);


typedef struct {
    MapHashFunc    hash;
	List**         slots;
	size_t          slot_n;
    MapKvDestroyFunc data_destroy;
	void*           data_destroy_ctx;

    void* prototype;
    size_t load_factor; 
    size_t threshold;
} Map;

Map* map_create(MapKvDestroyFunc data_destroy, void* ctx, MapHashFunc key_hash);

size_t map_length(Map* self);
int map_set(Map* self, void* key, void* value);
int map_delete(Map* self, DataCompareFunc cmp, void* key);
int map_get(Map* self, DataCompareFunc cmp, void* key, void** value);
int map_foreach(Map* self, MapKvVisitFunc visit, void* ctx);

void map_destroy(Map* self);

#endif /*MAP_H*/