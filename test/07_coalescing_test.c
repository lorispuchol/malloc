#include <stdio.h>
#include <string.h>
#include "../includes/malloc.h"

void test_memory_coalescing() {
    printf("=== MEMORY COALESCING TEST ===\n");
    
    printf("1. Initial state:\n");
    show_alloc_mem();
    
    printf("\n2. Allocating 5 adjacent small blocks:\n");
    void *ptr1 = malloc(100);
    void *ptr2 = malloc(100);
    void *ptr3 = malloc(100);
    void *ptr4 = malloc(100);
    void *ptr5 = malloc(100);
    
    printf("   Allocated blocks:\n");
    printf("   ptr1: %p\n", ptr1);
    printf("   ptr2: %p\n", ptr2);
    printf("   ptr3: %p\n", ptr3);
    printf("   ptr4: %p\n", ptr4);
    printf("   ptr5: %p\n", ptr5);
    show_alloc_mem();
    
    printf("\n3. Freeing middle block (ptr3):\n");
    free(ptr3);
    show_alloc_mem();
    
    printf("\n4. Freeing adjacent block (ptr2) - should coalesce:\n");
    free(ptr2);
    show_alloc_mem();
    
    printf("\n5. Freeing another adjacent block (ptr4) - should coalesce:\n");
    free(ptr4);
    show_alloc_mem();
    
    printf("\n6. Allocating larger block in coalesced space:\n");
    void *large_ptr = malloc(350); // Should fit in coalesced space
    printf("   Large block allocated at: %p\n", large_ptr);
    show_alloc_mem();
    
    printf("\n7. Testing coalescing with different sizes:\n");
    void *a = malloc(50);
    void *b = malloc(200);
    void *c = malloc(75);
    printf("   Before freeing:\n");
    show_alloc_mem();
    
    printf("\n8. Freeing in order to test coalescing:\n");
    free(b);
    printf("   After freeing middle block:\n");
    show_alloc_mem();
    
    free(a);
    printf("   After freeing left block (should coalesce):\n");
    show_alloc_mem();
    
    free(c);
    printf("   After freeing right block (should coalesce):\n");
    show_alloc_mem();
    
    printf("\n9. Testing boundary coalescing:\n");
    void *x = malloc(128); // TINY zone
    void *y = malloc(256); // SMALL zone
    void *z = malloc(128); // TINY zone again
    
    printf("   Different zone allocations:\n");
    printf("   x (TINY): %p\n", x);
    printf("   y (SMALL): %p\n", y);
    printf("   z (TINY): %p\n", z);
    show_alloc_mem();
    
    printf("\n10. Cleanup and final coalescing:\n");
    free(ptr1);
    free(ptr5);
    free(large_ptr);
    free(x);
    free(y);
    free(z);
    
    printf("   Final state after all cleanup:\n");
    show_alloc_mem();
    
    printf("\n11. Testing fragmentation and coalescing recovery:\n");
    // Create fragmentation
    void *frag[10];
    for (int i = 0; i < 10; i++) {
        frag[i] = malloc(50);
    }
    printf("   After creating fragmentation:\n");
    show_alloc_mem();
    
    // Free every other block
    for (int i = 1; i < 10; i += 2) {
        free(frag[i]);
    }
    printf("   After creating holes:\n");
    show_alloc_mem();
    
    // Free remaining blocks to test coalescing
    for (int i = 0; i < 10; i += 2) {
        free(frag[i]);
    }
    printf("   After coalescing all fragments:\n");
    show_alloc_mem();
    
    printf("=== MEMORY COALESCING TEST COMPLETED ===\n\n");
}

int main() {
    test_memory_coalescing();
    return 0;
}
