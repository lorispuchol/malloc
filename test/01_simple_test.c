#include <stdio.h>
#include <string.h>
#include "../includes/malloc.h"
#include "../includes/printf.h"

int main() {
    ft_printf("=== SIMPLE MALLOC TEST ===\n");
    
    // Test 1: Basic malloc
    ft_printf("1. Testing basic malloc...\n");
    void *ptr1 = malloc(64);
    if (ptr1) {
        ft_printf("   ✅ malloc(64) succeeded: %p\n", ptr1);
        // Write some data
        strcpy((char*)ptr1, "Hello World");
        ft_printf("   ✅ Data written and read: %s\n", (char*)ptr1);
    } else {
        ft_printf("   ❌ malloc(64) failed\n");
        return 1;
    }
    
    // Test 2: Different sizes
    ft_printf("2. Testing different sizes...\n");
    void *tiny = malloc(32);    // TINY
    void *small = malloc(512);  // SMALL  
    void *large = malloc(2048); // LARGE
    
    ft_printf("   TINY (32):  %p\n", tiny);
    ft_printf("   SMALL (512): %p\n", small);
    ft_printf("   LARGE (2048): %p\n", large);
    
    if (tiny && small && large) {
        ft_printf("   ✅ All allocations succeeded\n");
    } else {
        ft_printf("   ❌ Some allocations failed\n");
    }
    
    // Test 3: malloc(0)
    ft_printf("3. Testing malloc(0)...\n");
    void *zero_ptr = malloc(0);
    if (zero_ptr == NULL) {
        ft_printf("   ✅ malloc(0) correctly returned NULL\n");
    } else {
        ft_printf("   ❌ malloc(0) should return NULL, got %p\n", zero_ptr);
    }
    
    // Test 4: Free everything
    ft_printf("4. Testing free...\n");
    free(ptr1);
    free(tiny);
    free(small);
    free(large);
    ft_printf("   ✅ All frees completed without crash\n");
    
    ft_printf("=== SIMPLE TEST COMPLETED ===\n\n");
    return 0;
}
