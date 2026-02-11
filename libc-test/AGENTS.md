# AGENTS.md - musl libc-test

## Directory Structure

```
libc-test/
├── src/
│   ├── api/                 # Interface tests and header compatibility
│   ├── functional/          # Core functional tests (~150 tests)
│   ├── functionalext/       # Extended functional tests (~1,100 tests)
│   ├── regression/          # Regression tests for historical bugs
│   ├── math/                # Mathematical function tests
│   ├── common/              # Shared utilities and test framework
│   ├── eabi/                # EABI-specific tests
│   ├── gwp_asan/            # GWP-ASan memory safety tests
│   ├── musl/                # Musl-specific tests
│   ├── nativehook/          # Native hook tests
│   ├── ola_test/            # OLA (OpenHarmony LA) tests
│   └── static_link/         # Static linking tests
├── Makefile                 # Primary build system
├── config.mak.def           # Default configuration template
├── config.mak               # Actual configuration (generated)
└── test_template.gni        # GN build integration
```

---

## 1. Test Categories

### 1.1 API Tests (`src/api/`)

**Purpose**: Interface testing and build-time header validation

**Key Areas**:
- Header inclusion compatibility
- Type definitions and constants
- API signature validation
- Symbol visibility checks

**Location**: [`src/api/`](src/api/)

### 1.2 Functional Tests (`src/functional/`)

**Purpose**: Core libc functionality testing (~150 tests)

**Key Test Areas**:
- Threading and synchronization
- Memory allocation (malloc, calloc, free, etc.)
- File operations (open, read, write, close)
- String operations
- Time and date functions
- Signal handling
- Socket operations

**Special Features**:
- Both dynamic and static linking variants
- TLS (Thread Local Storage) tests
- dlopen functionality tests

**Location**: [`src/functional/`](src/functional/)

### 1.3 Extended Functional Tests (`src/functionalext/`)

**Purpose**: Comprehensive testing with ~1,100 tests - the largest category

#### 1.3.1 ADLT (Advanced Dynamic Linking Tests)

**Test Areas**:
- Relocation testing (GOT, PLT, TLS, REL, RELA)
- Versioning and symbol handling
- Cross-DSO (Dynamic Shared Object) tests
- Security features (CFI, HWASAN, UBSAN, TSAN)
- TLS testing
- ASAN/HWASAN/TSAN/UBSAN sanitizer tests

**Subdirectories**:
- `base/` - Basic dynamic linking tests
- `tls/` - Thread Local Storage tests
- `cfi/` - Control Flow Integrity tests
- `asan/` - AddressSanitizer tests
- `hwasan/` - HWAddressSanitizer tests
- `tsan/` - ThreadSanitizer tests
- `ubsan/` - UndefinedBehaviorSanitizer tests

**Location**: [`src/functionalext/adlt/`](src/functionalext/adlt/)

#### 1.3.2 Supplement Tests

Per POSIX/SUS category directories:
- `ctype/` - Character classification
- `dirent/` - Directory operations
- `errno/` - Error handling
- `fcntl/` - File control
- `math/` - Mathematical functions
- `pthread/` - Threading
- `stdio/` - Standard I/O
- `stdlib/` - Standard library
- `string/` - String operations
- `time/` - Time functions
- `unistd/` - UNIX standard functions

**Testing Framework**: GTest-based

**Location**: [`src/functionalext/supplement/`](src/functionalext/supplement/)

#### 1.3.3 OpenHarmony-Specific Extensions

**Test Areas**:
- `hilog/` - HiLog integration tests
- `ohos_dfx_log/` - DFX (Diagnostics and Failure Management) logging
- `ldso_cfi/` - Control Flow Integrity tests
- `ldso_randomization/` - Load order randomization tests
- `dlns/` - Dynamic linker namespace tests
- `ldso_debug/` - Linker debug functionality
- `sigchain/` - Signal chain tests
- `fortify/` - Fortify buffer overflow protection
- `relro/` - RELRO (Relocation Read-Only) tests
- `symver/` - Symbol versioning tests
- `rpath/` - RPATH/RUNPATH tests
- `backtrace/` - Stack backtrace tests
- `tgkill/` - tgkill system call tests

**Location**: [`src/functionalext/`](src/functionalext/)

### 1.4 Regression Tests (`src/regression/`)

**Purpose**: Tests for historical bugs and edge cases

**Features**:
- References to musl commit hashes
- Links to glibc bug tracker
- Race condition tests
- Performance regression tests

**Location**: [`src/regression/`](src/regression/)

### 1.5 Math Tests (`src/math/`)

**Purpose**: Mathematical function correctness testing

**Test Areas**:
- Generated test vectors
- Multiple precision testing (ld128)
- CRLibM integration for correctly rounded math
- Special function testing

**Location**: [`src/math/`](src/math/)

### 1.6 Common Test Framework (`src/common/`)

**Purpose**: Shared utilities and test execution framework

**Key Components**:
- `runtest` - Test runner with timeout protection
- Test harness utilities
- Error reporting functions
- Build helpers

**Location**: [`src/common/`](src/common/)

---

## 2. Adding New Tests

### 2.1 Adding a Basic Test

1. Create a `.c` file in the appropriate category directory
2. Implement `main()` function that returns 0 on success
3. Use `t_error()` for error reporting
4. Build and test

**Example Template**:
```c
#include "test.h"

int main(void)
{
    // Test code here
    if (some_condition_fails) {
        t_error("Test failed: reason\n");
        return 1;
    }
    return 0;
}
```

---

## 3. Related Resources

- [musl official documentation](http://www.musl-libc.org/)
- [POSIX specification](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [OpenHarmony documentation](https://docs.openharmony.cn/)