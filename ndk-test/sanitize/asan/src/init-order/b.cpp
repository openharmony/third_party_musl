// BUILD: clang -fsanitize=address a.cpp b.cpp -o IO-a
// BUILD: clang -fsanitize=address b.cpp a.cpp -o IO-b (noexpect)

// ASAN_OPTIONS=check_initialization_order=true IO-b
// CHECK: AddressSanitizer: initialization-order-fiasco

int foo() { return 42; }
int extern_global = foo();