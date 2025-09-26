#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include <stdlib.h>
#include <stdio.h>

// Core tracking functions
void* mt_malloc(size_t size, const char* file, int line);
void* mt_calloc(size_t nmemb, size_t size, const char* file, int line);
void* mt_realloc(void* ptr, size_t size, const char* file, int line);
void mt_free(void* ptr, const char* file, int line);
void mt_report(void);  // prints all unfreed allocations

// Macros to replace standard allocation/free
#define malloc(s) mt_malloc(s, __FILE__, __LINE__)
#define calloc(n,s) mt_calloc(n,s,__FILE__, __LINE__)
#define realloc(p,s) mt_realloc(p,s,__FILE__, __LINE__)
#define free(p) mt_free(p, __FILE__, __LINE__)

#endif // MEMORY_TRACKER_H
