// BUILD: clang -fsanitize=address %s -o %t
// CHECK: AddressSanitizer:Use-after-free

#include <cstdlib>

volatile void *buf;
volatile char sink;

int main(int argc, char *argv[]) {
    void *ptr = malloc(1);
    buf = ptr;
    free(ptr);
    sink = *static_cast<char*>(ptr);
    return 0;
}