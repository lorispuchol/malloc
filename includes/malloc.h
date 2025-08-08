#ifndef MALLOC_H
# define MALLOC_H

# define _GNU_SOURCE
# include <stddef.h> // size_t
# include <unistd.h> // write - getpagesize
# include <sys/mman.h> // mmap
# include <stdbool.h> // bool
# include <stdint.h> // SIZE_MAX

void *malloc(size_t size);
void *calloc(size_t count, size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void show_alloc_mem(void);
void show_alloc_mem_ex(void);

// Utility functions
size_t align_block_size(size_t size);
size_t align_page_size(size_t size);

# define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

# define TINY_BLOCK_MAX_SIZE 128 // Maximum size of a tiny block (subject: n bytes)
# define SMALL_BLOCK_MAX_SIZE 1024 // Maximum size of a small block (subject: m bytes)

# define MIN_CAPACITY 100 // Minimum number of block of MAX_SIZE size that tiny and small zone must contain with one call of mmap
# define LARGE_ZONE_MIN_CAPACITY 1 // Minimum number of blocks in a large zone


# define ALIGNMENT 16 // Memory alignment in bytes

typedef enum e_zone_type t_zone_type;
typedef struct s_memory_zones t_memory_zones;
typedef struct s_page_header t_page_header;
typedef struct s_block_header t_block_header;

enum e_zone_type {
    TINY,
    SMALL,
    LARGE
};

struct s_memory_zones {
    t_page_header *tiny;
    t_page_header *small;
    t_page_header *large;
};

// This structure represents the header of one or more successive pages allocated by one call of mmap.
struct s_page_header {
    t_zone_type type;
    t_block_header *block_list;
    size_t size;
    t_page_header *next;
    t_page_header *prev;
};

struct s_block_header {
    size_t size;
    bool is_free;
    t_block_header *next;
    t_block_header *prev;
};
    
extern t_memory_zones *g_memory_zones;

#endif // MALLOC_H