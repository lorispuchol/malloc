#include "utils.h"
#include "malloc.h"

void my_putchar(char c) {
    write(1, &c, 1);
}

void my_putstr(const char *str) {
    if (!str) str = "(null)";
    while (*str) {
        my_putchar(*str++);
    }
}

int my_strlen(const char *str) {
    if (str == NULL) {
        return 0;
    }
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void *my_memcpy(void *dest, const void *src, size_t n) {
    char *d = dest;
    const char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}