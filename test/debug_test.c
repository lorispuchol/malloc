#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Simple malloc wrapper that logs all calls
void* debug_malloc(size_t size) {
    void *ptr = malloc(size);
    static int call_count = 0;
    call_count++;
    if (call_count < 100) {  // Only log first 100 calls to avoid spam
        fprintf(stderr, "malloc(%zu) = %p [call #%d]\n", size, ptr, call_count);
    }
    return ptr;
}

void debug_free(void *ptr) {
    static int call_count = 0;
    call_count++;
    if (call_count < 100) {
        fprintf(stderr, "free(%p) [call #%d]\n", ptr, call_count);
    }
    free(ptr);
}

void* debug_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    static int call_count = 0;
    call_count++;
    if (call_count < 100) {
        fprintf(stderr, "realloc(%p, %zu) = %p [call #%d]\n", ptr, size, new_ptr, call_count);
    }
    return new_ptr;
}

#define malloc debug_malloc
#define free debug_free  
#define realloc debug_realloc

int main() {
    printf("Testing allocation patterns...\n");
    
    void *ptrs[10];
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc(i * 100 + 1);
    }
    
    for (int i = 0; i < 10; i++) {
        ptrs[i] = realloc(ptrs[i], (i * 200) + 1);
    }
    
    for (int i = 0; i < 10; i++) {
        free(ptrs[i]);
    }
    
    return 0;
}
