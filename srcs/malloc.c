#include "malloc.h"

void *malloc(size_t size) {
    int i = getpagesize();
    write(1, "My custom malloc\n", 17);
    (void)size;
    return NULL + i;
}