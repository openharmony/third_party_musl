# AGENTS.md - String and Memory Operations

## Directory Structure

```
src/string/
├── strcmp.c                # String comparison
├── strncmp.c               # String comparison with limit
├── strcasecmp.c            # Case-insensitive comparison
├── strncasecmp.c           # Case-insensitive comparison with limit
├── strcpy.c                # String copy
├── strncpy.c               # String copy with limit
├── stpcpy.c                # String copy with return pointer
├── stpncpy.c               # String copy with limit and return
├── strcat.c                # String concatenation
├── strncat.c               # String concatenation with limit
├── strlcpy.c               # Safe string copy (BSD)
├── strlcat.c               # Safe string concatenation (BSD)
├── strlen.c                # String length
├── strnlen.c               # String length with limit
├── strchr.c                # Find character in string
├── strrchr.c               # Find character from end
├── strchrnul.c             # Find character or null terminator
├── strpbrk.c               # Find first of any characters
├── strstr.c                # Find substring
├── strcasestr.c            # Case-insensitive substring search
├── strcspn.c               # Count initial characters not in set
├── strspn.c                # Count initial characters in set
├── strtok.c                # Tokenize string (not thread-safe)
├── strtok_r.c              # Tokenize string (reentrant)
├── strsep.c                # Separate string (BSD)
├── strdup.c                # Duplicate string
├── strndup.c               # Duplicate string with limit
├── strerror_r.c             # Error number to string (reentrant)
├── strsignal.c             # Signal number to string
├── strverscmp.c            # Version string comparison
├── memcpy.c                # Memory copy
├── memmove.c               # Memory copy with overlap handling
├── memccpy.c               # Memory copy until character
├── mempcpy.c               # Memory copy with return pointer
├── memcmp.c                # Memory comparison
├── memchr.c                # Find character in memory
├── memrchr.c               # Find character from end
├── rawmemchr.c             # Find character (no length check)
├── memset.c                # Set memory to value
├── memmem.c                # Find memory in memory
├── bcopy.c                 # Memory copy (BSD)
├── bcmp.c                  # Memory comparison (BSD)
├── bzero.c                 # Zero memory (BSD)
├── explicit_bzero.c         # Secure zero memory
├── index.c                 # Find character (BSD)
├── rindex.c                # Find character from end (BSD)
├── swab.c                  # Swap bytes
├── wcscpy.c                # Wide string copy
├── wcscat.c                # Wide string concatenation
├── wcschr.c                # Find wide character
├── wcsrchr.c               # Find wide character from end
├── wcscmp.c                # Wide string comparison
├── wcscasecmp.c            # Case-insensitive wide comparison
├── wcscasecmp_l.c          # Locale-aware case-insensitive comparison
├── wcslen.c                # Wide string length
├── wcsncmp.c               # Wide string comparison with limit
├── wcsncasecmp.c           # Case-insensitive wide comparison with limit
├── wcsncasecmp_l.c         # Locale-aware case-insensitive with limit
├── wcsncat.c               # Wide string concatenation with limit
├── wcsncpy.c               # Wide string copy with limit
├── wcsnlen.c               # Wide string length with limit
├── wcscspn.c               # Count wide characters not in set
├── wcsspn.c                # Count wide characters in set
├── wcspbrk.c               # Find first of any wide characters
├── wcsstr.c                # Find wide substring
├── wcstok.c                # Tokenize wide string
├── wcswcs.c                # Find wide substring (alias)
├── wcpcpy.c                # Wide string copy with return pointer
├── wcpncpy.c               # Wide string copy with limit and return
├── wcsdup.c                # Duplicate wide string
├── wmemchr.c               # Find wide character in memory
├── wmemcmp.c               # Wide memory comparison
├── wmemcpy.c               # Wide memory copy
├── wmemmove.c              # Wide memory copy with overlap
├── wmemset.c               # Set wide memory to value
├── aarch64/                # ARM64-optimized assembly
├── arm/                    # ARM-optimized assembly
├x86_64/                   # x86_64-optimized assembly
└── i386/                   # x86-optimized assembly
```

---

## 1. Build Commands

### Building Project
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

