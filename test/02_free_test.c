#include <stdio.h>
#include <string.h>
#include "../includes/malloc.h"

int main() {
    printf("=== FREE FUNCTION TEST ===\n");
    
    // Test 1: Basic free
    printf("1. Testing basic free...\n");
    void *ptr = malloc(128);
    printf("   Allocated: %p\n", ptr);
    free(ptr);
    printf("   ✅ Free completed without crash\n");
    
    // Test 2: free(NULL)
    printf("2. Testing free(NULL)...\n");
    free(NULL);
    printf("   ✅ free(NULL) handled safely\n");
    
    // Test 3: Double free detection
    printf("3. Testing double free...\n");
    void *ptr2 = malloc(64);
    printf("   Allocated: %p\n", ptr2);
    free(ptr2);
    printf("   First free completed\n");
    free(ptr2);  // This should be handled safely
    printf("   ✅ Double free handled without crash\n");
    
    // Test 4: Free different zone types
    printf("4. Testing free across zones...\n");
    void *tiny = malloc(32);
    void *small = malloc(512);  
    void *large = malloc(3000);
    printf("   Allocated TINY: %p, SMALL: %p, LARGE: %p\n", tiny, small, large);
    
    free(large);  // Free large first
    free(tiny);   // Free tiny second  
    free(small);  // Free small last
    printf("   ✅ All zone types freed successfully\n");
    
    // Test 5: Free invalid pointer (should not crash)
    printf("5. Testing free with invalid pointer...\n");
    void *invalid = (void*)0x12345678;
    free(invalid);
    printf("   ✅ Invalid pointer handled safely\n");
    
    printf("=== FREE TEST COMPLETED ===\n\n");
    return 0;
}
