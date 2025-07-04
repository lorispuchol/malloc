#include "malloc.h"
#include "printf.h"

t_memory_zones g_memory_zones = {0};

size_t get_round_size(size_t size) {
    // used to align the size to the nearest multiple of ALIGNMENT
    return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}


static void *allocate_tiny_block() {

}

static void *allocate_small_block() {

}

static void *allocate_large_block(size_t size) {

}

void *malloc(size_t size) {
    if (size <= TINY_BLOCK_MAX_SIZE) return allocate_tiny_block();
    if (size <= SMALL_BLOCK_MAX_SIZE) return allocate_small_block();
    return allocate_large_block(size);
}