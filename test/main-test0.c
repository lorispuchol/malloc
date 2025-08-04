int main() {
    void *size = malloc(1);
    printf("Allocated address: %p\n", size);
    free(size);
    free(size);
}