#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "../includes/malloc.h"
#include "../includes/printf.h"

#define MAX_ALLOCATIONS 1000
#define MAX_SIZE 4096
#define NUM_ITERATIONS 5

typedef struct allocation {
    void *ptr;
    size_t size;
    int active;
} allocation_t;

void test_stress_malloc() {
    ft_printf("=== COMPLEX STRESS TEST ===\n");
    
    allocation_t allocations[MAX_ALLOCATIONS];
    srand((unsigned int)time(NULL));
    
    // Initialize allocation tracking
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        allocations[i].ptr = NULL;
        allocations[i].size = 0;
        allocations[i].active = 0;
    }
    
    ft_printf("1. Random allocation/deallocation stress test:\n");
    show_alloc_mem();
    
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        ft_printf("\n   Iteration %d/%d:\n", iter + 1, NUM_ITERATIONS);
        
        // Allocation phase
        ft_printf("   - Allocating phase...\n");
        for (int i = 0; i < MAX_ALLOCATIONS / 2; i++) {
            if (!allocations[i].active) {
                size_t size = (rand() % MAX_SIZE) + 1;
                allocations[i].ptr = malloc(size);
                if (allocations[i].ptr) {
                    allocations[i].size = size;
                    allocations[i].active = 1;
                    
                    // Write pattern to verify memory integrity
                    memset(allocations[i].ptr, (char)(i % 256), size);
                }
            }
        }
        
        // Mixed allocation/deallocation phase
        ft_printf("   - Mixed operations phase...\n");
        for (int i = 0; i < MAX_ALLOCATIONS; i++) {
            if (rand() % 2 == 0) { // 50% chance to free
                if (allocations[i].active) {
                    // Verify memory integrity before freeing
                    char *data = (char *)allocations[i].ptr;
                    for (size_t j = 0; j < allocations[i].size; j++) {
                        assert(data[j] == (char)(i % 256));
                    }
                    
                    free(allocations[i].ptr);
                    allocations[i].ptr = NULL;
                    allocations[i].active = 0;
                }
            } else { // 50% chance to allocate
                if (!allocations[i].active) {
                    size_t size = (rand() % MAX_SIZE) + 1;
                    allocations[i].ptr = malloc(size);
                    if (allocations[i].ptr) {
                        allocations[i].size = size;
                        allocations[i].active = 1;
                        memset(allocations[i].ptr, (char)(i % 256), size);
                    }
                }
            }
        }
        
        // Count active allocations
        int active_count = 0;
        size_t total_allocated = 0;
        for (int i = 0; i < MAX_ALLOCATIONS; i++) {
            if (allocations[i].active) {
                active_count++;
                total_allocated += allocations[i].size;
            }
        }
        ft_printf("   - Active allocations: %d, Total size: %zu bytes\n", 
               active_count, total_allocated);
    }
    
    ft_printf("\n2. Memory state after stress test:\n");
    show_alloc_mem();
    
    ft_printf("\n3. Testing memory integrity after stress test:\n");
    int integrity_failures = 0;
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (allocations[i].active) {
            char *data = (char *)allocations[i].ptr;
            for (size_t j = 0; j < allocations[i].size; j++) {
                if (data[j] != (char)(i % 256)) {
                    integrity_failures++;
                    break;
                }
            }
        }
    }
    ft_printf("   Memory integrity failures: %d\n", integrity_failures);
    assert(integrity_failures == 0);
    ft_printf("   ✓ All memory integrity checks passed\n");
    
    ft_printf("\n4. Testing realloc stress:\n");
    for (int i = 0; i < 50; i++) {
        if (allocations[i].active) {
            size_t old_size = allocations[i].size;
            size_t new_size = (rand() % MAX_SIZE) + 1;
            
            void *old_ptr = allocations[i].ptr;
            allocations[i].ptr = realloc(allocations[i].ptr, new_size);
            
            if (allocations[i].ptr) {
                // Use old_ptr to avoid unused variable warning
                (void)old_ptr; // Suppress unused variable warning
                size_t check_size = (old_size < new_size) ? old_size : new_size;
                char *data = (char *)allocations[i].ptr;
                for (size_t j = 0; j < check_size; j++) {
                    assert(data[j] == (char)(i % 256));
                }
                
                allocations[i].size = new_size;
                // Fill any new space
                if (new_size > old_size) {
                    memset((char *)allocations[i].ptr + old_size, 
                           (char)(i % 256), new_size - old_size);
                }
            }
        }
    }
    ft_printf("   ✓ Realloc stress test completed\n");
    
    ft_printf("\n5. Testing extreme fragmentation:\n");
    // Allocate many small blocks
    void *small_blocks[1000];
    for (int i = 0; i < 1000; i++) {
        small_blocks[i] = malloc(8);
        if (small_blocks[i]) {
            *((int *)small_blocks[i]) = i;
        }
    }
    ft_printf("   ✓ Allocated 1000 small blocks\n");
    
    // Free every other block to create fragmentation
    for (int i = 1; i < 1000; i += 2) {
        free(small_blocks[i]);
        small_blocks[i] = NULL;
    }
    ft_printf("   ✓ Created fragmentation pattern\n");
    
    // Try to allocate larger blocks
    void *medium_blocks[10];
    for (int i = 0; i < 10; i++) {
        medium_blocks[i] = malloc(64);
        if (medium_blocks[i]) {
            memset(medium_blocks[i], 0xAA, 64);
        }
    }
    ft_printf("   ✓ Allocated medium blocks despite fragmentation\n");
    
    // Cleanup fragmentation test
    for (int i = 0; i < 1000; i += 2) {
        if (small_blocks[i]) {
            assert(*((int *)small_blocks[i]) == i);
            free(small_blocks[i]);
        }
    }
    for (int i = 0; i < 10; i++) {
        if (medium_blocks[i]) {
            free(medium_blocks[i]);
        }
    }
    ft_printf("   ✓ Fragmentation cleanup completed\n");
    
    // Final cleanup
    ft_printf("\n6. Final cleanup:\n");
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (allocations[i].active) {
            free(allocations[i].ptr);
            allocations[i].active = 0;
        }
    }
    
    ft_printf("   Final memory state:\n");
    show_alloc_mem();
    
    ft_printf("\n=== COMPLEX STRESS TEST PASSED ===\n\n");
}

int main() {
    test_stress_malloc();
    return 0;
}
