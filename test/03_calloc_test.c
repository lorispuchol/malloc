#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../includes/malloc.h"
#include "../includes/printf.h"

int main() {
    ft_printf("=== CALLOC FUNCTION TEST ===\n");
    
    // Test 1: Basic calloc
    ft_printf("1. Testing basic calloc...\n");
    int *arr = (int*)calloc(10, sizeof(int));
    if (arr) {
        ft_printf("   ✅ calloc(10, sizeof(int)) succeeded: %p\n", arr);
        
        // Check if memory is zeroed
        int all_zero = 1;
        for (int i = 0; i < 10; i++) {
            if (arr[i] != 0) {
                all_zero = 0;
                break;
            }
        }
        if (all_zero) {
            ft_printf("   ✅ Memory properly zeroed\n");
        } else {
            ft_printf("   ❌ Memory not properly zeroed\n");
        }
        
        // Test writing and reading
        arr[0] = 42;
        arr[9] = 99;
        if (arr[0] == 42 && arr[9] == 99) {
            ft_printf("   ✅ Memory read/write works\n");
        } else {
            ft_printf("   ❌ Memory read/write failed\n");
        }
        
        free(arr);
    } else {
        ft_printf("   ❌ calloc failed\n");
    }
    
    // Test 2: calloc(0, size) and calloc(size, 0)
    ft_printf("2. Testing calloc with zero...\n");
    void *ptr1 = calloc(0, 10);
    void *ptr2 = calloc(10, 0);
    if (ptr1 == NULL && ptr2 == NULL) {
        ft_printf("   ✅ calloc with zero correctly returns NULL\n");
    } else {
        ft_printf("   ❌ calloc with zero should return NULL\n");
    }
    
    // Test 3: Large calloc
    ft_printf("3. Testing large calloc...\n");
    char *big_arr = (char*)calloc(2000, 1);
    if (big_arr) {
        ft_printf("   ✅ Large calloc succeeded: %p\n", big_arr);
        
        // Check first and last bytes are zero
        if (big_arr[0] == 0 && big_arr[1999] == 0) {
            ft_printf("   ✅ Large memory properly zeroed\n");
        } else {
            ft_printf("   ❌ Large memory not properly zeroed\n");
        }
        free(big_arr);
    } else {
        ft_printf("   ❌ Large calloc failed\n");
    }
    
    // Test 4: Overflow protection
    ft_printf("4. Testing overflow protection...\n");
    // Test with values that would definitely overflow when multiplied
    void *overflow = calloc(1000000000UL, 1000000000UL);
    if (overflow == NULL) {
        ft_printf("   ✅ Overflow protection works\n");
    } else {
        ft_printf("   ❌ Overflow protection failed\n");
        free(overflow);
    }
    
    // Test 5: Different zones through calloc
    ft_printf("5. Testing calloc across zones...\n");
    void *tiny_c = calloc(8, 4);    // 32 bytes - TINY
    void *small_c = calloc(100, 5); // 500 bytes - SMALL  
    void *large_c = calloc(300, 4); // 1200 bytes - LARGE
    
    ft_printf("   TINY calloc: %p\n", tiny_c);
    ft_printf("   SMALL calloc: %p\n", small_c);
    ft_printf("   LARGE calloc: %p\n", large_c);
    
    if (tiny_c && small_c && large_c) {
        ft_printf("   ✅ calloc works across all zones\n");
    } else {
        ft_printf("   ❌ Some calloc allocations failed\n");
    }
    
    free(tiny_c);
    free(small_c);  
    free(large_c);
    
    ft_printf("=== CALLOC TEST COMPLETED ===\n\n");
    return 0;
}
