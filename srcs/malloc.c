#include "malloc.h"
#include "printf.h"

t_memory_zones *g_memory_zones = {0};

size_t align_block_size(size_t size) {
    // used to align the block by rounding up its size to the nearest multiple of ALIGNMENT
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

size_t align_page_size(size_t size) {
    // used to align the claim page size by rounding up its size to the nearest multiple of the system page size
    return (size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}


static void *allocate_tiny_block() {
    if (!g_memory_zones->tiny) {
        // Allocate a new tiny zone if it doesn't exist
        size_t zone_size = align_page_size(MIN_CAPACITY * (TINY_BLOCK_MAX_SIZE + sizeof(t_block_header)) + sizeof(t_page_header));
        t_page_header *new_zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (new_zone == MAP_FAILED) {
            return NULL; // mmap failed
        }
        new_zone->type = TINY;
        new_zone->size = zone_size;
        new_zone->block_list = NULL; // Initialize the block list
        new_zone->next = NULL;
        new_zone->prev = NULL;

        g_memory_zones->tiny = new_zone;
    }
    // Allocate a block from the tiny zone
}

// static void *allocate_small_block() {

// }

// static void *allocate_large_block(size_t size) {

// }

void *malloc(size_t size) {
    if (size <= TINY_BLOCK_MAX_SIZE) return allocate_tiny_block();
    // if (size <= SMALL_BLOCK_MAX_SIZE) return allocate_small_block();
    // return allocate_large_block(size);
}

