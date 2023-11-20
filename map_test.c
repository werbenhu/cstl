#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include <string.h>

int cmp(void *i, void *j) {
    return strcmp((char *) i, (char *) j);
}

void kv_destroy(void* ctx, void* key, void* value) {
    STL_FREE(key);
    STL_FREE(value);
}

int kv_visit(void* ctx, void* key, void* value) {
    printf("ctx:%s, key:%s, value:%s\n", (char*)ctx, (char*)key, (char*)value);
    return OK;
}

int hash(void* key) {
    int k = *(int*)key;
    return k % 10000;
}

int malloc_n = 0;

int main() {
    Map *map = map_create(kv_destroy, NULL, hash);
    unsigned int seed = (unsigned int)(time(NULL) + clock());
    srand(seed);

    int elements_size = 18;
    for (int i = 0; i < elements_size; i++) {
        char *key = (char*) STL_MALLOC(16);
        snprintf(key, 16, "key%d", i);

        char *value = (char*) STL_MALLOC(16);
        snprintf(value, 16, "value%d", i);
        map_set(map, key, value);
    }

    char *k = "key11";
    void* val = NULL;
    if (map_get(map, cmp, k, &val) != OK) {
        printf("can't find key:%s in map\n", k);
    }

    
    for (int i = 0; i < 6; i++) {
        char key[16] = {0};
        snprintf(key, 16, "key%d", i);
        map_delete(map, cmp, key);
    }

    char *ctx = "visit-ctx";
    map_foreach(map, kv_visit, ctx);

    printf("val:%s\n", (char*)val);
    map_destroy(map);

    printf("malloc_n:%d\n", malloc_n);
    return 0;
}
