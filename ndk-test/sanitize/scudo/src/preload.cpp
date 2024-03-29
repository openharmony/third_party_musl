// Test that the preloaded runtime works without linking the static library.

// RUN: %clang %s -lstdc++ -o %t
// RUN: env LD_PRELOAD=%shared_libscudo    not %run %t 2>&1 | FileCheck %s
// RUN: env LD_PRELOAD=%shared_minlibscudo not %run %t 2>&1 | FileCheck %s

#include <assert.h>

int main(int argc, char *argv[]) {
  int *p = new int;
  assert(p);
  *p = 0;
  delete p;
  delete p;
  return 0;
}

// CHECK: ERROR: invalid chunk state
