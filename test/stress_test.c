#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("Testing multiple malloc/free cycles...\n");
    
    for (int i = 0; i < 1000; i++) {
        void *ptr = malloc(64);
        if (!ptr) {
            printf("malloc failed at iteration %d\n", i);
            return 1;
        }
        memset(ptr, 0, 64);
        free(ptr);
    }
    
    printf("1000 malloc/free cycles completed successfully\n");
    
    // Test multiple allocations at once
    void *ptrs[100];
    for (int i = 0; i < 100; i++) {
        ptrs[i] = malloc(128);
        if (!ptrs[i]) {
            printf("malloc failed at bulk allocation %d\n", i);
            return 1;
        }
    }
    
    for (int i = 0; i < 100; i++) {
        free(ptrs[i]);
    }
    
    printf("Bulk allocation test completed successfully\n");
    return 0;
}
