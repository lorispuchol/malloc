#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("Testing buffer expansion patterns like flex might use...\n");
    
    // Start with a small buffer
    size_t buffer_size = 16;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        printf("Initial malloc failed\n");
        return 1;
    }
    
    // Simulate buffer expansion like flex does
    for (int i = 0; i < 20; i++) {
        buffer_size *= 2;  // Double the size
        char *new_buffer = realloc(buffer, buffer_size);
        if (!new_buffer) {
            printf("realloc failed at size %zu\n", buffer_size);
            free(buffer);
            return 1;
        }
        buffer = new_buffer;
        printf("Successfully reallocated to %zu bytes\n", buffer_size);
        
        // Fill with some data
        memset(buffer, 'A', buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
    }
    
    free(buffer);
    printf("Buffer expansion test completed successfully\n");
    return 0;
}
