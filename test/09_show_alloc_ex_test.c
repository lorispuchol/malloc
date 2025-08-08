#include <stdio.h>
#include <string.h>
#include "../includes/malloc.h"
#include "../includes/printf.h"

int main() {
    ft_printf("=== EXTENDED MEMORY ANALYSIS TEST ===\n\n");
    
    ft_printf("1. Initial state:\n");
    show_alloc_mem_ex();
    
    ft_printf("\n2. Allocating various sizes with data:\n");
    
    // Allocate and fill with different patterns
    char *tiny1 = (char*)malloc(32);
    strcpy(tiny1, "Hello World!");
    
    char *tiny2 = (char*)malloc(64);
    strcpy(tiny2, "TINY block with some text data");
    
    char *small1 = (char*)malloc(256);
    strcpy(small1, "This is a SMALL block containing more data for testing purposes. ");
    strcat(small1, "It should show up nicely in the hex dump!");
    
    char *small2 = (char*)malloc(512);
    for (int i = 0; i < 100; i++) {
        small2[i] = 'A' + (i % 26);
    }
    small2[100] = '\0';
    
    char *large1 = (char*)malloc(2048);
    strcpy(large1, "LARGE allocation with pattern: ");
    for (int i = 31; i < 200; i++) {
        large1[i] = '0' + (i % 10);
    }
    large1[200] = '\0';
    
    char *large2 = (char*)malloc(4096);
    memset(large2, 0xAB, 100);  // Fill with pattern
    strcpy(large2 + 100, " <- Pattern ends here");
    
    show_alloc_mem_ex();
    
    ft_printf("\n3. After freeing some blocks (creating fragmentation):\n");
    free(tiny1);
    free(small1);
    
    show_alloc_mem_ex();
    
    ft_printf("\n4. Allocating into freed spaces:\n");
    char *new_tiny = (char*)malloc(16);
    strcpy(new_tiny, "New tiny");
    
    char *new_small = (char*)malloc(128);
    strcpy(new_small, "New small block in freed space");
    
    show_alloc_mem_ex();
    
    ft_printf("\n5. Comparison with standard show_alloc_mem():\n");
    ft_printf("Standard output:\n");
    show_alloc_mem();
    
    // Cleanup
    free(tiny2);
    free(small2);
    free(large1);
    free(large2);
    free(new_tiny);
    free(new_small);
    
    ft_printf("\n6. Final state after cleanup:\n");
    show_alloc_mem_ex();
    
    ft_printf("=== EXTENDED MEMORY ANALYSIS TEST COMPLETED ===\n");
    return 0;
}
