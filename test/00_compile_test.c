#include <unistd.h>

int main() {
    write(1, "=== COMPILE TEST ===\n", 21);
    return 0;
}