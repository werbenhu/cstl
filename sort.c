#include <stdio.h>
#include <stdlib.h>
#include "sort.h"
#include "typedef.h"

static size_t partition(void *arr, size_t low, size_t high, SortGetFunc get, SortSetFunc set, SortCmpFunc cmp, SortSwapFunc swap) {
    void *pivot = NULL;
    get(arr, high, &pivot);
    size_t i = low;

    for (size_t j = low; j < high; j++) {
        void *current = NULL;
        get(arr, j, &current);
        if (cmp(current, pivot) < 0) {
            if (i != j) {
                swap(arr, i, j);
            }
            i++;
        }
    }

    if (i != high) {
        swap(arr, i, high);
    }
    return i;
}

void quick_sort(void *arr, size_t low, size_t high, SortGetFunc get, SortSetFunc set, SortCmpFunc cmp, SortSwapFunc swap) {
    size_t* stack = (size_t*)STL_MALLOC((high - low + 1) * sizeof(size_t));
    int top = -1;

    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0) {
        high = stack[top--];
        low = stack[top--];

        size_t mid = partition(arr, low, high, get, set, cmp, swap);
        if (mid > low) {
            stack[++top] = low;
            stack[++top] = mid - 1;
        }

        if (mid < high) {
            stack[++top] = mid + 1;
            stack[++top] = high;
        }
    }
    STL_FREE(stack);
}

int binary_search(void *arr, size_t low, size_t high, void *data, SortGetFunc get, SortCmpFunc cmp) {
    while (low <= high) {
        size_t mid = low + (high - low) / 2;
        void *current = NULL;
        get(arr, mid, &current);

        int comparison = cmp(current, data);
        if (comparison == 0) {
            return (int)mid;
        } else if (comparison < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (int)low;
}
