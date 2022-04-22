// BUILD: clang -fsanitize=address %s -o %t
// CHECK: AddressSanitizer:Use-after-scope

volatile int *p = 0;

int main() {
  {
    int x = 0;
    p = &x;
  }
  *p = 5;
  return 0;
}