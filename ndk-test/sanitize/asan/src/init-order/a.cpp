// BUILD: clang -fsanitize=address a.cpp b.cpp -o IO-a
// BUILD: clang -fsanitize=address b.cpp a.cpp -o IO-b (noexpect)

// ASAN_OPTIONS=check_initialization_order=true IO-a
// ASAN_OPTIONS=check_initialization_order=true IO-b (noexpect)
// CHECK: AddressSanitizer: Initialization-order-fiasco

#include <cstdio>

extern int extern_global;

int __attribute__((noinline)) read_extern_global()
{
    return extern_global;
}

int x = read_extern_global() + 1;

int main()
{
    printf("%d\n", x);
    return 0;
}
