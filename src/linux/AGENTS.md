# AGENTS.md - Linux System Call Wrappers

## Directory Structure

```
src/linux/
├── aarch64/             # ARM 64-bit architecture implementations
├── arm/                 # ARM 32-bit architecture implementations
├── x86_64/              # AMD64/Intel 64-bit implementations
├── x32/                 # 32-bit ABI on 64-bit systems
├── liteos_a/            # Huawei LiteOS-specific implementations
├── syscall_hooks/       # System call hooking mechanism
├── linux/               # Additional Linux-specific implementations
├── adjtime.c            # Time adjustment system calls
├── adjtimex.c           # Fine-grained time adjustment
├── arch_prctl.c         # Architecture-specific process control
├── brk.c                # Data segment breakpoint
├── cache.c              # Cache operations
├── cap.c                # Capability operations
├── chroot.c             # Change root directory
├── clock_adjtime.c      # Clock adjustment
├── clone.c              # Create child process with flags
├── copy_file_range.c    # Copy data between file descriptors
├── epoll.c              # I/O event notification facility
├── eventfd.c            # File descriptor for event notification
├── fallocate.c          # Preallocate or deallocate file space
├── fanotify.c           # File system event monitoring
├── flock.c              # File locking
├── getdents.c           # Get directory entries
├── getrandom.c          # Get random numbers
├── gettid.c             # Get thread ID
├── inotify.c            # File system event monitoring
├── ioperm.c             # I/O port permissions (x86)
├── iopl.c               # I/O privilege level (x86)
├── klogctl.c            # Kernel log control
├── membarrier.c         # Memory barrier expedited
├── memfd_create.c       # Create anonymous file
├── mlock2.c             # Lock memory in RAM
├── module.c             # Load/unload kernel modules
├── mount.c              # Mount filesystem
├── name_to_handle_at.c  # Get file handle
├── open_by_handle_at.c  # Open file by handle
├── personality.c        # Process execution domain
├── pivot_root.c         # Change root filesystem
├── prctl.c              # Process control operations
├── preadv2.c            # Read from multiple buffers (new)
├── prlimit.c            # Process resource limits
├── process_vm.c         # Process memory operations
├── ptrace.c             # Process tracing
├── pwritev2.c           # Write to multiple buffers (new)
├── quotactl.c           # Disk quota control
├── readahead.c          # Read ahead on file
├── reboot.c             # Reboot system
├── remap_file_pages.c   # Remap pages in file
├── sbrk.c               # Data segment increment
├── sendfile.c           # Transfer data between files
├── setfsgid.c           # Set filesystem group ID
├── setfsuid.c           # Set filesystem user ID
├── setgroups.c          # Set group list
├── sethostname.c        # Set hostname
├── setns.c              # Set namespace
├── settimeofday.c       # Set time of day
├── signalfd.c           # File descriptor for signals
├── splice.c             # Splice data to/from pipe
├── statx.c              # Extended file status
├── stime.c              # Set time
├── swap.c               # Swap management
├── sync_file_range.c    # Sync file region
├── syncfs.c             # Sync filesystem
├── sysinfo.c            # System information
├── tee.c                # Duplicate pipe content
├── tgkill.c             # Send signal to thread group
├── timerfd.c            # File descriptor for timers
├── unshare.c            # Disassociate parts of execution context
├── utimes.c             # File timestamps
├── vhangup.c            # Virtually hangup terminal
├── vmsplice.c           # Splice user pages to/from pipe
├── wait3.c              # Wait for process termination (BSD)
├── wait4.c              # Wait for process termination
└── xattr.c              # Extended attributes
```

---

## 1. Purpose

This directory contains **Linux-specific system call wrapper implementations** for the musl libc library.

### 1.1 What This Directory Provides

- **System call wrappers**: Direct interface to Linux kernel system calls
- **Platform abstraction**: Consistent API across different architectures
- **POSIX compatibility**: Linux-specific implementations of POSIX interfaces
- **Extended functionality**: Linux-specific features not in POSIX

### 1.2 Relationship to musl

- Part of the musl libc source tree
- Contains Linux-specific code separated from portable code
- Most files are only compiled on Linux targets
- Architecture-specific implementations in subdirectories

---

## 2. System Call Categories

### 2.1 Process Management

| File | System Call | Description |
|------|-------------|-------------|
| `clone.c` | `clone()` | Create child process with configurable execution context |
| `wait4.c` | `wait4()` | Wait for process termination (BSD-style) |
| `wait3.c` | `wait3()` | Wait for process termination (older) |
| `ptrace.c` | `ptrace()` | Process tracing and debugging |
| `prctl.c` | `prctl()` | Process control operations |
| `personality.c` | `personality()` | Set process execution domain |
| `setns.c` | `setns()` | Set namespace association |
| `unshare.c` | `unshare()` | Disassociate parts of execution context |

### 2.2 Memory Management

