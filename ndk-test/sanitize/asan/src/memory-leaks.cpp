// BUILD: clang -fsanitize=address %s -o %t
// CHECK: AddressSanitizer:Memory-leaks

#include <cstdlib>

void* p;

int main(int argc, char *argv[]) {
    p = malloc(7);
    p = 0;
    return 0;
}