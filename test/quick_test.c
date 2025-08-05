#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *p = malloc(100);
    strcpy(p, "Hello World from our malloc!");
    printf("%s\n", p);
    free(p);
    return 0;
}
