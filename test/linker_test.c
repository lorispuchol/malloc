#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Test memory allocations similar to what a linker might do
    printf("Testing linker-like allocation patterns...\n");
    
    // Allocate many small blocks (like symbol table entries)
    void *ptrs[1000];
    for (int i = 0; i < 1000; i++) {
        ptrs[i] = malloc(32 + (i % 64)); // Variable sizes
        if (!ptrs[i]) {
            printf("Failed at allocation %d\n", i);
            return 1;
        }
    }
    
    // Free every other one (fragmentation test)
    for (int i = 0; i < 1000; i += 2) {
        free(ptrs[i]);
    }
    
    // Allocate some more
    for (int i = 0; i < 1000; i += 2) {
        ptrs[i] = malloc(16 + (i % 32));
        if (!ptrs[i]) {
            printf("Failed at reallocation %d\n", i);
            return 1;
        }
    }
    
    // Free all
    for (int i = 0; i < 1000; i++) {
        free(ptrs[i]);
    }
    
    printf("Linker-like test completed successfully\n");
    return 0;
}
