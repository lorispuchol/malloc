#include <stdlib.h>

int main () {
    char *ptr = malloc(1);
    // free(ptr);
    char *result = realloc(ptr + 2, 4);  // Invalid pointer: offset from original
    (void)result; // Suppress unused variable warning
    return 0;
}