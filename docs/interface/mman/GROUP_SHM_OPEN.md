## GROUP_SHM_OPEN &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual

#### **NAME**

​       group_shm_open - create or open a group-scoped POSIX shared memory object.

#### **SYNOPSIS**

```c
#include <sys/group_ipc.h>

int group_shm_open(const char *name, int oflag, mode_t mode, gid_t gid);
```

#### **DESCRIPTION**

​       The group_shm_open() function creates or opens a shared memory object in
​       the group-specific directory selected by **gid**. It behaves like
​       shm_open(), except that the object is mapped to the following path:

```text
/dev/group/shm/<gid>/<name>
```

​       Leading slash characters in **name** are ignored. After leading slashes
​       are removed, **name** must be a single nonempty path component. The names
​       `.` and `..`, embedded or trailing slash characters, and names longer than
​       **NAME_MAX** are rejected.

​       The directory `/dev/group/shm/<gid>` must already exist and must provide
​       the permissions required by the caller. group_shm_open() does not create
​       the directory, change its ownership or permissions, or fall back to
​       `/dev/shm`.

​       The **oflag** and **mode** arguments have the same meaning as for
​       shm_open(). The implementation also applies **O_NOFOLLOW**, **O_CLOEXEC**,
​       and **O_NONBLOCK** when opening the backing object. The returned file
​       descriptor can be used with ftruncate(), mmap(), and close().

​       Processes that use the same **gid** and **name** access the same backing
​       object. Using a different **gid** selects a different path.

#### **PARAMETERS**

​       **name**: Name of the shared memory object.

​       **oflag**: Open flags. Common values include **O_RDONLY**, **O_RDWR**,
​       **O_CREAT**, **O_EXCL**, and **O_TRUNC**.

​       **mode**: Permission bits used when a new object is created. The process
​       file creation mask applies.

​       **gid**: Application-supplied group identifier used as the directory name
​       under `/dev/group/shm`.

#### **RETURN VALUE**

​       On success, group_shm_open() returns a nonnegative file descriptor.

​       On failure, -1 is returned and errno is set to indicate the error.

#### **ERRORS**

​       **EACCES**: The caller does not have the required permission for the group
​       directory or object.

​       **EEXIST**: **O_CREAT** and **O_EXCL** were specified and the object already
​       exists.

​       **EINVAL**: **name** is empty, is `.` or `..`, or contains a slash after
​       leading slashes are removed.

​       **ENAMETOOLONG**: **name** exceeds **NAME_MAX**, or the mapped path cannot
​       be represented by the implementation buffer.

​       **ENOENT**: The group directory does not exist, or the requested object
​       does not exist and **O_CREAT** was not specified.

​       The function may also fail with other errors reported by open().

#### **ATTRIBUTES**

| Interface        | Attribute     | Value    |
| ---------------- | ------------- | -------- |
| group_shm_open() | Thread safety | MT-safe  |
|                  | Signal safety | Not Safe |

#### **HISTORY**

​       -- 2026

#### **NOTES**

When existing musl logging is enabled, a failed call logs the interface name
and errno directly, without changing the original errno or adding permission
checks. group_sem_unlink() uses the shared group_shm_unlink() diagnostic.

​       The **gid** argument is supplied by the application. The function does not
​       require it to match the real or effective group ID of the calling process.

​       Use group_shm_unlink() with the same **gid** and **name** to remove the
​       object's directory entry.

#### **CONFORMING TO**

​       group_shm_open() is an OpenHarmony extension and is not specified by POSIX.

#### **EXAMPLES**

```c
#include <fcntl.h>
#include <stdio.h>
#include <sys/group_ipc.h>
#include <unistd.h>

int main(void)
{
    const gid_t gid = 1001;
    const char *name = "/shared-state";
    int *state;
    int fd;

    /* /dev/group/shm/1001 must already exist. */
    fd = group_shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0600, gid);
    if (fd < 0) {
        perror("group_shm_open");
        return 1;
    }
    if (ftruncate(fd, sizeof(*state)) < 0) {
        perror("ftruncate");
        (void)close(fd);
        (void)group_shm_unlink(name, gid);
        return 1;
    }
    state = mmap(NULL, sizeof(*state), PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0);
    if (state == MAP_FAILED) {
        perror("mmap");
        (void)close(fd);
        (void)group_shm_unlink(name, gid);
        return 1;
    }

    *state = 42;
    (void)munmap(state, sizeof(*state));
    (void)close(fd);
    (void)group_shm_unlink(name, gid);
    return 0;
}
```

#### **COLOPHON**

​       This page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).
