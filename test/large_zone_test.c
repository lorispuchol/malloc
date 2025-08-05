#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"

int main(void) {
    printf("=== Testing Large Zone Implementation ===\n");
    printf("Each large allocation (>1024 bytes) should get its own mmap call\n\n");
    
    // Allocate multiple large blocks
    void *large1 = malloc(2000);   // Large block 1
    void *large2 = malloc(3000);   // Large block 2  
    void *large3 = malloc(5000);   // Large block 3
    
    printf("Large allocation 1 (2000 bytes): %p\n", large1);
    printf("Large allocation 2 (3000 bytes): %p\n", large2);
    printf("Large allocation 3 (5000 bytes): %p\n", large3);
    
    // Check that they are not adjacent (indicating separate mmap calls)
    char *ptr1 = (char*)large1;
    char *ptr2 = (char*)large2;
    char *ptr3 = (char*)large3;
    
    printf("\nAddress analysis:\n");
    printf("Distance between large1 and large2: %ld bytes\n", (long)(ptr2 - ptr1));
    printf("Distance between large2 and large3: %ld bytes\n", (long)(ptr3 - ptr2));
    
    // Large distances indicate separate mmap calls
    if (abs((long)(ptr2 - ptr1)) > 10000 && abs((long)(ptr3 - ptr2)) > 10000) {
        printf("✅ Large allocations appear to use separate mmap calls\n");
    } else {
        printf("⚠️  Large allocations might be adjacent (check implementation)\n");
    }
    
    printf("\nMemory map visualization:\n");
    show_alloc_mem();
    
    // Free the allocations
    free(large1);
    free(large2);
    free(large3);
    
    printf("\nAfter freeing large allocations:\n");
    show_alloc_mem();
    
    // Test with calloc large allocations
    printf("\n--- Testing large calloc allocations ---\n");
    void *large_calloc1 = calloc(1000, 2);  // 2000 bytes
    void *large_calloc2 = calloc(1500, 2);  // 3000 bytes
    
    printf("Large calloc 1 (2000 bytes): %p\n", large_calloc1);
    printf("Large calloc 2 (3000 bytes): %p\n", large_calloc2);
    
    // Verify memory is zeroed
    char *check_ptr = (char*)large_calloc1;
    int is_zeroed = 1;
    for (int i = 0; i < 2000; i++) {
        if (check_ptr[i] != 0) {
            is_zeroed = 0;
            break;
        }
    }
    
    if (is_zeroed) {
        printf("✅ Large calloc memory is properly zeroed\n");
    } else {
        printf("❌ Large calloc memory is not zeroed\n");
    }
    
    free(large_calloc1);
    free(large_calloc2);
    
    printf("\n=== Large zone test completed ===\n");
    return 0;
}