# Run specific string tests
./src/common/runtest.exe -w '' strcmp
./src/common/runtest.exe -w '' strcpy
./src/common/runtest.exe -w '' strlen
./src/common/runtest.exe -w '' memcpy
./src/common/runtest.exe -w '' strstr
```

### Building Single Object
```bash
# Rebuild specific string object file
make obj/src/string/strcmp.o
make obj/src/string/strlen.o
make obj/src/string/memcpy.o
```

---

## 2. String Functions

### 2.1 String Comparison

| File | Function | Description |
|------|----------|-------------|
| `strcmp.c` | `strcmp()` | Compare strings |
| `strncmp.c` | `strncmp()` | Compare strings with limit |
| `strcasecmp.c` | `strcasecmp()` | Case-insensitive compare |
| `strncasecmp.c` | `strncase()` | Case-insensitive compare with limit |

### 2.2 String Copy

| File | Function | Description |
|------|----------|-------------|
| `strcpy.c` | `strcpy()` | Copy string |
| `strncpy.c` | `strncpy()` | Copy string with limit |
| `stpcpy.c` | `stpcpy()` | Copy string, return end |
| `stpncpy.c` | `stpncpy()` | Copy string with limit, return end |
| `strlcpy.c` | `strlcpy()` | Safe copy (BSD) |

### 2.3 String Concatenation

| File | Function | Description |
|------|----------|-------------|
| `strcat.c` | `strcat()` | Concatenate strings |
| `strncat.c` | `strncat()` | Concatenate with limit |
| `strlcat.c` | `strlcat()` | Safe concatenation (BSD) |

### 2.4 String Length

| File | Function | Description |
|------|----------|-------------|
| `strlen.c` | `strlen()` | String length |
| `strnlen.c` | `strnlen()` | String length with limit |

### 2.5 String Search

| File | Function | Description |
|------|----------|-------------|
| `strchr.c` | `strchr()` | Find character |
| `strrchr.c` | `strrchr()` | Find character from end |
| `strchrnul.c` | `strchrnul()` | Find character or null |
| `strpbrk.c` | `strpbrk()` | Find first of any characters |
| `strstr.c` | `strstr()` | Find substring |
| `strcasestr.c` | `strcasestr()` | Case-insensitive substring |

### 2.6 Character Classification

| File | Function | Description |
|------|----------|-------------|
| `strcspn.c` | `strcspn()` | Count chars not in set |
| `strspn.c` | `strspn()` | Count chars in set |

### 2.7 String Tokenization

| File | Function | Description |
|------|----------|-------------|
| `strtok.c` | `strtok()` | Tokenize (not thread-safe) |
| `strtok_r.c` | `strtok_r()` | Tokenize (reentrant) |
| `strsep.c` | `strsep()` | Separate string (BSD) |

### 2.8 String Duplication

| File | Function | Description |
|------|----------|-------------|
| `strdup.c` | `strdup()` | Duplicate string |
| `strndup.c` | `strndup()` | Duplicate with limit |

### 2.9 Error/Signal Messages

| File | Function | Description |
|------|----------|-------------|
| `strerror_r.c` | `strerror_r()` | Error to string (reentrant) |
| `strsignal.c` | `strsignal()` | Signal to string |

---

## 3. Memory Functions

### 3.1 Memory Copy

| File | Function | Description |
|------|----------|-------------|
| `memcpy.c` | `memcpy()` | Copy memory |
| `memmove.c` | `memmove()` | Copy with overlap handling |
| `memccpy.c` | `memccpy()` | Copy until character |
| `mempcpy.c` | `mempcpy()` | Copy, return end |
| `bcopy.c` | `bcopy()` | Copy memory (BSD) |

### 3.2 Memory Comparison

| File | Function | Description |
|------|----------|-------------|
| `memcmp.c` | `memcmp()` | Compare memory |
| `bcmp.c` | `bcmp()` | Compare memory (BSD) |

### 3.3 Memory Search

| File | Function | Description |
|------|----------|-------------|
| `memchr.c` | `memchr()` | Find character |
| `memrchr.c` | `memrchr()` | Find character from end |
| `rawmemchr.c` | `rawmemchr()` | Find character (no length check) |
| `memmem.c` | `memmem()` | Find memory in memory |

### 3.4 Memory Set

| File | Function | Description |
|------|----------|-------------|
| `memset.c` | `memset()` | Set memory to value |
| `bzero.c` | `bzero()` | Zero memory (BSD) |
| `explicit_bzero.c` | `explicit_bzero()` | Secure zero memory |

---

## 4. Wide Character Functions

### 4.1 Wide String Operations

| File | Function | Description |
|------|----------|-------------|
| `wcscpy.c` | `wcscpy()` | Copy wide string |
| `wcscat.c` | `wcscat()` | Concatenate wide string |
| `wcschr.c` | `wcschr()` | Find wide character |
| `wcsrchr.c` | `wcsrchr()` | Find wide character from end |
| `wcscmp.c` | `wcscmp()` | Compare wide strings |
| `wcslen.c` | `wcslen()` | Wide string length |
| `wcsncmp.c` | `wcsncmp()` | Compare with limit |
| `wcsncat.c` | `wcsncat()` | Concatenate with limit |
| `wcsncpy.c` | `wcsncpy()` | Copy with limit |
| `wcsnlen.c` | `wcsnlen()` | Length with limit |
| `wcscspn.c` | `wcscspn()` | Count chars not in set |
| `wcsspn.c` | `wcsspn()` | Count chars in set |
| `wcspbrk.c` | `wcspbrk()` | Find first of any chars |
| `wcsstr.c` | `wcsstr()` | Find wide substring |
| `wcstok.c` | `wcstok()` | Tokenize wide string |
| `wcswcs.c` | `wcswcs()` | Find wide substring (alias) |
| `wcsdup.c` | `wcsdup()` | Duplicate wide string |

### 4.2 Wide Memory Operations

| File | Function | Description |
|------|----------|-------------|
| `wmemchr.c` | `wmemchr()` | Find wide character |
| `wmemcmp.c` | `wmemcmp()` | Compare wide memory |
| `wmemcpy.c` | `wmemcpy()` | Copy wide memory |
| `wmemmove.c` | `wmemmove()` | Copy with overlap |
| `wmemset.c` | `wmemset()` | Set wide memory |

---

## 5. Code Style Guidelines

### File Structure and Headers
- Apache 2.0 license header in all source files
- Include `<string.h>` for string functions
- Include `<stdint.h>` for fixed-width types
- Include `<endian.h>` for byte order macros

### Imports and Includes
- System headers first: `#include <string.h>`, `#include <stdint.h>`, etc.
- Minimal includes for performance-critical code
- No internal headers typically needed

