#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Testing large allocation...\n");
    
    // Test progressively larger allocations
    void *ptr1 = malloc(1024 * 1024);  // 1MB
    printf("1MB allocation: %p\n", ptr1);
    
    void *ptr2 = malloc(10 * 1024 * 1024);  // 10MB
    printf("10MB allocation: %p\n", ptr2);
    
    void *ptr3 = malloc(100 * 1024 * 1024);  // 100MB
    printf("100MB allocation: %p\n", ptr3);
    
    if (ptr1) free(ptr1);
    if (ptr2) free(ptr2);
    if (ptr3) free(ptr3);
    
    printf("Large allocation test completed\n");
    return 0;
}
