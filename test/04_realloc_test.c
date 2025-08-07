#include <stdio.h>
#include <string.h>
#include "../includes/malloc.h"

int main() {
    printf("=== REALLOC FUNCTION TEST ===\n");
    
    // Test 1: Basic realloc (grow)
    printf("1. Testing realloc growth...\n");
    char *ptr = (char*)malloc(50);
    strcpy(ptr, "Hello World");
    printf("   Original (50 bytes): %p -> \"%s\"\n", ptr, ptr);
    
    ptr = (char*)realloc(ptr, 100);
    if (ptr && strcmp(ptr, "Hello World") == 0) {
        printf("   ✅ Realloc to 100 bytes succeeded: %p\n", ptr);
        printf("   ✅ Data preserved: \"%s\"\n", ptr);
    } else {
        printf("   ❌ Realloc growth failed\n");
    }
    
    // Test 2: Realloc shrink
    printf("2. Testing realloc shrink...\n");
    strcpy(ptr, "Test");
    ptr = (char*)realloc(ptr, 20);
    if (ptr && strcmp(ptr, "Test") == 0) {
        printf("   ✅ Realloc to 20 bytes succeeded: %p\n", ptr);
        printf("   ✅ Data preserved: \"%s\"\n", ptr);
    } else {
        printf("   ❌ Realloc shrink failed\n");
    }
    
    // Test 3: realloc(NULL, size) - should behave like malloc
    printf("3. Testing realloc(NULL, size)...\n");
    char *ptr2 = (char*)realloc(NULL, 64);
    if (ptr2) {
        printf("   ✅ realloc(NULL, 64) works like malloc: %p\n", ptr2);
        strcpy(ptr2, "New allocation");
        printf("   ✅ Can write to new memory: \"%s\"\n", ptr2);
    } else {
        printf("   ❌ realloc(NULL, size) failed\n");
    }
    
    // Test 4: realloc(ptr, 0) - should behave like free
    printf("4. Testing realloc(ptr, 0)...\n");
    char *ptr3 = (char*)malloc(32);
    strcpy(ptr3, "To be freed");
    printf("   Before: %p -> \"%s\"\n", ptr3, ptr3);
    
    void *result = realloc(ptr3, 0);
    if (result == NULL) {
        printf("   ✅ realloc(ptr, 0) returned NULL (like free)\n");
    } else {
        printf("   ❌ realloc(ptr, 0) should return NULL\n");
        free(result);
    }
    
    // Test 5: Cross-zone realloc (TINY -> SMALL -> LARGE)
    printf("5. Testing cross-zone realloc...\n");
    void *cross_ptr = malloc(32);  // TINY
    printf("   TINY (32): %p\n", cross_ptr);
    
    cross_ptr = realloc(cross_ptr, 256);  // SMALL  
    printf("   SMALL (256): %p\n", cross_ptr);
    
    cross_ptr = realloc(cross_ptr, 2048);  // LARGE
    printf("   LARGE (2048): %p\n", cross_ptr);
    
    cross_ptr = realloc(cross_ptr, 64);   // Back to TINY
    printf("   Back to TINY (64): %p\n", cross_ptr);
    
    if (cross_ptr) {
        printf("   ✅ Cross-zone realloc successful\n");
    } else {
        printf("   ❌ Cross-zone realloc failed\n");
    }
    
    // Test 6: Realloc with data preservation across zones
    printf("6. Testing data preservation across zones...\n");
    char *data_ptr = (char*)malloc(50);
    strcpy(data_ptr, "Preserve this data");
    printf("   Original: \"%s\"\n", data_ptr);
    
    data_ptr = (char*)realloc(data_ptr, 1500);  // Move to LARGE zone
    if (data_ptr && strncmp(data_ptr, "Preserve this data", 18) == 0) {
        printf("   ✅ Data preserved across zone change: \"%s\"\n", data_ptr);
    } else {
        printf("   ❌ Data not preserved across zone change\n");
    }
    
    // Clean up
    free(ptr);
    free(ptr2);
    free(cross_ptr);
    free(data_ptr);
    
    printf("=== REALLOC TEST COMPLETED ===\n\n");
    return 0;
}
