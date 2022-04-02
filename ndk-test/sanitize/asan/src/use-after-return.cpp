// BUILD: clang -fsanitize=address %s -o %t
// ASAN_OPTIONS=detect_stack_use_after_return=true %t
// CHECK: AddressSanitizer:Use-after-return

int *ptr;

void FunctionThatEscapesLocalObject() {
  int local[100];
  ptr = &local[0];
}

int main(int argc, char **argv) {
  FunctionThatEscapesLocalObject();
  return ptr[argc];
}