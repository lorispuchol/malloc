#include <stdlib.h>
#include "../includes/malloc.h"

int main(void) {
    void *size = malloc(42);
    void *size_2 = malloc(2000);
    (void)size;
    (void)size_2;
    show_alloc_mem();
    free(size);
    show_alloc_mem();
    free(size_2);
    free(NULL);
    return 0;
}