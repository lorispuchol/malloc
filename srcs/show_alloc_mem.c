#include "malloc.h"
#include "utils.h"

static void print_hex_address(void *addr) {
    intptr_t a = (intptr_t)addr;
    char buf[20];
    int i = 0;
    if (a == 0) {
        write(1, "0x0", 3);
        return;
    }
    while (a) {
        buf[i++] = "0123456789abcdef"[a % 16];
        a /= 16;
    }
    write(1, "0x", 2);
    while (--i >= 0) write(1, &buf[i], 1);
}

static void print_number(size_t n) {
    if (n == 0) {
        write(1, "0", 1);
        return;
    }
    char buf[20];
    int i = 0;
    while (n) {
        buf[i++] = '0' + n % 10;
        n /= 10;
    }
    while (--i >= 0) write(1, &buf[i], 1);
}

void show_alloc_mem() {
    struct block_info {
        void *ptr;
        size_t size;
        t_zone_type type;
    } blocks[100000];
    int count = 0;

    for (t_zone *zone = g_zones; zone; zone = zone->next) {
        if (zone->type == TINY || zone->type == SMALL) {
            size_t block_size = (zone->type == TINY) ? 
                (BLOCK_HEADER_SIZE + TINY_ALLOC_MAX_SIZE) : 
                (BLOCK_HEADER_SIZE + SMALL_ALLOC_MAX_SIZE);
            t_block *block = (t_block *)((char *)zone + ZONE_HEADER_SIZE);
            for (int i = 0; i < (zone->type == TINY ? TINY_ZONE_ALLOCATIONS : SMALL_ZONE_ALLOCATIONS); ++i) {
                if (!block->is_free && count < 100000) {
                    blocks[count].ptr = (void *)((char *)block + BLOCK_HEADER_SIZE);
                    blocks[count].size = block->size;
                    blocks[count].type = zone->type;
                    count++;
                }
                block = (t_block *)((char *)block + block_size);
            }
        } else if (zone->type == LARGE) {
            t_block *block = (t_block *)((char *)zone + ZONE_HEADER_SIZE);
            if (!block->is_free && count < 100000) {
                blocks[count].ptr = (void *)((char *)block + BLOCK_HEADER_SIZE);
                blocks[count].size = block->size;
                blocks[count].type = LARGE;
                count++;
            }
        }
    }

    // Sort blocks by address (omitted for brevity; use a simple sort in real code)
    size_t total = 0;
    void *last_zone_start = NULL;
    for (int i = 0; i < count; ++i) {
        t_block *block = (t_block *)((char *)blocks[i].ptr - BLOCK_HEADER_SIZE);
        t_zone *zone = (t_zone *)((char *)block - ZONE_HEADER_SIZE);
        if (last_zone_start != (void *)zone) {
            last_zone_start = zone;
            const char *type_str = (blocks[i].type == TINY) ? "TINY" : (blocks[i].type == SMALL) ? "SMALL" : "LARGE";
            write(1, type_str, ft_strlen(type_str));
            write(1, " : ", 3);
            print_hex_address(last_zone_start);
            write(1, "\n", 1);
        }
        print_hex_address(blocks[i].ptr);
        write(1, " - ", 3);
        print_hex_address(blocks[i].ptr + blocks[i].size);
        write(1, " : ", 3);
        print_number(blocks[i].size);
        write(1, " bytes\n", 7);
        total += blocks[i].size;
    }
    write(1, "Total : ", 8);
    print_number(total);
    write(1, " bytes\n", 7);
}