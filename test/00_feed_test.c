# include <unistd.h>
# include "../includes/malloc.h"



int main() {
    char *ptr = malloc(100);
    int i = 0;
    while (i < 100) {
        ptr[i] = 'A';
        i++;
    }
    show_alloc_mem_ex(); 
    return 0;
}