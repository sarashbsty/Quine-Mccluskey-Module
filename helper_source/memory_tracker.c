#include "memory_tracker.h"

#undef malloc
#undef free
#undef calloc
#undef realloc

#define REAL_MALLOC malloc
#define REAL_CALLOC calloc
#define REAL_REALLOC realloc
#define REAL_FREE free

typedef struct MemRecord {
    void* ptr;
    size_t size;
    const char* file;
    int line;
    struct MemRecord* next;
} MemRecord;

static MemRecord* head = NULL;

// Forward declaration
static void report_leaks_at_exit(void);

// Initialize automatic reporting once
__attribute__((constructor)) // GCC/MinGW specific
static void init_memory_tracker(void) {
    atexit(report_leaks_at_exit);
}

// malloc wrapper
void* mt_malloc(size_t size, const char* file, int line) {
    void* ptr = REAL_MALLOC(size);
    if (ptr) {
        MemRecord* rec = REAL_MALLOC(sizeof(MemRecord));
        if (rec) {
            rec->ptr = ptr;
            rec->size = size;
            rec->file = file;
            rec->line = line;
            rec->next = head;
            head = rec;
        }
    }
    return ptr;
}

// calloc wrapper
void* mt_calloc(size_t nmemb, size_t size, const char* file, int line) {
    void* ptr = REAL_CALLOC(nmemb, size);
    if (ptr) {
        MemRecord* rec = REAL_MALLOC(sizeof(MemRecord));
        if (rec) {
            rec->ptr = ptr;
            rec->size = nmemb * size;
            rec->file = file;
            rec->line = line;
            rec->next = head;
            head = rec;
        }
    }
    return ptr;
}

// realloc wrapper
void* mt_realloc(void* ptr, size_t size, const char* file, int line) {
    if (!ptr) return mt_malloc(size, file, line);

    MemRecord **cur = &head;
    while (*cur) {
        if ((*cur)->ptr == ptr) {
            MemRecord* tmp = *cur;
            *cur = tmp->next;
            REAL_FREE(tmp);
            break;
        }
        cur = &((*cur)->next);
    }

    void* new_ptr = REAL_REALLOC(ptr, size);
    if (new_ptr) {
        MemRecord* rec = REAL_MALLOC(sizeof(MemRecord));
        if (rec) {
            rec->ptr = new_ptr;
            rec->size = size;
            rec->file = file;
            rec->line = line;
            rec->next = head;
            head = rec;
        }
    }
    return new_ptr;
}

// free wrapper
void mt_free(void* ptr, const char* file, int line) {
    if (!ptr) return;

    MemRecord **cur = &head;
    while (*cur) {
        if ((*cur)->ptr == ptr) {
            MemRecord* tmp = *cur;
            *cur = tmp->next;
            REAL_FREE(tmp);
            break;
        }
        cur = &((*cur)->next);
    }
    REAL_FREE(ptr);
}

// manual report (can still call if desired)
void mt_report(void) {
    MemRecord* cur = head;
    if (!cur) {
        printf("No memory leaks detected.\n");
        return;
    }

    printf("Memory leaks detected:\n");
    while (cur) {
        printf("Leaked %zu bytes at %p allocated from %s:%d\n",
               cur->size, cur->ptr, cur->file, cur->line);
        cur = cur->next;
    }
}

// Automatic report at program exit
static void report_leaks_at_exit(void) {
    mt_report();
}
