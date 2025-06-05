#ifndef MALLOC_H
# define MALLOC_H

# include <stddef.h> // size_t
# include <unistd.h> // write - getpagesize

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#endif