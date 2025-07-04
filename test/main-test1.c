# include <stdlib.h>
# include <stdio.h>

int main (){
    void *ptr = malloc(1);
    printf("malloc(0) returned: %p\n", ptr);
    free(ptr);
    free(ptr);
}