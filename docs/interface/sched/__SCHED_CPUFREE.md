## __sched_cpufree &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   

#### **NAME**

​       __sched_cpufree - A wrapper of free.

#### **SYNOPSIS**

        #include <sched.h>
        #include <stdlib.h>

        void __sched_cpufree(cpu_set_t *set);

#### **DESCRIPTION**

​      The __sched_cpufree() function releases a CPU affinity set previously allocated by scheduling-related allocation interfaces.

​      This function is a thin wrapper around free(), provided to pair with internal CPU set  helpers and to maintain a consistent libc-internal API.

​      The memory pointed to by set must have been obtained via dynamic allocation.

​      Passing a pointer to stack-allocated or statically allocated cpu_set_t objects results in undefined behavior.

#### **RETURN VALUE**

​       The __sched_cpufree() functions do not return.

#### **ERRORS**

​       No errors are defined.

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| __sched_cpufree() | Thread safety | MT-Safe |
|                         | Signal safety | MT-Safe |

#### HISTORY

​       -- 2026

#### NOTES

​       This interface is an internal helper designed for use with musl and OHOS libc

#### CONFORMING TO

​       This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    cpu_set_t *set = malloc(sizeof(cpu_set_t));
    if (!set) {
        return -1;
    }

    CPU_ZERO(set);
    CPU_SET(0, set);

    __sched_cpufree(set);
    return 0;
}

```

#### COLOPHTON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)