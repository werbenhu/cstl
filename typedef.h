#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <stdlib.h>

#define OK 0
#define ERR_NIL (-1)
#define ERR_OOM (-2)

#define return_val_if_fail(p, ret) do {\
    if (!(p)) {\
        printf("%s:%d Warning: "#p" failed.\n", __func__, __LINE__);\
        return (ret);\
    }\
} while (0)

typedef void (*DataDestroyFunc)(void* ctx, void* data);
typedef int  (*DataCompareFunc)(void* ctx, void* data);
typedef int  (*DataVisitFunc)(void* ctx, size_t index, void* data);
typedef int (*DataSwapFunc)(void* ctx, size_t i, size_t j);

#define STL_MEM_LEAK_CHECK
#ifdef STL_MEM_LEAK_CHECK
extern int malloc_n;

#define STL_FREE(ptr) stl_free((void**)&(ptr))
#define STL_MALLOC(size) stl_calloc((size))

#else

#define STL_FREE(ptr) stl_free((void**)&(ptr))
#define STL_MALLOC(size) calloc(1, (size))

#endif /* STL_DEBUG */

static inline void stl_free(void** ptr) {
    if (*ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
#ifdef STL_MEM_LEAK_CHECK
        malloc_n--;
#endif
    }
}

static inline void* stl_calloc(size_t size) {
    void* ptr = calloc(1, size);
    if (ptr != NULL) {
#ifdef STL_MEM_LEAK_CHECK
        malloc_n++;
#endif
    }
    return ptr;
}

#endif