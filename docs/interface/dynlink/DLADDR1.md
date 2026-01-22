## DLNS_ADD_PLUGIN_DEFAULT_LD_DICTIONARY &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



### NAME

​       dladdr1 - translate address to symbolic information.

### SYNOPSIS

       int dladdr1(const void *addr, Dl_info *info, void **extra_info,
                   int flags);

### DESCRIPTION
        
Because the internal structure of musl dso differs from glibc, specifying the `flags` parameter as `RTLD_DL_LINKMAP` will disable the corresponding functionality.

The function dladdr1() is like [dladdr](https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/functions/dladdr.html), but returns additional information via the argument `extra_info`.  The information returned depends on the value specified in `flags`, which can have one of the following values:

`RTLD_DL_SYMENT`
              Obtain a pointer to the ELF symbol table entry of the matching symbol.  The extra_info argument is a pointer to a symbol pointer: const ElfW(Sym) **.  The ElfW() macro definition turns its argument into the name of an ELF data type suitable for the hardware architecture. For more details, please refer to [dladdr1](https://www.man7.org/linux/man-pages/man3/dladdr.3.html).

### REFERS

Refers to [dladdr1](https://www.man7.org/linux/man-pages/man3/dladdr.3.html).

### EXAMPLES

```c
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <link.h>

// for outer use, define `RTLD_DL_SYMENT`.
#ifndef RTLD_DL_SYMENT
#define RTLD_DL_SYMENT (1)
#endif

// Correct function pointer type declaration
typedef int (*DL_ADDR1_FUNC_TYPE)(const void *addr, Dl_info *info, void **extra_info, int flags);

int main()
{
    Dl_info info = {0};
    ElfW(Sym) *sym = NULL;  // Use pointer to ElfW(Sym).
    DL_ADDR1_FUNC_TYPE dladdr1 = (DL_ADDR1_FUNC_TYPE)dlsym(RTLD_DEFAULT, "dladdr1");
    if (!dladdr1) {
        printf("Error: dladdr1 not found: %s\n", dlerror());
        return -1;
    }
    
    void *malloc_addr = dlsym(RTLD_DEFAULT, "malloc");
    if (!malloc_addr) {
        printf("Error: malloc not found: %s\n", dlerror());
        return -1;
    }
    
    // Pass the address of sym pointer, not a label address.
    int result = dladdr1(malloc_addr, &info, (void **)&sym, RTLD_DL_SYMENT);
    
    if (result == 0) {
        printf("dladdr1 failed\n");
        return -1;
    }
    
    // Print the information.
    printf("Address: %p\n", malloc_addr);
    printf("Dl_info:\n");
    printf("  dli_fname: %s\n", info.dli_fname ? info.dli_fname : "(null)");
    printf("  dli_fbase: %p\n", info.dli_fbase);
    printf("  dli_sname: %s\n", info.dli_sname ? info.dli_sname : "(null)");
    printf("  dli_saddr: %p\n", info.dli_saddr);
    
    if (sym) {
        printf("Symbol info:\n");
        printf("  st_name: %u\n", sym->st_name);
        printf("  st_info: 0x%x\n", sym->st_info);
        printf("  st_other: 0x%x\n", sym->st_other);
        printf("  st_shndx: %u\n", sym->st_shndx);
        printf("  st_value: %lu\n", sym->st_value);
        printf("  st_size: %lu\n", sym->st_size);
    } else {
        printf("No symbol information available\n");
    }
    
    return 0;
}
```

### NOTE

This interface is differentiated at compile time, only enabled when `musl_extended_function` is true.

### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).