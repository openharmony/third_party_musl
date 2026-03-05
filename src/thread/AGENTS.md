# AGENTS.md - Threading Implementation

## Directory Structure

```
src/thread/
├── pthread_*.c              # POSIX threads API implementation
├── pthread_mutex_*.c        # Mutex operations
├── pthread_cond_*.c         # Condition variable operations
├── pthread_rwlock_*.c       # Read-write lock operations
├── pthread_barrier_*.c      # Barrier synchronization
├── pthread_spin_*.c         # Spin lock operations
├── pthread_attr_*.c         # Thread attribute management
├── pthread_key_*.c          # Thread-specific data keys
├── sem_*.c                  # Semaphore operations
├── cnd_*.c                  # C11 threads condition variables
├── mtx_*.c                  # C11 threads mutexes
├── thrd_*.c                 # C11 threads API
├── tss_*.c                  # C11 thread-specific storage
├── call_once.c              # C11 one-time initialization
├── clone.c                  # Thread creation via clone syscall
├── __lock.c                 # Internal locking primitives
├── __wait.c                 # Wait operations
├── __timedwait.c            # Timed wait operations
├── __syscall_cp.c           # Cancellation-point syscalls
├── synccall.c               # Synchronized call across threads
├── vmlock.c                 # Virtual memory locking
├── lock_ptc.c               # Process thread list locking
├── pthread_atfork.c         # Fork handlers
├── pthread_cancel.c         # Thread cancellation
├── pthread_setcancel*.c      # Cancellation state management
├── aarch64/                 # ARM64-specific assembly
├── arm/                    # ARM-specific assembly
├── x86_64/                 # x86_64-specific assembly
├── i386/                   # x86-specific assembly
├── mips/                   # MIPS-specific assembly
├── mips64/                 # MIPS64-specific assembly
├── powerpc/                # PowerPC-specific assembly
├── powerpc64/              # PowerPC64-specific assembly
├── riscv64/                # RISC-V 64-bit assembly
├── s390x/                  # s390x-specific assembly
├── loongarch64/            # LoongArch64-specific assembly
├── microblaze/             # MicroBlaze-specific assembly
├── m68k/                   # m68k-specific assembly
├── or1k/                   # OpenRISC-specific assembly
└── sh/                     # SuperH-specific assembly
```

---

## 1. Build Commands

### Building the Project
```bash
# Configure for target architecture
./configure --target=aarch64-linux-musl --prefix=/usr/local/musl

# Build all components
make

# Clean build artifacts
make clean
make distclean
```

### Testing Commands
```bash
# Build libc-test suite
cd libc-test
make -f Makefile config.mak
make -f Makefile

# Run specific threading tests
./src/common/runtest.exe -w '' pthread_create
./src/common/runtest.exe -w '' pthread_mutex_lock
./src/common/runtest.exe -w '' pthread_cond_wait
./src/common/runtest.exe -w '' pthread_rwlock

# Run C11 threads tests
./src/common/runtest.exe -w '' thrd_create
./src/common/runtest.exe -w '' mtx_lock
./src/common/runtest.exe -w '' cnd_wait
```

### Building Single Object
```bash
# Rebuild specific thread object file
make obj/src/thread/pthread_create.o
make obj/src/thread/pthread_mutex_lock.o
make obj/src/thread/pthread_cond_timedwait.o
```

---

## 2. Threading API Categories

### 2.1 Thread Management

| File | Function | Description |
|------|----------|-------------|
| `pthread_create.c` | `pthread_create()` | Create new thread |
| `pthread_detach.c` | `pthread_detach()` | Detach thread |
| `pthread_join.c` | `pthread_join()` | Wait for thread termination |
| `pthread_exit.c` | `pthread_exit()` | Terminate calling thread |
| `pthread_self.c` | `pthread_self()` | Get thread ID |
| `pthread_equal.c` | `pthread_equal()` | Compare thread IDs |
| `pthread_kill.c` | `pthread_kill()` | Send signal to thread |

### 2.2 Mutex Operations

| File | Function | Description |
|------|----------|-------------|
| `pthread_mutex_init.c` | `pthread_mutex_init()` | Initialize mutex |
| `pthread_mutex_lock.c` | `pthread_mutex_lock()` | Lock mutex |
| `pthread_mutex_trylock.c` | `pthread_mutex_trylock()` | Try to lock mutex |
| `pthread_mutex_unlock.c` | `pthread_mutex_unlock()` | Unlock mutex |
| `pthread_mutex_timedlock.c` | `pthread_mutex_timedlock()` | Lock with timeout |
| `pthread_mutex_destroy.c` | `pthread_mutex_destroy()` | Destroy mutex |

### 2.3 Condition Variables

| File | Function | Description |
|------|----------|-------------|
| `pthread_cond_init.c` | `pthread_cond_init()` | Initialize condition variable |
| `pthread_cond_wait.c` | `pthread_cond_wait()` | Wait on condition |
| `pthread_cond_signal.c` | `pthread_cond_signal()` | Signal one thread |
| `pthread_cond_broadcast.c` | `pthread_cond_broadcast()` | Signal all threads |
| `pthread_cond_timedwait.c` | `pthread_cond_timedwait()` | Wait with timeout |

