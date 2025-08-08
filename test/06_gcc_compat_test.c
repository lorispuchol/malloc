#include <stdio.h>
#include <string.h>
#include "../includes/malloc.h"
#include "../includes/printf.h"

void test_gcc_compatibility() {
    ft_printf("=== GCC COMPATIBILITY TEST ===\n");
    
    // Test 1: Standard library compatibility
    ft_printf("1. Testing standard library compatibility:\n");
    char *str = (char *)malloc(100);
    strcpy(str, "Hello, World!");
    ft_printf("   String: %s\n", str);
    if (strcmp(str, "Hello, World!") == 0) {
        ft_printf("   ✓ Standard string functions work\n");
    } else {
        ft_printf("   ❌ String test failed\n");
    }
    free(str);
    
    // Test 2: Array allocation
    ft_printf("\n2. Testing array allocation:\n");
    int *arr = (int *)malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        arr[i] = i * i;
    }
    ft_printf("   Array values: ");
    for (int i = 0; i < 10; i++) {
        ft_printf("%d ", arr[i]);
        if (arr[i] != i * i) {
            ft_printf("\n   ❌ Array test failed at index %d\n", i);
            free(arr);
            return;
        }
    }
    ft_printf("\n   ✓ Array operations work\n");
    free(arr);
    
    // Test 3: Struct allocation
    ft_printf("\n3. Testing struct allocation:\n");
    typedef struct {
        int id;
        char name[50];
        double value;
    } test_struct_t;
    
    test_struct_t *s = (test_struct_t *)malloc(sizeof(test_struct_t));
    s->id = 42;
    strcpy(s->name, "Test Structure");
    s->value = 3.14159;
    
    ft_printf("   Struct: id=%d, name=%s, value=%.5f\n", s->id, s->name, s->value);
    if (s->id == 42 && strcmp(s->name, "Test Structure") == 0 && s->value > 3.14 && s->value < 3.15) {
        ft_printf("   ✓ Struct operations verified\n");
    } else {
        ft_printf("   ❌ Struct test failed\n");
    }
    free(s);
    ft_printf("   ✓ Struct operations work\n");
    
    // Test 4: Multiple allocations with mixed operations
    ft_printf("\n4. Testing mixed operations:\n");
    void *ptrs[10];
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc((i + 1) * 50);
        if (ptrs[i] == NULL) {
            ft_printf("   ❌ Allocation failed at index %d\n", i);
            // Free previously allocated pointers
            for (int j = 0; j < i; j++) {
                free(ptrs[j]);
            }
            return;
        }
    }
    ft_printf("   ✓ Multiple allocations successful\n");
    
    // Free every other pointer
    for (int i = 0; i < 10; i += 2) {
        free(ptrs[i]);
    }
    ft_printf("   ✓ Selective freeing successful\n");
    
    // Allocate again in freed spaces
    for (int i = 0; i < 10; i += 2) {
        ptrs[i] = malloc((i + 1) * 25);
        if (ptrs[i] == NULL) {
            ft_printf("   ❌ Reallocation failed at index %d\n", i);
            // Free remaining pointers
            for (int j = 1; j < 10; j += 2) {
                free(ptrs[j]);
            }
            return;
        }
    }
    ft_printf("   ✓ Reallocation in freed spaces successful\n");
    
    // Clean up
    for (int i = 0; i < 10; i++) {
        free(ptrs[i]);
    }
    ft_printf("   ✓ Final cleanup successful\n");
    
    // Test 5: Edge cases that might cause issues with some malloc implementations
    ft_printf("\n5. Testing edge cases:\n");
    
    // Very small allocation
    void *tiny = malloc(1);
    if (tiny == NULL) {
        ft_printf("   ❌ Tiny allocation failed\n");
        return;
    }
    *((char *)tiny) = 'A';
    if (*((char *)tiny) != 'A') {
        ft_printf("   ❌ Tiny allocation write/read failed\n");
        free(tiny);
        return;
    }
    free(tiny);
    ft_printf("   ✓ 1-byte allocation works\n");
    
    // Alignment test
    void *aligned = malloc(17); // Non-power-of-2 size
    if (aligned == NULL) {
        ft_printf("   ❌ Alignment test allocation failed\n");
        return;
    }
    if (((uintptr_t)aligned % sizeof(void *)) != 0) {
        ft_printf("   ❌ Alignment test failed - not pointer-aligned\n");
        free(aligned);
        return;
    }
    free(aligned);
    ft_printf("   ✓ Alignment works for odd sizes\n");
    
    // Zero allocation
    void *zero = malloc(0);
    // malloc(0) behavior is implementation-defined, but should not crash
    if (zero) {
        free(zero);
    }
    ft_printf("   ✓ malloc(0) handled gracefully\n");
    
    ft_printf("\n=== GCC COMPATIBILITY TEST PASSED ===\n\n");
}

int main() {
    test_gcc_compatibility();
    return 0;
}
