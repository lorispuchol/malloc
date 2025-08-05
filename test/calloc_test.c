#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"

int main(void) {
    printf("=== Testing calloc implementation ===\n");
    
    // Test 1: Basic calloc functionality
    printf("\n--- Basic calloc tests ---\n");
    int *arr = calloc(10, sizeof(int));
    if (!arr) {
        printf("❌ calloc(10, sizeof(int)) failed\n");
        return 1;
    }
    
    // Verify memory is zeroed
    for (int i = 0; i < 10; i++) {
        if (arr[i] != 0) {
            printf("❌ Memory not zeroed at index %d: %d\n", i, arr[i]);
            free(arr);
            return 1;
        }
    }
    printf("✅ calloc(10, sizeof(int)) - memory properly zeroed\n");
    
    // Test writing to the memory
    for (int i = 0; i < 10; i++) {
        arr[i] = i + 1;
    }
    for (int i = 0; i < 10; i++) {
        if (arr[i] != i + 1) {
            printf("❌ Memory write/read failed at index %d\n", i);
            free(arr);
            return 1;
        }
    }
    printf("✅ Memory read/write works correctly\n");
    free(arr);
    
    // Test 2: Zero size allocations
    printf("\n--- Edge case tests ---\n");
    void *ptr1 = calloc(0, 10);
    void *ptr2 = calloc(10, 0);
    void *ptr3 = calloc(0, 0);
    
    if (ptr1 != NULL || ptr2 != NULL || ptr3 != NULL) {
        printf("⚠️  Zero size calloc returned non-NULL (implementation defined behavior)\n");
    } else {
        printf("✅ Zero size calloc correctly returns NULL\n");
    }
    
    // Test 3: Large allocation
    printf("\n--- Large allocation test ---\n");
    char *large_arr = calloc(1000, 1000);  // 1MB
    if (!large_arr) {
        printf("❌ Large calloc failed\n");
        return 1;
    }
    
    // Check first and last bytes are zero
    if (large_arr[0] != 0 || large_arr[999999] != 0) {
        printf("❌ Large allocation not properly zeroed\n");
        free(large_arr);
        return 1;
    }
    printf("✅ Large allocation (1MB) properly zeroed\n");
    free(large_arr);
    
    // Test 4: Different zones (tiny, small, large)
    printf("\n--- Zone-specific tests ---\n");
    
    // Tiny zone
    char *tiny = calloc(64, 1);  // 64 bytes
    if (!tiny || tiny[63] != 0) {
        printf("❌ Tiny zone calloc failed\n");
        return 1;
    }
    printf("✅ Tiny zone calloc works\n");
    free(tiny);
    
    // Small zone
    char *small = calloc(512, 1);  // 512 bytes
    if (!small || small[511] != 0) {
        printf("❌ Small zone calloc failed\n");
        return 1;
    }
    printf("✅ Small zone calloc works\n");
    free(small);
    
    // Large zone
    char *large = calloc(2048, 1);  // 2048 bytes
    if (!large || large[2047] != 0) {
        printf("❌ Large zone calloc failed\n");
        return 1;
    }
    printf("✅ Large zone calloc works\n");
    free(large);
    
    // Test 5: Overflow protection
    printf("\n--- Overflow protection test ---\n");
    void *overflow = calloc(SIZE_MAX, 2);
    if (overflow == NULL) {
        printf("✅ Overflow protection works correctly\n");
    } else {
        printf("❌ Overflow protection failed\n");
        free(overflow);
        return 1;
    }
    
    printf("\n=== All calloc tests passed! ===\n");
    return 0;
}
