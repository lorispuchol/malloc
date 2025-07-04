#include "malloc.h"
#include "printf.h"

t_zone *g_zones = NULL;

size_t align_size(size_t size) {
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}


static void *allocate_tiny_block() {
    for (t_zone *zone = g_zones; zone; zone = zone->next) {
        if (zone->type == TINY && zone->free_list) {
            t_block *block = zone->free_list;
            zone->free_list = block->next_free;
            block->is_free = 0;
            return (void *)((char *)block + BLOCK_HEADER_SIZE);
        }
    }
    return create_tiny_zone();
}

static void *allocate_small_block() {
    for (t_zone *zone = g_zones; zone; zone = zone->next) {
        if (zone->type == SMALL && zone->free_list) {
            t_block *block = zone->free_list;
            zone->free_list = block->next_free;
            block->is_free = 0;
            return (void *)((char *)block + BLOCK_HEADER_SIZE);
        }
    }
    return create_small_zone();
}

static void *allocate_large_block(size_t size) {
    size_t page_size = getpagesize();
    size_t block_size = BLOCK_HEADER_SIZE + size;
    size_t total_size = ZONE_HEADER_SIZE + block_size;
    total_size = (total_size + page_size - 1) & ~(page_size - 1);

    t_zone *zone = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (zone == MAP_FAILED) return NULL;

    zone->type = LARGE;
    zone->total_size = total_size;
    zone->next = g_zones;
    zone->free_list = NULL;
    zone->magic = MAGIC;
    g_zones = zone;

    t_block *block = (t_block *)((char *)zone + ZONE_HEADER_SIZE);
    block->size = size;
    block->is_free = 0;
    return (void *)((char *)block + BLOCK_HEADER_SIZE);
}

void *malloc(size_t size) {
    ft_printf("coucou");
    if (size == 0) return NULL;
    if (size <= TINY_ALLOC_MAX_SIZE) return allocate_tiny_block();
    if (size <= SMALL_ALLOC_MAX_SIZE) return allocate_small_block();
    return allocate_large_block(size);
}