### Naming Conventions
- **Functions**: Standard C names (e.g., `strcmp`, `memcpy`)
- **Internal functions**: Static helpers with descriptive names
- **Variables**: Short names for hot paths (`l`, `r`, `s`, `d`)

### Type Usage
- Use `char *` for strings
- Use `unsigned char *` for byte operations
- Use `size_t` for lengths and counts
- Use `wchar_t *` for wide strings
- Use `int` for comparison results

### Error Handling
- Return pointer to result on success
- Return NULL on failure (for alloc functions)
- Return negative/positive for comparisons
- No errno setting typically

### Performance Optimization
- Use word-sized operations when possible
- Unroll loops for small fixed sizes
- Use `__attribute__((__may_alias__))` for type punning
- Handle alignment for optimal performance
- Architecture-specific assembly in subdirectories

### Memory Safety
- `strncpy()` does not guarantee null termination
- `strlcpy()`/`strlcat()` are safer alternatives (BSD)
- `explicit_bzero()` prevents compiler optimization
- `memmove()` handles overlapping regions correctly

### String Searching Algorithms
- `strstr()` uses Two-Way algorithm for long patterns
- Special cases for 2, 3, 4 byte patterns
- Byte set optimization for `strspn()`/`strcspn()`
- Shift table for efficient searching

---

## 6. Key Implementation Details

### 6.1 String Length (strlen)
- Word-at-a-time scanning with `HASZERO()` macro
- Alignment check before word operations
- Fallback to byte-by-byte for unaligned or short strings

### 6.2 Memory Copy (memcpy)
- Word-sized copies for aligned data
- Handle misalignment with byte copies
- Unroll loops for 16-byte chunks
- Special handling for overlapping regions (use memmove)

### 6.3 String Search (strstr)
- Two-Way algorithm for O(n) complexity
- Specialized functions for 2, 3, 4 byte needles
- Byte set filtering for quick rejection
- Shift table for skipping non-matching positions

### 6.4 Wide Character Support
- Thin wrappers around byte functions for many operations
- Proper handling of multi-byte sequences
- Locale-aware case folding

---

## 7. Related Resources

- [POSIX String Specification](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/string.h.html)
- [C11 String Specification](https://en.cppreference.com/w/c/string/byte)
- [Two-Way String Matching Algorithm](https://en.wikipedia.org/wiki/String_searching_algorithm)
