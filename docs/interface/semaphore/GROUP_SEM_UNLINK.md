## GROUP_SEM_UNLINK &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual

#### **NAME**

​       group_sem_unlink - remove a group-scoped named semaphore name.

#### **SYNOPSIS**

```c
#include <sys/group_ipc.h>

int group_sem_unlink(const char *name, gid_t gid);
```

#### **DESCRIPTION**

​       The group_sem_unlink() function removes the named semaphore entry mapped
​       to:

```text
/dev/group/shm/<gid>/<name>
```

​       It behaves like sem_unlink(), except that **gid** selects the directory
​       below `/dev/group/shm`. Leading slash characters in **name** are ignored.
​       The remaining name must be a single nonempty path component and must not
​       be `.`, `..`, or longer than **NAME_MAX**.

​       If one or more processes have the semaphore open, removing its name does
​       not invalidate their existing semaphore pointers. The backing object is
​       released after its name has been removed and all open references have been
​       closed with sem_close().

​       The function does not remove `/dev/group/shm/<gid>` itself.

#### **PARAMETERS**

​       **name**: Name of the semaphore to remove.

​       **gid**: Application-supplied group identifier used to locate the
​       semaphore.

#### **RETURN VALUE**

​       On success, group_sem_unlink() returns 0.

​       On failure, -1 is returned and errno is set to indicate the error.

#### **ERRORS**

​       **EACCES** or **EPERM**: The caller is not permitted to remove the
​       semaphore object.

​       **EINVAL**: **name** is empty, is `.` or `..`, or contains a slash after
​       leading slashes are removed.

​       **ENAMETOOLONG**: **name** exceeds **NAME_MAX**, or the mapped path cannot
​       be represented by the implementation buffer.

​       **ENOENT**: The group directory or semaphore object does not exist.

​       The function may also fail with other errors reported by unlink().

#### **ATTRIBUTES**

| Interface          | Attribute     | Value    |
| ------------------ | ------------- | -------- |
| group_sem_unlink() | Thread safety | MT-safe  |
|                    | Signal safety | Not Safe |

#### **HISTORY**

​       -- 2026

#### **NOTES**

When existing musl logging is enabled, a failed call logs the interface name
and errno directly, without changing the original errno or adding permission
checks. group_sem_unlink() uses the shared group_shm_unlink() diagnostic.

​       Use the same **gid** and **name** that were passed to group_sem_open().

​       group_sem_unlink() removes the name only. Use sem_close() to release each
​       open semaphore reference in the calling process.

#### **CONFORMING TO**

​       group_sem_unlink() is an OpenHarmony extension and is not specified by
​       POSIX.

#### **EXAMPLES**

```c
#include <sys/group_ipc.h>
#include <stdio.h>

int main(void)
{
    if (group_sem_unlink("/ready", 1001) < 0) {
        perror("group_sem_unlink");
        return 1;
    }
    return 0;
}
```

#### **COLOPHON**

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
