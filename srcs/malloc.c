#include "malloc.h"

static t_memory_zones g_memory_zones_struct = {NULL, NULL, NULL};
t_memory_zones *g_memory_zones = &g_memory_zones_struct;

static t_memory_zones *init_memory_zones() {
    return g_memory_zones;
}

size_t align_block_size(size_t size) {
    // used to align the block by rounding up its size to the nearest multiple of ALIGNMENT
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

size_t align_page_size(size_t size) {
    // used to align the claim page size by rounding up its size to the nearest multiple of the system page size
    size_t page_size = PAGE_SIZE;
    return (size + page_size - 1) & ~(page_size - 1);
}

static t_block_header *find_free_block(t_page_header *page, size_t size) {
    t_block_header *block = page->block_list;
    while (block) {
        if (block->is_free && block->size >= size) {
            return block;
        }
        block = block->next;
    }
    return NULL;
}

static t_block_header *split_block(t_block_header *block, size_t size) {
    if (block->size < size + sizeof(t_block_header) + ALIGNMENT) {
        // Not enough space to split
        return block;
    }
    
    t_block_header *new_block = (t_block_header *)((char *)block + sizeof(t_block_header) + size);
    new_block->size = block->size - size - sizeof(t_block_header);
    new_block->is_free = true;
    new_block->next = block->next;
    new_block->prev = block;
    
    if (block->next) {
        block->next->prev = new_block;
    }
    block->next = new_block;
    block->size = size;
    
    return block;
}

static void *allocate_from_zone(t_page_header **zone_head, t_zone_type type, size_t size) {
    t_page_header *page = *zone_head;
    
    // Look for existing free block in existing pages
    while (page) {
        t_block_header *block = find_free_block(page, size);
        if (block) {
            block->is_free = false;
            split_block(block, size);
            return (char *)block + sizeof(t_block_header);
        }
        page = page->next;
    }
    
    // Need to allocate a new page
    size_t block_max_size = (type == TINY) ? TINY_BLOCK_MAX_SIZE : SMALL_BLOCK_MAX_SIZE;
    size_t zone_size = align_page_size(MIN_CAPACITY * (block_max_size + sizeof(t_block_header)) + sizeof(t_page_header));
    
    t_page_header *new_page = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_page == MAP_FAILED) {
        return NULL;
    }
    
    new_page->type = type;
    new_page->size = zone_size;
    new_page->next = *zone_head;
    new_page->prev = NULL;
    if (*zone_head) {
        (*zone_head)->prev = new_page;
    }
    *zone_head = new_page;
    
    // Initialize the first block
    t_block_header *first_block = (t_block_header *)((char *)new_page + sizeof(t_page_header));
    first_block->size = zone_size - sizeof(t_page_header) - sizeof(t_block_header);
    first_block->is_free = true;
    first_block->next = NULL;
    first_block->prev = NULL;
    new_page->block_list = first_block;
    
    // Allocate from this block
    first_block->is_free = false;
    split_block(first_block, size);
    return (char *)first_block + sizeof(t_block_header);
}

static void *allocate_tiny_block(size_t size) {
    if (!init_memory_zones()) {
        return NULL;
    }
    return allocate_from_zone(&g_memory_zones->tiny, TINY, size);
}

static void *allocate_small_block(size_t size) {
    if (!init_memory_zones()) {
        return NULL;
    }
    return allocate_from_zone(&g_memory_zones->small, SMALL, size);
}

static void *allocate_large_block(size_t size) {
    if (!init_memory_zones()) {
        return NULL;
    }
    
    // For large allocations, each block gets its own mmap call
    // This ensures complete isolation between large allocations
    size_t total_size = align_page_size(sizeof(t_page_header) + sizeof(t_block_header) + size);
    t_page_header *new_page = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_page == MAP_FAILED) {
        return NULL;
    }
    
    new_page->type = LARGE;
    new_page->size = total_size;
    new_page->next = g_memory_zones->large;
    new_page->prev = NULL;
    if (g_memory_zones->large) {
        g_memory_zones->large->prev = new_page;
    }
    g_memory_zones->large = new_page;
    
    t_block_header *block = (t_block_header *)((char *)new_page + sizeof(t_page_header));
    block->size = size;
    block->is_free = false;
    block->next = NULL;
    block->prev = NULL;
    new_page->block_list = block;
    
    return (char *)block + sizeof(t_block_header);
}

void *malloc(size_t size) {
    // write(1, "=== MALLOC FUNCTION ===\n", 24);
    if (size == 0) {
        return NULL;
    }
    size = align_block_size(size);
    if (size <= TINY_BLOCK_MAX_SIZE) {
        return allocate_tiny_block(size);
    }
    if (size <= SMALL_BLOCK_MAX_SIZE) {
        return allocate_small_block(size);
    }
    return allocate_large_block(size);
}