| File | System Call | Description |
|------|-------------|-------------|
| `brk.c` | `brk()` | Change data segment location |
| `sbrk.c` | `sbrk()` | Increment data segment size |
| `mlock2.c` | `mlock2()` | Lock memory in RAM with flags |
| `membarrier.c` | `membarrier()` | Memory barrier expedited |
| `memfd_create.c` | `memfd_create()` | Create anonymous memory file |
| `remap_file_pages.c` | `remap_file_pages()` | Remap pages in file |
| `cache.c` | Cache operations | Cache flushing and synchronization |

### 2.3 File Operations

| File | System Call | Description |
|------|-------------|-------------|
| `copy_file_range.c` | `copy_file_range()` | Copy data between file descriptors |
| `sendfile.c` | `sendfile()` | Transfer data between file descriptors |
| `fallocate.c` | `fallocate()` | Preallocate or deallocate file space |
| `flock.c` | `flock()` | Apply or remove advisory lock on file |
| `sync_file_range.c` | `sync_file_range()` | Sync file region to disk |
| `syncfs.c` | `syncfs()` | Sync filesystem to disk |
| `splice.c` | `splice()` | Splice data to/from pipe |
| `tee.c` | `tee()` | Duplicate pipe content |
| `vmsplice.c` | `vmsplice()` | Splice user pages to/from pipe |

### 2.4 I/O Event Notification

| File | System Call | Description |
|------|-------------|-------------|
| `epoll.c` | `epoll_*()` | I/O event notification facility |
| `eventfd.c` | `eventfd()` | File descriptor for event notification |
| `signalfd.c` | `signalfd()` | File descriptor for signals |
| `timerfd.c` | `timerfd()` | File descriptor for timers |
| `inotify.c` | `inotify_*()` | File system event monitoring |
| `fanotify.c` | `fanotify_*()` | File system event monitoring (newer) |

### 2.5 Time and Clocks

| File | System Call | Description |
|------|-------------|-------------|
| `adjtime.c` | `adjtime()` | Smoothly adjust system time |
| `adjtimex.c` | `adjtimex()` | Fine-grained time adjustment |
| `clock_adjtime.c` | `clock_adjtime()` | Adjust clock |
| `settimeofday.c` | `settimeofday()` | Set time of day |
| `stime.c` | `stime()` | Set time (older) |

### 2.6 System Information

| File | System Call | Description |
|------|-------------|-------------|
| `sysinfo.c` | `sysinfo()` | System information |
| `statx.c` | `statx()` | Extended file status |
| `uname.c` | `uname()` | System information (not in list, but exists) |

### 2.7 Network and IPC

| File | System Call | Description |
|------|-------------|-------------|
| `quotactl.c` | `quotactl()` | Disk quota control |
| `getrandom.c` | `getrandom()` | Get random numbers from kernel |

### 2.8 Privileged Operations

| File | System Call | Description |
|------|-------------|-------------|
| `cap.c` | `capget()/capset()` | Capability operations |
| `setfsuid.c` | `setfsuid()` | Set filesystem user ID |
| `setfsgid.c` | `setfsgid()` | Set filesystem group ID |
| `setgroups.c` | `setgroups()` | Set group list |
| `ioperm.c` | `ioperm()` | I/O port permissions (x86) |
| `iopl.c` | `iopl()` | I/O privilege level (x86) |

### 2.9 Filesystem Operations

| File | System Call | Description |
|------|-------------|-------------|
| `mount.c` | `mount()` | Mount filesystem |
| `pivot_root.c` | `pivot_root()` | Change root filesystem |
| `chroot.c` | `chroot()` | Change root directory |
| `swap.c` | `swapon()/swapoff()` | Swap management |

### 2.10 Extended Attributes

| File | System Call | Description |
|------|-------------|-------------|
| `xattr.c` | `*xattr()` | Extended attributes operations |
| `getdents.c` | `getdents()/getdents64()` | Get directory entries |

---

## 3. Architecture-Specific Directories

### 3.1 Overview

| Directory | Architecture | Description |
|-----------|--------------|-------------|
| `aarch64/` | ARM 64-bit | ARMv8-A and later implementations |
| `arm/` | ARM 32-bit | ARMv7 and earlier implementations |
| `x86_64/` | AMD64/Intel 64 | x86_64 64-bit implementations |
| `x32/` | x32 ABI | 32-bit ABI on x86_64 systems |

---

## 4. Thread Safety Considerations

### 4.1 Signal Safety

System call wrappers must be async-signal-safe when the underlying syscall is:
- No use of malloc or free
- No use of non-reentrant library functions
- Minimal stack usage

### 4.2 Cancellation Points

Some system calls are pthread cancellation points:
- `read()`, `write()`, etc.
- Implemented with `__syscall_cp()` wrapper
- Thread cancellation checked during blocking calls

---

## 5. Related Resources

- [Linux System Call Table](https://filippo.io/linux-syscall-table/)
- [Linux man pages](https://man7.org/linux/man-pages/)
- [Linux Kernel Documentation](https://www.kernel.org/doc/html/latest/)
