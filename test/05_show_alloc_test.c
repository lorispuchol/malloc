#include <stdio.h>
#include "../includes/malloc.h"
#include "../includes/printf.h"

int main() {
    ft_printf("=== SHOW_ALLOC_MEM TEST ===\n");
    
    ft_printf("1. Empty state:\n");
    show_alloc_mem();
    
    ft_printf("\n2. After allocating one from each zone:\n");
    void *tiny = malloc(64);
    void *small = malloc(512);
    void *large = malloc(2048);
    show_alloc_mem();
    
    ft_printf("\n3. After allocating multiple blocks:\n");
    void *tiny2 = malloc(32);
    void *small2 = malloc(256);
    void *large2 = malloc(4096);
    show_alloc_mem();
    
    ft_printf("\n4. After freeing some blocks:\n");
    free(tiny);
    free(small2);
    show_alloc_mem();
    
    ft_printf("\n5. After freeing all:\n");
    free(tiny2);
    free(small);
    // Note: small2 was already freed in step 4
    free(large);
    free(large2);
    show_alloc_mem();
    
    ft_printf("\n6. After new allocations:\n");
    void *new1 = malloc(128);
    void *new2 = malloc(1024);
    void *new3 = malloc(8192);
    show_alloc_mem();
    
    // Cleanup
    free(new1);
    free(new2);
    free(new3);
    
    ft_printf("\n7. Final state (should be empty again):\n");
    show_alloc_mem();
    
    ft_printf("=== SHOW_ALLOC_MEM TEST COMPLETED ===\n\n");
    return 0;
}
