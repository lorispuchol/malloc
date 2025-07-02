#include "malloc.h"
#include "utils.h"

void *realloc(void *ptr, size_t size) {
    if (!ptr) return malloc(size);
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    t_block *block = (t_block *)((char *)ptr - BLOCK_HEADER_SIZE);
    t_zone *zone = (t_zone *)((char *)block - ZONE_HEADER_SIZE);
    if (zone->magic != MAGIC) return NULL;

    t_zone_type new_type = (size <= TINY_BLOCK_SIZE) ? TINY : (size <= SMALL_BLOCK_SIZE) ? SMALL : LARGE;
    if ((zone->type == TINY || zone->type == SMALL) && new_type == zone->type) {
        block->size = size;
        return ptr;
    }
    if (zone->type == LARGE && new_type == LARGE && size <= block->size) {
        block->size = size;
        return ptr;
    }

    void *new_ptr = malloc(size);
    if (!new_ptr) return NULL;
    size_t cpy_size = (block->size < size) ? block->size : size;
    my_memcpy(new_ptr, ptr, cpy_size);
    free(ptr);
    return new_ptr;
}