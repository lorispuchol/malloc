#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>

int main() {
    void *ptr = malloc(SIZE_MAX - 20000);  
    printf("Allocated address: %p\n", ptr);
    free(ptr);
    // free(size);
    SIZE_MAX;
}