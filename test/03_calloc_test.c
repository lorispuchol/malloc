#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../includes/malloc.h"

int main() {
    printf("=== CALLOC FUNCTION TEST ===\n");
    
    // Test 1: Basic calloc
    printf("1. Testing basic calloc...\n");
    int *arr = (int*)calloc(10, sizeof(int));
    if (arr) {
        printf("   ✅ calloc(10, sizeof(int)) succeeded: %p\n", arr);
        
        // Check if memory is zeroed
        int all_zero = 1;
        for (int i = 0; i < 10; i++) {
            if (arr[i] != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) {
            printf("   ✅ Memory properly zeroed\n");
        } else {
            printf("   ❌ Memory not properly zeroed\n");
        }
        
        // Test writing and reading
        arr[0] = 42;
        arr[9] = 99;
        if (arr[0] == 42 && arr[9] == 99) {
            printf("   ✅ Memory read/write works\n");
        } else {
            printf("   ❌ Memory read/write failed\n");
        }
        
        free(arr);
    } else {
        printf("   ❌ calloc failed\n");
    }
    
    // Test 2: calloc(0, size) and calloc(size, 0)
    printf("2. Testing calloc with zero...\n");
    void *ptr1 = calloc(0, 10);
    void *ptr2 = calloc(10, 0);
    if (ptr1 == NULL && ptr2 == NULL) {
        printf("   ✅ calloc with zero correctly returns NULL\n");
    } else {
        printf("   ❌ calloc with zero should return NULL\n");
    }
    
    // Test 3: Large calloc
    printf("3. Testing large calloc...\n");
    char *big_arr = (char*)calloc(2000, 1);
    if (big_arr) {
        printf("   ✅ Large calloc succeeded: %p\n", big_arr);
        
        // Check first and last bytes are zero
        if (big_arr[0] == 0 && big_arr[1999] == 0) {
            printf("   ✅ Large memory properly zeroed\n");
        } else {
            printf("   ❌ Large memory not properly zeroed\n");
        }
        free(big_arr);
    } else {
        printf("   ❌ Large calloc failed\n");
    }
    
    // Test 4: Overflow protection
    printf("4. Testing overflow protection...\n");
    void *overflow = calloc(SIZE_MAX/2, SIZE_MAX/2);
    if (overflow == NULL) {
        printf("   ✅ Overflow protection works\n");
    } else {
        printf("   ❌ Overflow protection failed\n");
        free(overflow);
    }
    
    // Test 5: Different zones through calloc
    printf("5. Testing calloc across zones...\n");
    void *tiny_c = calloc(8, 4);    // 32 bytes - TINY
    void *small_c = calloc(100, 5); // 500 bytes - SMALL  
    void *large_c = calloc(300, 4); // 1200 bytes - LARGE
    
    printf("   TINY calloc: %p\n", tiny_c);
    printf("   SMALL calloc: %p\n", small_c);
    printf("   LARGE calloc: %p\n", large_c);
    
    if (tiny_c && small_c && large_c) {
        printf("   ✅ calloc works across all zones\n");
    } else {
        printf("   ❌ Some calloc allocations failed\n");
    }
    
    free(tiny_c);
    free(small_c);  
    free(large_c);
    
    printf("=== CALLOC TEST COMPLETED ===\n\n");
    return 0;
}
