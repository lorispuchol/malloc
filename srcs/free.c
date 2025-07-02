#include "malloc.h"
#include "utils.h"

void free(void *ptr) {
    if (!ptr) return;

    t_block *block = (t_block *)((char *)ptr - BLOCK_HEADER_SIZE);
    t_zone *zone = (t_zone *)((char *)block - ZONE_HEADER_SIZE);

    if (zone->magic != MAGIC) return;

    if (zone->type == LARGE) {
        if (g_zones == zone) g_zones = zone->next;
        else {
            t_zone *prev = g_zones;
            while (prev->next != zone) prev = prev->next;
            prev->next = zone->next;
        }
        munmap(zone, zone->total_size);
    } else {
        block->is_free = 1;
        block->next_free = zone->free_list;
        zone->free_list = block;
    }
}