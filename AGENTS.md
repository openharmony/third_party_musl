# AGENTS.md

## Project Overview

**Name:** musl libc (OpenHarmony integration)

**Purpose:** C standard library implementation providing efficient static/dynamic linking, lightweight code, and standards conformance (ISO C99 + POSIX 2008 base) for Linux and OpenHarmony systems.

**Primary Language:** C (with assembly for architecture-specific code)

---


## Directory Structure

```
third_party_musl/
├── arch/                  # Architecture-specific implementations (aarch64, arm, x86_64, riscv, mips, etc.)
├── compat/                # Compatibility layer (time32 for 32-bit time_t systems)
├── config/                # Namespace configuration files
├── crt/                   # C runtime startup code (crt1, scrt1, rcrt1)
├── docs/
├── include/               # Public headers (sys/, arpa/, netinet/, bits/)
├── ldso/                  # Dynamic linker implementation
│   ├── dlstart.c          # Stage 1 bootstrap
│   ├── dynlink.c          # Standard musl dynamic linker
│   └── linux/             # OpenHarmony extensions (namespace, CFI, randomization, ADLT)
├── libc-test/             # Test suite
├── libc_unittest/         # Unit tests
├── porting/               # Platform adaptation (linux, liteos_a, liteos_m, uniproton)
├── scripts/
├── src/                   # Main implementation source
│   ├── aio/               # Asynchronous I/O
│   ├── complex/
│   ├── conf/
│   ├── crypt/
│   ├── ctype/
│   ├── dfx/               # Debugability features
│   ├── dirent/
│   ├── env/
│   ├── errno/
│   ├── exit/
│   ├── fcntl/
│   ├── fdsan/             # File descriptor sanitizer
│   ├── fenv/
│   ├── fortify/           # Buffer overflow protection
│   ├── gwp_asan/          # GWP-ASan memory safety
│   ├── hilog/             # HiLog integration
│   ├── hook/              # OpenHarmony hooking (RESTRICT)
│   ├── include/
│   ├── info/              # Device/system info
│   ├── internal/          # Internal APIs and shared code
│   ├── ipc/
│   ├── ldso/              # Dynamic linker internals
│   ├── legacy/            # Legacy compatibility code
│   ├── linux/
│   ├── locale/            # Locale support (via ICU)
│   ├── malloc/            # Memory allocators (mallocng, jemalloc)
│   ├── math/
│   ├── misc/
│   ├── mman/
│   ├── mq/
│   ├── multibyte/
│   ├── network/
│   ├── passwd/
│   ├── prng/
│   ├── process/
│   ├── regex/
│   ├── sched/
│   ├── search/
│   ├── select/
│   ├── setjmp/
│   ├── sigchain/          # Signal chaining
│   ├── signal/
│   ├── stat/
│   ├── stdio/
│   ├── stdlib/
│   ├── string/
│   ├── syscall_hooks/     # Syscall hooks
│   ├── temp/
│   ├── termios/
│   ├── thread/
│   ├── time/
│   ├── trace/             # Tracing/trace markers
│   ├── unistd/
│   └── version/
├── third_party/           # Third-party code (openbsd)
└── tools/                 # Build utilities (CFI scripts, API comparison tool, compiler wrappers)
```

## Build Commands

### OpenHarmony Build
```bash
# Build libc for OpenHarmony (requires OpenHarmony build environment)(https://gitcode.com/openharmony/docs/tree/master/zh-cn)
./build.sh --product-name <product> --build-target musl_all

# Output directories for musl_all:
#   Build intermediates: out/<product>/obj/third_party/musl/
#   Libraries: out/<product>/lib.unstripped/obj/third_party/musl/user/lib/arm-linux-ohos/libc.so

```

## Testing

### Test Build
```bash
# Build libc-test for OpenHarmony (requires OpenHarmony build environment)(https://gitcode.com/openharmony/docs/tree/master/zh-cn)
./build.sh --product-name <product> --build-target libctest

# Output directories for libctest:
#   Executables: out/<product>/musl/libc-test
#   Libraries: out/<product>/musl/libc-test-lib

```

### On-Device Testing (OpenHarmony)
For automated testing, use the Windows batch script from `scripts/runtest_Windows.bat` after configuring the paths.

For manual testing:
```bash
# Transfer runtest and test binary to device, then run:
./runtest -w '' <test_name>

# Example:
./runtest -w '' memmem
```
For detailed instructions, see [libc-test/README-HowtoRun.md](libc-test/README-HowtoRun.md)

### Test Organization
- `libc-test/src/functional/` - Functional tests
- `libc-test/src/regression/` - Regression tests
- `libc-test/src/math/` - Math library tests
- `libc-test/src/functionalext/` - OpenHarmony-specific extension tests
- `libc_unittest/` - Additional unit tests
- `fuzztest/` - Fuzzing tests


## OpenHarmony Integration Features

### CFI (Control Flow Integrity)
- Indirect call validation via `icall_item` structures
- Modifier ranges for valid targets

### ADLT (Advanced Dynamic Linking Technology)
- Partial library loading from combined files
- Hugepage support
- GOT entry tracking for lazy relocations

### Namespace Isolation
- Configuration: `/etc/ld-musl-namespace-*.ini`
- API: `dlns_init()`, `dlns_create()`, `dlopen_ns()` (dlfcn.h)
- Separates DSO loading contexts to prevent symbol conflicts
- See [ldso/linux/README.md](ldso/linux/README.md)

### Other Features
- Multiple kernel support: LiteOS-a, LiteOS-m, UniProton
- GWP-ASan for memory safety
