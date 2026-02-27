## SET_SIGCHAIN_PRE_VALIDATION_IGNORE &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



### NAME

​       set_sigchain_pre_validation_ignore - control whether to validate SIG_IGN before calling user signal handler.

### SYNOPSIS

       #include <sigchain.h>

       void set_sigchain_pre_validation_ignore(int value);

### DESCRIPTION

The `set_sigchain_pre_validation_ignore()` function controls whether the signal chain should validate the `SIG_IGN` signal handler before invoking the user's signal handler.

When pre-validation is enabled (value is non-zero):
- The signal chain checks if the signal handler is set to `SIG_IGN` before calling the user handler.
- If the handler is `SIG_IGN`, the signal processing returns immediately without calling the user handler to avoid cppcrash.

When pre-validation is disabled (value is zero):
- The signal chain will not perform pre-validation and will proceed to call the user handler regardless of whether it is `SIG_IGN`.

Note: This function is not MT-Safe (multi-thread safe) and not signal-safe. It should not be called from signal handlers.

### PARAMETERS

**value**
       Control flag for SIG_IGN pre-validation.
       
       0      Disable pre-validation (default behavior).
       
       Non-zero    Enable pre-validation.

### RETURN VALUE

The `set_sigchain_pre_validation_ignore()` function does not return a value.

### ATTRIBUTES

| Interface                   | Attribute      | Value    |
| --------------------------- | -------------- | -------- |
| set_sigchain_pre_validation_ignore() | Thread safety  | Not Safe |
|                             | Signal safety  | Not Safe |

### HISTORY

​       -- 2026

### NOTES

The default state is disabled (pre_validation_ignore = 0). This function affects only the signal chain behavior and does not modify the kernel's signal handling disposition.

### CONFORMING TO

​       This is a platform-specific extension for OpenHarmony musl and is not part of any POSIX standard.

### EXAMPLES

```c
#include <signal.h>
#include <sigchain.h>

int main(void)
{
    // Enable pre-validation for SIG_IGN
    set_sigchain_pre_validation_ignore(1);

    // Disable pre-validation
    set_sigchain_pre_validation_ignore(0);
}
```

### SEE ALSO

[sigaction](https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/functions/sigaction.html),
[signal](https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/functions/signal.html),
[add_special_signal_handler](../../src/sigchain/linux/sigchain.c)

### COLOPHON

​      This page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
