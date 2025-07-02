#include <stdlib.h>

int main(void) {
    void *size = malloc(42);
    (void)size;
    show_alloc_mem();
    free(size);
    free(NULL);
}