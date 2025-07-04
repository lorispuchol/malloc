#include <stdlib.h>
# include "../includes/malloc.h"

int main(void) {
    void *size = malloc(42);
    void *size_2 = malloc(2000);
    (void)size;
        _mem();
    free(size);
    show_alloc_mem();
    free(NULL);
}