# AGENTS.md - Dynamic Linker (ldso)

## Directory Structure

```
ldso/
├── dlstart.c            # Stage 1 bootstrap (runs before relocations)
├── dynlink.c            # Standard musl dynamic linker
└── linux/               # OpenHarmony extensions
    ├── README.md        # Linux-specific documentation
    ├── dynlink.c        # Extended dynamic linker
    ├── namespace.c      # Namespace isolation implementation
    ├── namespace.h      # Namespace API definitions
    ├── ns_config.c      # Namespace configuration parser
    ├── ns_config.h      # Configuration API
    ├── cfi.c            # Control Flow Integrity
    ├── cfi.h            # CFI API definitions
    ├── dynlink_rand.c   # Load order randomization
    ├── dynlink_rand.h   # Randomization API
    ├── dynlink_adlt.h   # ADLT (partial loading) headers
    ├── ld_log.c         # Logging infrastructure
    ├── ld_log.h         # Logging macros and utilities
    ├── strops.c         # String operations
    ├── strops.h         # String operations API
    └── zip_archive.h    # ZIP archive support for ADLT
```

---

## 1. Build Configuration

**OpenHarmony build system uses `ldso/linux/dynlink.c` (extended version), not the standard musl version.**

### 1.1 Build Process

1. **GN action** ([`../BUILD.gn`](../BUILD.gn)):
   - Calls `scripts/porting.sh` to prepare source files
   - Defines `musl_src_ldso` with reference path `ldso/dynlink.c`
   - `musl_src_files_ext` defines actual OpenHarmony extension sources

2. **Porting script** ([`../scripts/porting.sh`](../scripts/porting.sh)):
   ```bash
   cp -rfp ${SRC_DIR}/ldso/linux/* ${DST_DIR}/ldso
   ```
   Copies `ldso/linux/*` to `ldso/` in output directory

3. **Output directory structure**:
   ```
   ${target_out_dir}/${musl_ported_dir}/ldso/
   ├── dynlink.c         ← Copied from ldso/linux/dynlink.c
   ├── namespace.c       ← Copied from ldso/linux/namespace.c
   ├── cfi.c             ← Copied from ldso/linux/cfi.c
   ├── dynlink_rand.c    ← Copied from ldso/linux/dynlink_rand.c
   ├── ld_log.c          ← Copied from ldso/linux/ld_log.c
   ├── ns_config.c       ← Copied from ldso/linux/ns_config.c
   └── strops.c          ← Copied from ldso/linux/strops.c
   ```

---

## 2. Content Common to Both OpenHarmony and Standard musl

### 2.1 dlstart.c - Stage 1 Bootstrap Code

**Location**: `ldso/dlstart.c`

**Description**:
- Runs in the first stage of the dynamic linker, before relocations
- Parses the auxiliary vector (auxv) from the stack
- Handles FDPIC vs normal ELF loading differences
- Performs initial relocations
- Calls stage 2 dynamic linker `__dls2`

**Architecture-specific**:
- Uses assembly macros from `crt_arch.h`
- Architecture-specific code handled via conditional compilation

### 2.2 Basic Dynamic Linking Functionality

Both standard musl and OpenHarmony share these core features:

**ELF File Parsing**:
- Parses ELF program headers (PT_DYNAMIC, PT_LOAD)
- No section headers required
- Loads and maps shared libraries into memory

**Symbol Resolution and Relocation**:
- Symbol lookup and binding
- Relocation processing (REL, RELA, RELR)
- Lazy binding support
- TLS (Thread Local Storage) support

**Basic dlopen API**:
- `dlopen()` - Dynamically load shared libraries
- `dlsym()` - Find symbols
- `dlclose()` - Unload shared libraries
- `dlerror()` - Error reporting

### 2.3 Design Philosophy

Both follow musl's design principles:
- **Simplicity**: Minimal dependencies
- **Self-contained**: Single-file implementation (in standard musl)
- **Efficiency**: Fast startup time and low memory footprint

---

## 3. Content Unique to OpenHarmony

### 3.1 Extended Dynamic Linker (linux/dynlink.c)

**Location**: `ldso/linux/dynlink.c`

**Core Extensions**:
- Integrated namespace isolation system
- Integrated Control Flow Integrity (CFI)
- Integrated load order randomization
- ADLT (Application Data Loading Technology) support
- Enhanced logging and debugging features

### 3.2 Namespace System (namespace.c/h, ns_config.c/h)

**Location**: `ldso/linux/namespace.c`, `ldso/linux/namespace.h`,
         `ldso/linux/ns_config.c`, `ldso/linux/ns_config.h`

**Description**:
- Isolates shared libraries in different namespaces
- Prevents library conflicts and security boundaries
- Multiple independent namespace support
- Namespace inheritance mechanism
- Permitted/allowed list control

**Configuration Method**:
Via configuration files (e.g., `/etc/ld-musl-namespace-arm.ini`):

**Key APIs**:
- `create_namespace()` - Create a new namespace
- `find_library_in_ns()` - Find library in namespace
- `namespace_add_dso()` - Add DSO to namespace
- `parse_ns_config()` - Parse namespace configuration

