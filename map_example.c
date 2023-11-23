#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "map.h"

// Comparison function for keys
int cmp(void *i, void *j) {
    return strcmp((char *) i, (char *) j);
}

// Key-Value pair destruction function
void kv_destroy(void* ctx, void* key, void* value) {
    STL_FREE(key);
    STL_FREE(value);
}

// Key-Value pair visit function
int kv_visit(void* ctx, void* key, void* value) {
    printf("ctx:%s, key:%s, value:%s\n", (char*)ctx, (char*)key, (char*)value);
    return OK;
}

// Hash function for keys
int hash(void* key) {
    int k = *(int*)key;
    return k % 10000;
}

int main() {
    // Create a map with key-value destruction function and hash function
    Map *map = map_create(kv_destroy, NULL, hash);

    // Seed the random number generator
    unsigned int seed = (unsigned int)(time(NULL) + clock());
    srand(seed);

    int elements_size = 18;

    // Populate the map with key-value pairs
    for (int i = 0; i < elements_size; i++) {
        // Create a key
        char *key = (char*) STL_MALLOC(16);
        snprintf(key, 16, "key%d", i);

        // Create a value
        char *value = (char*) STL_MALLOC(16);
        snprintf(value, 16, "value%d", i);

        // Set the key-value pair in the map
        map_set(map, key, value);
    }

    // Example of getting a value by key
    char *k = "key11";
    void* val = NULL;
    if (map_get(map, cmp, k, &val) != OK) {
        printf("can't find key:%s in map\n", k);
    }

    // Example of deleting keys
    for (int i = 0; i < 6; i++) {
        char key[16] = {0};
        snprintf(key, 16, "key%d", i);
        map_delete(map, cmp, key);
    }

    // Example of traversing the map and printing key-value pairs
    char *ctx = "visit-ctx";
    map_foreach(map, kv_visit, ctx);

    printf("val:%s\n", (char*)val);

    // Destroy the map, freeing allocated memory
    map_destroy(map);
    return 0;
}
