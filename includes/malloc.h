#ifndef MALLOC_H
# define MALLOC_H

# include <stddef.h> // size_t
# include <unistd.h> // write - getpagesize
# include <sys/mman.h> // mmap

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void show_alloc_mem(void);

# define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

# define TINY_ALLOC_MAX_SIZE 128 // n bytes (cf subject)
# define SMALL_ALLOC_MAX_SIZE 1024 // m bytes (cf subject)

# define TINY_ZONE_MIN_CAPACITY 100 // Minimum number of TINY_ALLOC_MAX_SIZE allocations in a tiny zone
# define SMALL_ZONE_MIN_CAPACITY 100 // Minimum number of SMALL_ALLOC_MAX_SIZE allocations in a small zone
# define LARGE_ZONE_MIN_CAPACITY 1 // Number of allocations in a large zone

# define ALIGNMENT 16 // Memory alignment in bytes

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