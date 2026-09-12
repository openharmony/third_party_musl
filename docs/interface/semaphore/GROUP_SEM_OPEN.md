## GROUP_SEM_OPEN &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual

#### **NAME**

​       group_sem_open - create or open a group-scoped named semaphore.

#### **SYNOPSIS**

```c
#include <sys/group_ipc.h>

sem_t *group_sem_open(const char *name, int oflag, gid_t gid, ...);
```

#### **DESCRIPTION**

​       The group_sem_open() function creates or opens a named semaphore in the
​       group-specific directory selected by **gid**. It behaves like sem_open(),
​       except that the semaphore is backed by:

```text
/dev/group/shm/<gid>/<name>
```

​       Leading slash characters in **name** are ignored. After leading slashes
​       are removed, **name** must be a single nonempty path component. The names
​       `.` and `..`, embedded or trailing slash characters, and names longer than
​       **NAME_MAX** are rejected.

​       The directory `/dev/group/shm/<gid>` must already exist and must provide
​       the permissions required by the caller. group_sem_open() does not create
​       the directory, change its ownership or permissions, or fall back to
​       `/dev/shm`.

​       Only **O_CREAT** and **O_EXCL** are used from **oflag**. If **O_CREAT** is
​       specified, two additional arguments must follow **gid**: a value of type
​       mode_t that specifies the permission bits, and an unsigned value that
​       specifies the initial semaphore value. The initial value must not exceed
​       **SEM_VALUE_MAX**.

​       Creation uses a temporary file in `/dev/group/shm/<gid>` and atomically
​       links it to the requested name. Processes that use the same **gid** and
​       **name** access the same named semaphore. A different **gid** selects a
​       different path.

#### **PARAMETERS**

​       **name**: Name of the semaphore.

​       **oflag**: Zero or a bitwise OR of **O_CREAT** and **O_EXCL**.

​       **gid**: Application-supplied group identifier used as the directory name
​       under `/dev/group/shm`.

​       **mode**: Required after **gid** when **O_CREAT** is specified. Only the
​       permission bits represented by `0666` are used, and the process file
​       creation mask applies.

​       **value**: Required after **mode** when **O_CREAT** is specified. Initial
​       semaphore value in the range from 0 through **SEM_VALUE_MAX**.

#### **RETURN VALUE**

​       On success, group_sem_open() returns a pointer to the named semaphore.

​       On failure, **SEM_FAILED** is returned and errno is set to indicate the
​       error.

#### **ERRORS**

​       **EACCES**: The caller does not have the required permission for the group
​       directory or semaphore object.

​       **EEXIST**: **O_CREAT** and **O_EXCL** were specified and the semaphore
​       already exists.

​       **EINVAL**: **name** is invalid, or **value** exceeds **SEM_VALUE_MAX**.

​       **EMFILE**: No named-semaphore mapping slot is available in the process.

​       **ENAMETOOLONG**: **name** exceeds **NAME_MAX**, or the mapped path cannot
​       be represented by the implementation buffer.

​       **ENOENT**: The group directory does not exist, or the requested semaphore
​       does not exist and **O_CREAT** was not specified.

​       **ENOMEM**: Memory for the semaphore table or shared mapping could not be
​       allocated.

​       The function may also fail with errors reported by access(), open(),
​       write(), fstat(), mmap(), or link().

#### **ATTRIBUTES**

| Interface        | Attribute     | Value    |
| ---------------- | ------------- | -------- |
| group_sem_open() | Thread safety | MT-safe  |
|                  | Signal safety | Not Safe |

#### **HISTORY**

​       -- 2026

#### **NOTES**

When existing musl logging is enabled, a failed call logs the interface name
and errno directly, without changing the original errno or adding permission
checks. group_sem_unlink() uses the shared group_shm_unlink() diagnostic.

​       Close a semaphore returned by group_sem_open() with sem_close(). No
​       separate group_sem_close() interface is required.

​       Use group_sem_unlink() with the same **gid** and **name** to remove the
​       semaphore name.

​       The **gid** argument is supplied by the application. The function does not
​       require it to match the real or effective group ID of the calling process.

#### **CONFORMING TO**

​       group_sem_open() is an OpenHarmony extension and is not specified by POSIX.

#### **EXAMPLES**

```c
#include <fcntl.h>
#include <sys/group_ipc.h>
#include <stdio.h>

int main(void)
{
    const gid_t gid = 1001;
    const char *name = "/ready";
    sem_t *sem;

    /* /dev/group/shm/1001 must already exist. */
    sem = group_sem_open(name, O_CREAT | O_EXCL, gid, 0600, 0);
    if (sem == SEM_FAILED) {
        perror("group_sem_open");
        return 1;
    }

    (void)sem_post(sem);
    (void)sem_wait(sem);
    (void)sem_close(sem);
    (void)group_sem_unlink(name, gid);
    return 0;
}
```

#### **COLOPHON**

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
