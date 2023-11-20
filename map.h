#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "typedef.h"
#include "list.h"

// Function pointer type for hashing keys in the map
typedef int (*MapHashFunc)(void* key);

// Function pointer type for destroying key-value pairs in the map
typedef void (*MapKvDestroyFunc)(void* ctx, void* key, void* value);

// Function pointer type for visiting key-value pairs in the map
typedef int (*MapKvVisitFunc)(void* ctx, void* key, void* value);

// Structure to represent a map
typedef struct {
    MapHashFunc    hash;                // Hash function for keys
    List**         slots;               // Array of linked lists (slots) to store key-value pairs
    size_t          slot_n;             // Number of slots in the map
    MapKvDestroyFunc data_destroy;      // Function to destroy key-value pairs
    void*           data_destroy_ctx;   // Context for data destruction
    void* prototype;    // Prototype object for map initialization
    size_t load_factor;  // Current load factor of the map
    size_t threshold;    // Threshold for resizing the map
} Map;

// Create a new map
Map* map_create(MapKvDestroyFunc data_destroy, void* ctx, MapHashFunc key_hash);

// Get the number of key-value pairs in the map
size_t map_length(Map* self);

// Set a key-value pair in the map
int map_set(Map* self, void* key, void* value);

// Delete a key-value pair from the map
int map_delete(Map* self, DataCompareFunc cmp, void* key);

// Get the value associated with a key in the map
int map_get(Map* self, DataCompareFunc cmp, void* key, void** value);

// Iterate over key-value pairs in the map
int map_foreach(Map* self, MapKvVisitFunc visit, void* ctx);

// Destroy the map
void map_destroy(Map* self);

#endif /*MAP_H*/