### 3.3 Control Flow Integrity - CFI (cfi.c/h)

**Location**: `ldso/linux/cfi.c`, `ldso/linux/cfi.h`

**Description**:
- Security enhancement to prevent control flow hijacking
- Defense against ROP (Return-Oriented Programming) attacks
- Validates validity of indirect jump targets

**Implementation Mechanism**:
- **Shadow memory mapping**: Maintains shadow memory for CFI
- **Library alignment**: Requires libraries aligned to 1MB boundaries
- **DSO mapping**: Maps DSOs to CFI shadow regions
- **Runtime validation**: Validates target addresses before indirect calls

**Key APIs**:
- `cfi_init()` - Initialize CFI system
- `cfi_map_shadow()` - Map CFI shadow memory
- `cfi_validate()` - Validate control flow target
- `cfi_check_dso()` - Check DSO CFI compliance

### 3.4 Load Order Randomization (dynlink_rand.c/h)

**Location**: `ldso/linux/dynlink_rand.c`, `ldso/linux/dynlink_rand.h`

**Description**:
- Security through randomization
- Randomizes loading order of dependencies
- Makes memory layout unpredictable to attackers

**Implementation Mechanism**:
- **Handle management**: Manages randomized load handles
- **Task framework**: Task-based loading framework
- **Namespace integration**: Integrated with namespace system
- **Entropy source**: Uses system entropy for random sequence

**Key APIs**:
- `randomize_load_order()` - Randomize load order
- `shuffle_handles()` - Shuffle DSO handle order
- `create_load_task()` - Create load task

### 3.5 ADLT Support (dynlink_adlt.h, zip_archive.h)

**Location**: `ldso/linux/dynlink_adlt.h`, `ldso/linux/zip_archive.h`

**Description**:
- **Application Data Loading Technology**
- Supports loading libraries from ZIP archives
- Compressed library loading for reduced storage
- Supports partial loading (on-demand loading)

**Implementation Mechanism**:
- **ZIP parsing**: Parses ZIP archive format
- **File offset**: Handles offsets within compressed files
- **On-demand extraction**: Extracts required libraries at runtime
- **Randomization integration**: Integrated with load order randomization

**Key APIs**:
- `zip_archive_open()` - Open ZIP archive
- `zip_find_file()` - Find file in ZIP
- `zip_extract()` - Extract compressed content
- `adlt_load_library()` - Load library from archive

### 3.6 Logging Infrastructure (ld_log.c/h)

**Location**: `ldso/linux/ld_log.c`, `ldso/linux/ld_log.h`

**Description**:
- Comprehensive debugging and runtime logging
- HiLog system integration
- Multi-level log support
- Runtime enable/disable capability

**Key APIs**:
```c
LD_LOG(level, "format", ...)  // Basic log macro
LD_ERROR("message")           // Error log
LD_WARNING("message")         // Warning log
LD_INFO("message")            // Info log
LD_DEBUG("message")           // Debug log
```

---

## 4. OpenHarmony Extension Design Goals

The OpenHarmony dynamic linker extends standard musl to achieve:

1. **Security Enhancement**: CFI and randomization mitigate attack surfaces
2. **Isolation**: Namespace system for library conflicts and security boundaries
3. **Performance**: ADLT compressed library loading and fast startup
4. **Debuggability**: Comprehensive logging and configuration system
5. **Compatibility**: Maintains musl ABI compatibility while adding proprietary features

---

## 5. Key File Descriptions

### 5.1 dlstart.c
- **Purpose**: Stage 1 bootstrap, runs before relocations
- **Architecture dependent**: Yes, uses architecture-specific assembly macros
- **Modification frequency**: Low, core startup code

### 5.2 dynlink.c (standard musl)
- **Purpose**: Standard musl dynamic linker
- **Used in OpenHarmony**: No, replaced by linux/dynlink.c

### 5.3 linux/dynlink.c
- **Purpose**: OpenHarmony extended dynamic linker
- **Core**: Integrates all OpenHarmony features
- **Modification frequency**: High, main development file

### 5.4 linux/namespace.c
- **Purpose**: Namespace isolation implementation
- **Configuration**: Parsed via ns_config
- **Key**: Core of library isolation

### 5.5 linux/cfi.c
- **Purpose**: Control Flow Integrity
- **Security**: Defense against ROP attacks
- **Alignment requirement**: DSOs require 1MB alignment

### 5.6 linux/dynlink_rand.c
- **Purpose**: Load order randomization
- **Security**: ASLR supplement
- **Integration**: Works with namespace system

### 5.7 linux/ld_log.c
- **Purpose**: Logging infrastructure
- **Integration**: HiLog system
- **Debugging**: Runtime configurable

### 5.8 linux/ns_config.c
- **Purpose**: Namespace configuration parser
- **Format**: INI format configuration files
- **Path**: `/etc/ld-musl-namespace-*.ini`

---

## 6. Related Resources

- [musl official documentation](http://www.musl-libc.org/)
- [ELF specification](https://refspecs.linuxfoundation.org/elf/elf.pdf)
- [OpenHarmony documentation](https://docs.openharmony.cn/)
- TLS implementation reference: [`ldso/dynlink.c`](ldso/dynlink.c)
