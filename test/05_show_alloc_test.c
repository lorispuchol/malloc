#include <stdio.h>
#include "../includes/malloc.h"

int main() {
    printf("=== SHOW_ALLOC_MEM TEST ===\n");
    
    printf("1. Empty state:\n");
    show_alloc_mem();
    
    printf("\n2. After allocating one from each zone:\n");
    void *tiny = malloc(64);
    void *small = malloc(512);
    void *large = malloc(2048);
    show_alloc_mem();
    
    printf("\n3. After allocating multiple blocks:\n");
    void *tiny2 = malloc(32);
    void *small2 = malloc(256);
    void *large2 = malloc(4096);
    show_alloc_mem();
    
    printf("\n4. After freeing some blocks:\n");
    free(tiny);
    free(small2);
    show_alloc_mem();
    
    printf("\n5. After freeing all:\n");
    free(tiny2);
    free(small);
    free(small2);  // Double free test
    free(large);
    free(large2);
    show_alloc_mem();
    
    printf("\n6. After new allocations:\n");
    void *new1 = malloc(128);
    void *new2 = malloc(1024);
    void *new3 = malloc(8192);
    show_alloc_mem();
    
    // Cleanup
    free(new1);
    free(new2);
    free(new3);
    
    printf("\n7. Final state:\n");
    show_alloc_mem();
    
    printf("=== SHOW_ALLOC_MEM TEST COMPLETED ===\n\n");
    return 0;
}
