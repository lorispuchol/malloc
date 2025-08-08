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

// Helper function to print hex dump of memory content
static void print_hex_dump(void *ptr, size_t size, size_t max_display) {
    unsigned char *data = (unsigned char *)ptr;
    size_t display_size = (size > max_display) ? max_display : size;
    
    ft_printf("    Hex dump (first %u bytes):\n", (unsigned int)display_size);
    
    for (size_t i = 0; i < display_size; i += 16) {
        // Print address
        ft_printf("    ");
        print_hex_address((char *)ptr + i);
        ft_printf(": ");
        
        // Print hex values
        for (size_t j = 0; j < 16 && (i + j) < display_size; j++) {
            ft_printf("%02x ", data[i + j]);
        }
        
        // Pad with spaces if needed
        for (size_t j = display_size - i; j < 16 && j > 0; j++) {
            ft_printf("   ");
        }
        
        // Print ASCII representation
        ft_printf(" |");
        for (size_t j = 0; j < 16 && (i + j) < display_size; j++) {
            char c = data[i + j];
            if (ft_isprintable(c)) {
                ft_printf("%c", c);
            } else {
                ft_printf(".");
            }
        }
        ft_printf("|\n");
    }
    
    if (size > max_display) {
        ft_printf("    ... (%u more bytes)\n", (unsigned int)(size - max_display));
    }
}

// Helper function to get memory statistics for a zone
static void get_zone_stats(t_page_header *zone_head, const char *zone_name, 
                          size_t *total_allocated, size_t *total_free, 
                          size_t *block_count, size_t *free_count, size_t *page_count) {
    (void)zone_name; // Suppress unused parameter warning
    *total_allocated = 0;
    *total_free = 0;
    *block_count = 0;
    *free_count = 0;
    *page_count = 0;
    
    t_page_header *page = zone_head;
    while (page) {
        (*page_count)++;
        t_block_header *block = page->block_list;
        while (block) {
            (*block_count)++;
            if (block->is_free) {
                (*free_count)++;
                *total_free += block->size;
            } else {
                *total_allocated += block->size;
            }
            block = block->next;
        }
        page = page->next;
    }
}

// Helper function to show detailed zone information with hex dumps
static void show_zone_blocks_ex(t_page_header *zone_head, const char *zone_name) {
    if (!zone_head) {
        ft_printf("=== %s ZONE: EMPTY ===\n\n", zone_name);
        return;
    }
    
    // Get statistics
    size_t total_allocated, total_free, block_count, free_count, page_count;
    get_zone_stats(zone_head, zone_name, &total_allocated, &total_free, 
                  &block_count, &free_count, &page_count);
    
    ft_printf("=== %s ZONE DETAILS ===\n", zone_name);
    ft_printf("  Pages: %u\n", (unsigned int)page_count);
    ft_printf("  Total blocks: %u (allocated: %u, free: %u)\n", 
             (unsigned int)block_count, 
             (unsigned int)(block_count - free_count), 
             (unsigned int)free_count);
    ft_printf("  Total allocated: %u bytes\n", (unsigned int)total_allocated);
    ft_printf("  Total free: %u bytes\n", (unsigned int)total_free);
    ft_printf("  Fragmentation: %u%%\n", 
             total_allocated > 0 ? (unsigned int)((free_count * 100) / block_count) : 0);
    
    // Show page details
    t_page_header *page = zone_head;
    size_t page_num = 1;
    
    while (page) {
        ft_printf("\n  Page %u: ", (unsigned int)page_num);
        print_hex_address(page);
        ft_printf(" (size: %u bytes)\n", (unsigned int)page->size);
        
        t_block_header *block = page->block_list;
        size_t block_num = 1;
        
        while (block) {
            void *block_start = (char *)block + sizeof(t_block_header);
            void *block_end = (char *)block_start + block->size;
            
            ft_printf("    Block %u: ", (unsigned int)block_num);
            print_hex_address(block_start);
            ft_printf(" - ");
            print_hex_address(block_end);
            ft_printf(" : %u bytes [%s]\n", 
                     (unsigned int)block->size,
                     block->is_free ? "FREE" : "ALLOCATED");
            
            // Show hex dump for allocated blocks
            if (!block->is_free && block->size > 0) {
                print_hex_dump(block_start, block->size, 64); // Show first 64 bytes
            }
            
            block = block->next;
            block_num++;
        }
        
        page = page->next;
        page_num++;
    }
    ft_printf("\n");
}

void show_alloc_mem_ex() {
    ft_printf("=====================================\n");
    ft_printf("     EXTENDED MEMORY ANALYSIS\n");
    ft_printf("=====================================\n\n");
    
    if (!g_memory_zones) {
        ft_printf("Memory zones not initialized.\n");
        return;
    }
    
    // Global statistics
    size_t total_allocated = 0, total_free = 0, total_blocks = 0, total_free_blocks = 0, total_pages = 0;
    
    // Calculate global stats
    size_t zone_alloc, zone_free, zone_blocks, zone_free_blocks, zone_pages;
    
    get_zone_stats(g_memory_zones->tiny, "TINY", &zone_alloc, &zone_free, &zone_blocks, &zone_free_blocks, &zone_pages);
    total_allocated += zone_alloc; total_free += zone_free; total_blocks += zone_blocks; total_free_blocks += zone_free_blocks; total_pages += zone_pages;
    
    get_zone_stats(g_memory_zones->small, "SMALL", &zone_alloc, &zone_free, &zone_blocks, &zone_free_blocks, &zone_pages);
    total_allocated += zone_alloc; total_free += zone_free; total_blocks += zone_blocks; total_free_blocks += zone_free_blocks; total_pages += zone_pages;
    
    get_zone_stats(g_memory_zones->large, "LARGE", &zone_alloc, &zone_free, &zone_blocks, &zone_free_blocks, &zone_pages);
    total_allocated += zone_alloc; total_free += zone_free; total_blocks += zone_blocks; total_free_blocks += zone_free_blocks; total_pages += zone_pages;
    
    // Show global summary
    ft_printf("GLOBAL SUMMARY:\n");
    ft_printf("  Total pages: %u\n", (unsigned int)total_pages);
    ft_printf("  Total blocks: %u (allocated: %u, free: %u)\n", 
             (unsigned int)total_blocks, 
             (unsigned int)(total_blocks - total_free_blocks), 
             (unsigned int)total_free_blocks);
    ft_printf("  Total allocated memory: %u bytes\n", (unsigned int)total_allocated);
    ft_printf("  Total free memory: %u bytes\n", (unsigned int)total_free);
    ft_printf("  Memory efficiency: %u%%\n", 
             (total_allocated + total_free) > 0 ? 
             (unsigned int)((total_allocated * 100) / (total_allocated + total_free)) : 0);
    ft_printf("  Average block size: %u bytes\n", 
             total_blocks > 0 ? (unsigned int)(total_allocated / (total_blocks - total_free_blocks)) : 0);
    ft_printf("\n");
    
    // Show detailed zone information
    show_zone_blocks_ex(g_memory_zones->tiny, "TINY");
    show_zone_blocks_ex(g_memory_zones->small, "SMALL");
    show_zone_blocks_ex(g_memory_zones->large, "LARGE");
    
    ft_printf("=====================================\n");
    ft_printf("       ANALYSIS COMPLETE\n");
    ft_printf("=====================================\n");
}