#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"

int main(void) {
    void *ptr1 = malloc(42);        // TINY allocation
    void *ptr2 = malloc(500);       // SMALL allocation  
    void *ptr3 = malloc(2000);      // LARGE allocation
    
    printf("Tiny allocation (42 bytes): %p\n", ptr1);
    printf("Small allocation (500 bytes): %p\n", ptr2);
    printf("Large allocation (2000 bytes): %p\n", ptr3);
    
    // Test realloc
    ptr1 = realloc(ptr1, 100);
    printf("Reallocated tiny to 100 bytes: %p\n", ptr1);
    
    // Show memory state
    show_alloc_mem();
    
    // Free memory
    free(ptr1);
    free(ptr2);
    free(ptr3);
    
    printf("After freeing all:\n");
    show_alloc_mem();
    
    return 0;
}
