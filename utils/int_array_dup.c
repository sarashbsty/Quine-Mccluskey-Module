#include "memory_tracker.h"
#include "int_array_dup.h"
#include<stdlib.h>
#include<string.h>

int *intDupArr(const int *src, size_t n){
    if (src == NULL || n == 0)
        return NULL;

    int *copy = malloc(n * sizeof *copy);
    if (!copy)
        return NULL;

    memcpy(copy, src, n * sizeof *copy);
    return copy;
}
