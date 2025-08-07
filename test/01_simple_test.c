#include <stdio.h>
#include <string.h>
#include "../includes/malloc.h"

int main() {
    printf("=== SIMPLE MALLOC TEST ===\n");
    
    // Test 1: Basic malloc
    printf("1. Testing basic malloc...\n");
    void *ptr1 = malloc(64);
    if (ptr1) {
        printf("   ✅ malloc(64) succeeded: %p\n", ptr1);
        // Write some data
        strcpy((char*)ptr1, "Hello World");
        printf("   ✅ Data written and read: %s\n", (char*)ptr1);
    } else {
        printf("   ❌ malloc(64) failed\n");
        return 1;
    }
    
    // Test 2: Different sizes
    printf("2. Testing different sizes...\n");
    void *tiny = malloc(32);    // TINY
    void *small = malloc(512);  // SMALL  
    void *large = malloc(2048); // LARGE
    
    printf("   TINY (32):  %p\n", tiny);
    printf("   SMALL (512): %p\n", small);
    printf("   LARGE (2048): %p\n", large);
    
    if (tiny && small && large) {
        printf("   ✅ All allocations succeeded\n");
    } else {
        printf("   ❌ Some allocations failed\n");
    }
    
    // Test 3: malloc(0)
    printf("3. Testing malloc(0)...\n");
    void *zero_ptr = malloc(0);
    if (zero_ptr == NULL) {
        printf("   ✅ malloc(0) correctly returned NULL\n");
    } else {
        printf("   ❌ malloc(0) should return NULL, got %p\n", zero_ptr);
    }
    
    // Test 4: Free everything
    printf("4. Testing free...\n");
    free(ptr1);
    free(tiny);
    free(small);
    free(large);
    printf("   ✅ All frees completed without crash\n");
    
    printf("=== SIMPLE TEST COMPLETED ===\n\n");
    return 0;
}
