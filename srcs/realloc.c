#include "malloc.h"

void *realloc(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    write(1, "My custom realloc\n", 18);
    return NULL;
}