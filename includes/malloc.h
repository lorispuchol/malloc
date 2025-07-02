#ifndef MALLOC_H
# define MALLOC_H

# include <stddef.h> // size_t
# include <unistd.h> // write - getpagesize
# include <sys/mman.h> // mmap

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void show_alloc_mem();


# define TINY_BLOCK_SIZE 128
# define SMALL_BLOCK_SIZE 1024
# define BLOCK_HEADER_SIZE 32
# define MAGIC 0x12345678
# define ZONE_HEADER_SIZE sizeof(t_zone)
# define TINY_ZONE_ALLOCATIONS 100
# define SMALL_ZONE_ALLOCATIONS 100

typedef enum e_zone_type {
    TINY,
    SMALL,
    LARGE
} t_zone_type;

typedef struct s_zone {
    t_zone_type type;
    size_t total_size;
    struct s_zone *next;
    struct s_block *free_list;
    int magic;
} t_zone;

typedef struct s_block {
    size_t size;
    int is_free;
    struct s_block *next_free;
} t_block;

extern t_zone *g_zones;


#endif // MALLOC_H