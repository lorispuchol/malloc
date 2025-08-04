#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"

int main(void) {
    printf("=== Testing malloc implementation ===\n");
    
    // Test tiny allocations (1-128 bytes)
    printf("\n--- TINY allocations ---\n");
    void *tiny1 = malloc(42);
    void *tiny2 = malloc(100);
    void *tiny3 = malloc(1);
    printf("tiny1 (42 bytes): %p\n", tiny1);
    printf("tiny2 (100 bytes): %p\n", tiny2);
    printf("tiny3 (1 byte): %p\n", tiny3);
    
    // Test small allocations (129-1024 bytes)
    printf("\n--- SMALL allocations ---\n");
    void *small1 = malloc(300);
    void *small2 = malloc(800);
    void *small3 = malloc(1000);
    printf("small1 (300 bytes): %p\n", small1);
    printf("small2 (800 bytes): %p\n", small2);
    printf("small3 (1000 bytes): %p\n", small3);
    
    // Test large allocations (>1024 bytes)
    printf("\n--- LARGE allocations ---\n");
    void *large1 = malloc(2000);
    void *large2 = malloc(5000);
    void *large3 = malloc(10000);
    printf("large1 (2000 bytes): %p\n", large1);
    printf("large2 (5000 bytes): %p\n", large2);
    printf("large3 (10000 bytes): %p\n", large3);
    
    printf("\n--- Memory state after all allocations ---\n");
    show_alloc_mem();
    
    // Test realloc
    printf("\n--- Testing realloc ---\n");
    tiny1 = realloc(tiny1, 200);  // tiny -> small
    printf("Reallocated tiny1 from 42 to 200 bytes: %p\n", tiny1);
    
    small1 = realloc(small1, 2500);  // small -> large
    printf("Reallocated small1 from 300 to 2500 bytes: %p\n", small1);
    
    large1 = realloc(large1, 50);  // large -> tiny
    printf("Reallocated large1 from 2000 to 50 bytes: %p\n", large1);
    
    printf("\n--- Memory state after reallocs ---\n");
    show_alloc_mem();
    
    // Test edge cases
    printf("\n--- Testing edge cases ---\n");
    void *zero = malloc(0);
    printf("malloc(0): %p\n", zero);
    
    void *null_realloc = realloc(NULL, 100);
    printf("realloc(NULL, 100): %p\n", null_realloc);
    
    void *size_zero_realloc = realloc(null_realloc, 0);
    printf("realloc(ptr, 0): %p\n", size_zero_realloc);
    
    // Free everything
    printf("\n--- Freeing memory ---\n");
    free(tiny1);
    free(tiny2);
    free(tiny3);
    free(small1);
    free(small2);
    free(small3);
    free(large2);
    free(large3);
    free(zero);
    
    printf("--- Memory state after freeing ---\n");
    show_alloc_mem();
    
    // Test double free (should not crash)
    printf("\n--- Testing double free (should not crash) ---\n");
    free(tiny1);  // Double free
    free(NULL);   // Free NULL
    
    printf("Double free and NULL free handled successfully\n");
    
    return 0;
}
