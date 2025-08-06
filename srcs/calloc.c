#include "malloc.h"
#include "utils.h"

void *calloc(size_t count, size_t size) {
    // Check for overflow
    if (count != 0 && size > SIZE_MAX / count) {
        return NULL;
    }
    
    size_t total_size = count * size;
    
    // Handle zero size
    if (total_size == 0) {
        return NULL;
    }
    
    // Allocate memory
    void *ptr = malloc(total_size);
    if (!ptr) {
        return NULL;
    }
    
    // Zero out the memory
    ft_memset(ptr, 0, total_size);
    
    return ptr;
}
