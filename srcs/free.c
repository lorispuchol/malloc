#include "malloc.h"
#include "utils.h"
#include "printf.h"
#include <stdlib.h>

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

// Helper function to coalesce adjacent free blocks
static void coalesce_free_blocks(t_block_header *block) {
    // Coalesce with next blocks if they're free
    while (block->next && block->next->is_free) {
        t_block_header *next_block = block->next;
        block->size += sizeof(t_block_header) + next_block->size;
        block->next = next_block->next;
        if (next_block->next) {
            next_block->next->prev = block;
        }
    }
    
    // Coalesce with previous blocks if they're free (symmetric defensive approach)
    while (block->prev && block->prev->is_free) {
        t_block_header *prev_block = block->prev;
        prev_block->size += sizeof(t_block_header) + block->size;
        prev_block->next = block->next;
        if (block->next) {
            block->next->prev = prev_block;
        }
        // Update reference point for next iteration
        block = prev_block;
    }
}

// Helper function to remove empty pages from tiny/small zones
static void cleanup_empty_page(t_page_header **zone_head, t_page_header *page) {
    // Only cleanup tiny/small zones, not large zones
    if (page->type == LARGE) {
        return;
    }
    
    // Check if the entire page is one free block
    t_block_header *block = page->block_list;
    if (block && block->is_free && !block->next && 
        block->size >= page->size - sizeof(t_page_header) - sizeof(t_block_header)) {
        
        // Remove page from linked list
        if (page->prev) {
            page->prev->next = page->next;
        } else {
            *zone_head = page->next;
        }
        if (page->next) {
            page->next->prev = page->prev;
        }
        
        // Unmap the page
        munmap(page, page->size);
    }
}

void free(void *ptr) {
    // write(1, "=== FREE FUNCTION ===\n", 23); 
    if (!ptr || !g_memory_zones) {
        return;  // Standard behavior: free(NULL) is safe
    }
    
    t_page_header *page = NULL;
    t_block_header *block = NULL;
    
    // Find which zone contains this pointer
    page = find_page_containing_ptr(g_memory_zones->tiny, ptr);
    if (page) {
        block = find_block_for_ptr(page, ptr);
        if (block) {
            if (block->is_free) {
                // Double free detected - abort like real free()
                ft_printf("*** Error: double free or corruption (tiny): %p ***\n", ptr);
                abort();
            }
            block->is_free = true;
            coalesce_free_blocks(block);
            // if(g_memory_zones->small->next)
            cleanup_empty_page(&g_memory_zones->tiny, page);
            return;
        } else {
            // Pointer is within page range but not a valid block start
            ft_printf("*** Error: free(): invalid pointer %p (corrupted tiny zone) ***\n", ptr);
            abort();
        }
    }
    
    page = find_page_containing_ptr(g_memory_zones->small, ptr);
    if (page) {
        block = find_block_for_ptr(page, ptr);
        if (block) {
            if (block->is_free) {
                // Double free detected - abort like real free()
                ft_printf("*** Error: double free or corruption (small): %p ***\n", ptr);
                abort();
            }
            block->is_free = true;
            coalesce_free_blocks(block);
            // if (g_memory_zones->small->next)
            cleanup_empty_page(&g_memory_zones->small, page);
            return;
        } else {
            // Pointer is within page range but not a valid block start
            ft_printf("*** Error: free(): invalid pointer %p (corrupted small zone) ***\n", ptr);
            abort();
        }
    }
    
    page = find_page_containing_ptr(g_memory_zones->large, ptr);
    if (page) {
        block = find_block_for_ptr(page, ptr);
        if (block) {
            if (block->is_free) {
                // This shouldn't happen for large blocks since they get unmapped
                ft_printf("*** Error: double free or corruption (large): %p ***\n", ptr);
                abort();
            }
            // For large blocks, we immediately unmap the entire page
            if (page->prev) {
                page->prev->next = page->next;
            } else {
                g_memory_zones->large = page->next;
            }
            if (page->next) {
                page->next->prev = page->prev;
            }
            munmap(page, page->size);
            return;
        } else {
            // Pointer is within page range but not a valid block start
            ft_printf("*** Error: free(): invalid pointer %p (corrupted large zone) ***\n", ptr);
            abort();
        }
    }
    
    // Invalid pointer - abort like real free()
    ft_printf("*** Error: free(): invalid pointer %p ***\n", ptr);
    abort();
}
