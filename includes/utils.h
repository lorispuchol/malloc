#ifndef UTILS_H
# define UTILS_H


# include <unistd.h>

int     ft_isprintable(int c);
void    ft_putchar(char c);
void    ft_putstr(const char *str);
int     ft_strlen(const char *str);
size_t  ft_min(size_t a, size_t b);
void    *ft_memset(void *s, int c, size_t n);
void    *ft_memcpy(void *dest, const void *src, size_t n);

#endif // UTILS_H