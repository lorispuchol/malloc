#include "malloc.h"
#include "printf.h"

#include "malloc.h"
#include "utils.h"

t_zone *g_zones = NULL;

void *create_tiny_zone() {
    size_t page_size = getpagesize();
    size_t zone_size = ZONE_HEADER_SIZE + TINY_ZONE_ALLOCATIONS * (BLOCK_HEADER_SIZE + TINY_BLOCK_SIZE);
    zone_size = (zone_size + page_size - 1) & ~(page_size - 1);

    t_zone *zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (zone == MAP_FAILED) return NULL;

    zone->type = TINY;
    zone->total_size = zone_size;
    zone->next = g_zones;
    zone->free_list = (t_block *)((char *)zone + ZONE_HEADER_SIZE);
    zone->magic = MAGIC;
    g_zones = zone;

    t_block *block = zone->free_list;
    for (int i = 0; i < TINY_ZONE_ALLOCATIONS - 1; ++i) {
        block->size = TINY_BLOCK_SIZE;
        block->is_free = 1;
        block->next_free = (t_block *)((char *)block + BLOCK_HEADER_SIZE + TINY_BLOCK_SIZE);
        block = block->next_free;
    }
    block->size = TINY_BLOCK_SIZE;
    block->is_free = 1;
    block->next_free = NULL;

    block = zone->free_list;
    zone->free_list = block->next_free;
    block->is_free = 0;
    return (void *)((char *)block + BLOCK_HEADER_SIZE);
}

void *create_small_zone() {
    size_t page_size = getpagesize();
    size_t block_size = BLOCK_HEADER_SIZE + SMALL_BLOCK_SIZE;
    size_t zone_size = ZONE_HEADER_SIZE + SMALL_ZONE_ALLOCATIONS * block_size;
    
    // Align zone size to page boundaries
    zone_size = (zone_size + page_size - 1) & ~(page_size - 1);

    t_zone *zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, 
                       MAP_PRIVATE | MAP_ANON, -1, 0);
    if (zone == MAP_FAILED) return NULL;

    // Initialize zone metadata
    zone->type = SMALL;
    zone->total_size = zone_size;
    zone->next = g_zones;
    zone->free_list = (t_block *)((char *)zone + ZONE_HEADER_SIZE);
    zone->magic = MAGIC;
    g_zones = zone;

    // Initialize all blocks in the zone
    t_block *block = zone->free_list;
    for (int i = 0; i < SMALL_ZONE_ALLOCATIONS - 1; ++i) {
        block->size = SMALL_BLOCK_SIZE;
        block->is_free = 1;
        block->next_free = (t_block *)((char *)block + block_size);
        block = block->next_free;
    }
    
    // Terminate the last block
    block->size = SMALL_BLOCK_SIZE;
    block->is_free = 1;
    block->next_free = NULL;

    // Allocate the first block from this new zone
    block = zone->free_list;
    zone->free_list = block->next_free;
    block->is_free = 0;
    
    return (void *)((char *)block + BLOCK_HEADER_SIZE);
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
    if (size == 0) return NULL;
    if (size <= TINY_BLOCK_SIZE) return allocate_tiny_block();
    if (size <= SMALL_BLOCK_SIZE) return allocate_small_block();
    return allocate_large_block(size);
}