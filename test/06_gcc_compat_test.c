#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../includes/malloc.h"

void test_gcc_compatibility() {
    printf("=== GCC COMPATIBILITY TEST ===\n");
    
    // Test 1: Standard library compatibility
    printf("1. Testing standard library compatibility:\n");
    char *str = (char *)malloc(100);
    strcpy(str, "Hello, World!");
    printf("   String: %s\n", str);
    assert(strcmp(str, "Hello, World!") == 0);
    free(str);
    printf("   ✓ Standard string functions work\n");
    
    // Test 2: Array allocation
    printf("\n2. Testing array allocation:\n");
    int *arr = (int *)malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        arr[i] = i * i;
    }
    printf("   Array values: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
        assert(arr[i] == i * i);
    }
    printf("\n   ✓ Array operations work\n");
    free(arr);
    
    // Test 3: Struct allocation
    printf("\n3. Testing struct allocation:\n");
    typedef struct {
        int id;
        char name[50];
        double value;
    } test_struct_t;
    
    test_struct_t *s = (test_struct_t *)malloc(sizeof(test_struct_t));
    s->id = 42;
    strcpy(s->name, "Test Structure");
    s->value = 3.14159;
    
    printf("   Struct: id=%d, name=%s, value=%.5f\n", s->id, s->name, s->value);
    assert(s->id == 42);
    assert(strcmp(s->name, "Test Structure") == 0);
    assert(s->value > 3.14 && s->value < 3.15);
    free(s);
    printf("   ✓ Struct operations work\n");
    
    // Test 4: Multiple allocations with mixed operations
    printf("\n4. Testing mixed operations:\n");
    void *ptrs[10];
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc((i + 1) * 50);
        assert(ptrs[i] != NULL);
    }
    printf("   ✓ Multiple allocations successful\n");
    
    // Free every other pointer
    for (int i = 0; i < 10; i += 2) {
        free(ptrs[i]);
    }
    printf("   ✓ Selective freeing successful\n");
    
    // Allocate again in freed spaces
    for (int i = 0; i < 10; i += 2) {
        ptrs[i] = malloc((i + 1) * 25);
        assert(ptrs[i] != NULL);
    }
    printf("   ✓ Reallocation in freed spaces successful\n");
    
    // Clean up
    for (int i = 0; i < 10; i++) {
        free(ptrs[i]);
    }
    printf("   ✓ Final cleanup successful\n");
    
    // Test 5: Edge cases that might cause issues with some malloc implementations
    printf("\n5. Testing edge cases:\n");
    
    // Very small allocation
    void *tiny = malloc(1);
    assert(tiny != NULL);
    *((char *)tiny) = 'A';
    assert(*((char *)tiny) == 'A');
    free(tiny);
    printf("   ✓ 1-byte allocation works\n");
    
    // Alignment test
    void *aligned = malloc(17); // Non-power-of-2 size
    assert(aligned != NULL);
    assert(((uintptr_t)aligned % sizeof(void *)) == 0); // Should be pointer-aligned
    free(aligned);
    printf("   ✓ Alignment works for odd sizes\n");
    
    // Zero allocation
    void *zero = malloc(0);
    // malloc(0) behavior is implementation-defined, but should not crash
    if (zero) {
        free(zero);
    }
    printf("   ✓ malloc(0) handled gracefully\n");
    
    printf("\n=== GCC COMPATIBILITY TEST PASSED ===\n\n");
}

int main() {
    test_gcc_compatibility();
    return 0;
}