### 2.4 Read-Write Locks

| File | Function | Description |
|------|----------|-------------|
| `pthread_rwlock_init.c` | `pthread_rwlock_init()` | Initialize rwlock |
| `pthread_rwlock_rdlock.c` | `pthread_rwlock_rdlock()` | Acquire read lock |
| `pthread_rwlock_wrlock.c` | `pthread_rwlock_wrlock()` | Acquire write lock |
| `pthread_rwlock_unlock.c` | `pthread_rwlock_unlock()` | Release rwlock |

### 2.5 Thread-Specific Data

| File | Function | Description |
|------|----------|-------------|
| `pthread_key_create.c` | `pthread_key_create()` | Create TSD key |
| `pthread_setspecific.c` | `pthread_setspecific()` | Set TSD value |
| `pthread_getspecific.c` | `pthread_getspecific()` | Get TSD value |

### 2.6 C11 Threads

| File | Function | Description |
|------|----------|-------------|
| `thrd_create.c` | `thrd_create()` | Create C11 thread |
| `thrd_join.c` | `thrd_join()` | Join C11 thread |
| `thrd_exit.c` | `thrd_exit()` | Exit C11 thread |
| `thrd_sleep.c` | `thrd_sleep()` | Sleep for duration |
| `thrd_yield.c` | `thrd_yield()` | Yield processor |
| `mtx_*.c` | `mtx_*()` | C11 mutex operations |
| `cnd_*.c` | `cnd_*()` | C11 condition variables |
| `tss_*.c` | `tss_*()` | C11 thread-specific storage |
| `call_once.c` | `call_once()` | One-time initialization |

---

## 3. Code Style Guidelines

### File Structure and Headers
- Apache 2.0 license header in all source files
- Include `"pthread_impl.h"` for thread implementation details
- Include `"lock.h"` for internal locking primitives
- Include `"libc.h"` for libc internals

### Imports and Includes
- System headers first: `#include <pthread.h>`, `#include <sched.h>`, etc.
- Internal headers second: `#include "pthread_impl.h"`, `#include "lock.h"`, etc.
- Alphabetical order within groups

### Naming Conventions
- **Functions**: `pthread_*` for POSIX, `thrd_*`/`mtx_*`/`cnd_*` for C11
- **Internal functions**: `__pthread_*` for internal implementations
- **Variables**: `snake_case` for local variables
- **Struct members**: Short names like `tid`, `next`, `result`

### Type Usage
- Use `pthread_t` for thread identifiers
- Use `pthread_mutex_t` for mutexes
- Use `pthread_cond_t` for condition variables
- Use `pthread_key_t` for TSD keys
- Use `int` for return values (0 on success, error number on failure)

### Error Handling
- Return 0 on success, error number on failure
- Set `errno` when appropriate
- Check return values of system calls
- Use `a_cas()` and `a_store()` for atomic operations

### Thread Safety
- All thread functions must be thread-safe
- Use `LOCK()` and `UNLOCK()` macros for internal locks
- Use `__wait()` and `__wake()` for futex-based waiting
- Handle cancellation points with `__syscall_cp()`

### Cancellation Handling
- Cancellation state managed via `canceldisable` and `cancelasync`
- Cleanup handlers executed via `cancelbuf` list
- Use `pthread_setcancelstate()` to manage cancellation
- Use `pthread_testcancel()` to create cancellation points

### Architecture-Specific Code
- Assembly primitives in architecture-specific subdirectories
- Context switch and thread creation in assembly
- TLS (Thread Local Storage) setup in architecture code

---

## 4. Key Implementation Details

### 4.1 Thread Creation
- Uses `clone()` system call with `CLONE_VM | CLONE_FS | CLONE_FILES` flags
- Stack allocation via `mmap()` with guard pages
- TLS setup during thread initialization
- Atfork handlers registered with `pthread_atfork()`

### 4.2 Mutex Implementation
- Fast path: atomic CAS on lock word
- Slow path: futex-based waiting via `__wait()`
- Priority inheritance support via protocol attribute
- Robust mutexes for process-shared mutexes

### 4.3 Condition Variables
- Uses futex for waiting and signaling
- Supports CLOCK_REALTIME and CLOCK_MONOTONIC
- OpenHarmony extensions: `pthread_cond_timeout_np()`
- Proper handling of spurious wakeups

### 4.4 Thread Cancellation
- Asynchronous and deferred cancellation modes
- Cleanup handlers executed in LIFO order
- Cancellation points at blocking syscalls
- Signal-based cancellation for async mode

---

## 5. Related Resources

- [POSIX Threads Specification](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
- [C11 Threads Specification](https://en.cppreference.com/w/c/thread)
- [musl Threading Design](http://www.musl-libc.org/)
