#ifndef UTILS_H
# define UTILS_H


# include <unistd.h>

void my_putchar(char c);
void my_putstr(const char *str);
int my_strlen(const char *str);
void *my_memcpy(void *dest, const void *src, size_t n);

#endif // UTILS_H