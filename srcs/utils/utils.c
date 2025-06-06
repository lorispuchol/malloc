#include "utils.h"

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
    int len = 0;
    while (str[len]) len++;
    return len;
}