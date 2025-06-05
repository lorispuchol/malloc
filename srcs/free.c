#include "malloc.h"

void free(void *ptr) {
    (void)ptr;
    write(1, "My custom free\n", 15);
}