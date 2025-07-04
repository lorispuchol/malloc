#include "utils.h"
#include "malloc.h"

int ft_isprintable(int c) {
    if (c >= 32 && c <= 126)
        return (1);
    return (0);
}

void ft_putchar(char c) {
    write(1, &c, 1);
}

void ft_putstr(const char *str) {
    if (!str) str = "(null)";
    while (*str) {
        ft_putchar(*str++);
    }
}

int ft_strlen(const char *str) {
    if (str == NULL) {
        return 0;
    }
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

size_t ft_min(size_t a, size_t b) {
    if (a < b) {
        return a;
    }
    return b;
}

void *ft_memset(void *s, int c, size_t n) {
    char *ptr = (char *)s;
    while (n--) {
        *ptr++ = (unsigned char)c;
    }
    return s;
}

void *ft_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}