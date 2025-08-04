#include "malloc.h"
#include "utils.h"
#include "printf.h"
#include <stdint.h> // for uintptr_t

// Helper function to print address in hexadecimal format
static void print_hex_address(void *addr) {
    ft_printf("0x%X", (unsigned long)addr);
}

// Helper function to show blocks in a specific zone
static size_t show_zone_blocks(t_page_header *zone_head, const char *zone_name) {
    size_t total_bytes = 0;
    t_page_header *page = zone_head;
    
    if (!page) {
        return 0;
    }
    
    ft_printf("%s : ", zone_name);
    print_hex_address(page);
    ft_printf("\n");
    
    while (page) {
        t_block_header *block = page->block_list;
        while (block) {
            if (!block->is_free) {
                void *block_start = (char *)block + sizeof(t_block_header);
                void *block_end = (char *)block_start + block->size;
                
                print_hex_address(block_start);
                ft_printf(" - ");
                print_hex_address(block_end);
                ft_printf(" : %u bytes\n", (unsigned int)block->size);
                
                total_bytes += block->size;
            }
            block = block->next;
        }
        page = page->next;
    }
    
    return total_bytes;
}

void show_alloc_mem() {
    if (!g_memory_zones) {
        ft_printf("Total : 0 bytes\n");
        return;
    }
    
    size_t total_bytes = 0;
    
    // Show TINY zone
    total_bytes += show_zone_blocks(g_memory_zones->tiny, "TINY");
    
    // Show SMALL zone
    total_bytes += show_zone_blocks(g_memory_zones->small, "SMALL");
    
    // Show LARGE zone
    total_bytes += show_zone_blocks(g_memory_zones->large, "LARGE");
    
    ft_printf("Total : %u bytes\n", (unsigned int)total_bytes);
}