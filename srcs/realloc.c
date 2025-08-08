#include "malloc.h"
#include "utils.h"

// Helper function to find which page contains a given pointer
static t_page_header *find_page_containing_ptr(t_page_header *zone_head, void *ptr) {
    t_page_header *page = zone_head;
    while (page) {
        char *page_start = (char *)page;
        char *page_end = page_start + page->size;
        if (ptr >= (void *)page_start && ptr < (void *)page_end) {
            return page;
        }
        page = page->next;
    }
    return NULL;
}

// Helper function to find the block header for a given pointer
static t_block_header *find_block_for_ptr(t_page_header *page, void *ptr) {
    t_block_header *block = page->block_list;
    while (block) {
        void *block_data = (char *)block + sizeof(t_block_header);
        if (block_data == ptr) {
            return block;
        }
        block = block->next;
    }
    return NULL;
}

// Helper function to get the size of an allocated block
static size_t get_block_size(void *ptr) {
    if (!ptr || !g_memory_zones) {
        return 0;
    }
    
    t_page_header *page = NULL;
    t_block_header *block = NULL;
    
    // Check tiny zone
    page = find_page_containing_ptr(g_memory_zones->tiny, ptr);
    if (page) {
        block = find_block_for_ptr(page, ptr);
        if (block && !block->is_free) {
            return block->size;
        }
    }
    
    // Check small zone
    page = find_page_containing_ptr(g_memory_zones->small, ptr);
    if (page) {
        block = find_block_for_ptr(page, ptr);
        if (block && !block->is_free) {
            return block->size;
        }
    }
    
    // Check large zone
    page = find_page_containing_ptr(g_memory_zones->large, ptr);
    if (page) {
        block = find_block_for_ptr(page, ptr);
        if (block && !block->is_free) {
            return block->size;
        }
    }
    
    return 0;
}

void *realloc(void *ptr, size_t size) {
    write(1, "=== REALLOC FUNCTION ===\n", 25);
    // If ptr is NULL, behave like malloc
    if (!ptr) {
        return malloc(size);
    }
    
    // If size is 0, behave like free and return NULL
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    
    // Get the current size of the block
    size_t old_size = get_block_size(ptr);
    if (old_size == 0) {
        // Invalid pointer
        return NULL;
    }
    
    // Align the new size
    size_t aligned_size = align_block_size(size);
    
    // If the new size is the same as the old size, return the same pointer
    if (aligned_size == old_size) {
        return ptr;
    }
    
    // Allocate new memory
    void *new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL;
    }
    
    // Copy the data from old to new (use the smaller of the two sizes)
    size_t copy_size = ft_min(old_size, aligned_size);
    ft_memcpy(new_ptr, ptr, copy_size);
    
    // Free the old memory
    free(ptr);
    
    return new_ptr;
}