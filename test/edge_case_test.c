#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

int main() {
    printf("Testing malloc edge cases that might affect flex...\n");
    
    // Test 1: Very small allocations
    for (int i = 1; i <= 32; i++) {
        void *ptr = malloc(i);
        if (!ptr) {
            printf("malloc(%d) failed!\n", i);
            return 1;
        }
        // Check if we can write to the memory
        memset(ptr, 0xAA, i);
        // Check alignment
        if ((uintptr_t)ptr % 8 != 0) {
            printf("malloc(%d) returned unaligned pointer: %p\n", i, ptr);
        }
        free(ptr);
    }
    
    // Test 2: Typical flex buffer sizes
    size_t flex_sizes[] = {16384, 32768, 65536, 131072};
    for (int i = 0; i < 4; i++) {
        void *ptr = malloc(flex_sizes[i]);
        if (!ptr) {
            printf("malloc(%zu) failed!\n", flex_sizes[i]);
            return 1;
        }
        // Test write access
        memset(ptr, 0x55, flex_sizes[i]);
        // Test read access
        for (size_t j = 0; j < flex_sizes[i]; j++) {
            assert(((char*)ptr)[j] == 0x55);
        }
        free(ptr);
        printf("malloc(%zu) test passed\n", flex_sizes[i]);
    }
    
    // Test 3: Allocation patterns similar to flex buffer doubling
    void *ptr = malloc(1024);
    for (int i = 0; i < 10; i++) {
        size_t new_size = 1024 * (1 << i);
        ptr = realloc(ptr, new_size);
        if (!ptr) {
            printf("realloc to %zu failed!\n", new_size);
            return 1;
        }
        memset(ptr, 0x77, new_size);
        printf("realloc to %zu passed\n", new_size);
    }
    free(ptr);
    
    printf("All malloc edge case tests passed!\n");
    return 0;
}
