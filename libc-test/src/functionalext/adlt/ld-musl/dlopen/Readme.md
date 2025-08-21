# dlopen test

## Task

ensure that dlopen/dlsym  works with adlt

## Test list

Currently implemented:

- test_direct - just check that shared library works (no dlopen yet).
- test_dlopen - The dlopen()/dlsym() used to access shared library.
- test_doublemap - application uses both shared library mapped by ld.so,
and opened with dlopen(). Results are compared, including the function
address of directly mapped and dlopen()'ed library

_If environment variable VERBOSE_EXPECT is set or `-v` is supplied as a first argument to any test, then results of successfull checks are printed, otherwise, only failures are reported._


