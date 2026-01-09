## FCNTL64 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual



#### **NAME**

​       fcntl64 - manipulate file descriptor.

#### **SYNOPSIS**​

       #include <unistd.h>
       #include <fcntl.h>

       int fcntl64(int fd, int cmd, ... /* arg */ );

#### **DESCRIPTION**

​       When explicitly requesting the use of LFS's 64 suffix interface (such as open64) or enabling all GNU extensions including LFS support, fcntl64 is equivalent to the fcntl function in a 64 bit environment.

​       Certain of the operations below are supported only since a particular Linux kernel version.  The preferred method of checking whether the host kernel supports a  particular operation is to invoke fcntl64() with the desired cmd value and then test whether the call failed with EINVAL, indicating that the kernel does not recognize this value.

**Advisory record locking**

​       Linux implements traditional ("process-associated") UNIX record locks, as standardized by POSIX.  For a Linux-specific alternative with better semantics, see the discussionof open file description locks below.


​       F_SETLK64, F_SETLKW64, and F_GETLK64 are used to acquire, release, and test for the existence of record locks (also known as byte-range, file-segment, or file-region locks).   On systems that support 64 bit offset (such as modern Linux), macros such as F_GETLK64 are defined as aliases for dependent primitive commands.The third argument, lock, is a pointer to a structure that has at least the following fields (in unspecified order).


    struct flock {
               ...
               short l_type;    /* Type of lock: F_RDLCK, F_WRLCK, F_UNLCK */
               short l_whence;  /* How to interpret l_start: SEEK_SET, SEEK_CUR, SEEK_END */
               off_t l_start;   /* Starting offset for lock */
               off_t l_len;     /* Number of bytes to lock */
               pid_t l_pid;     /* PID of process blocking our lock  (set by F_GETLK and F_OFD_GETLK) */
               ...
           };

 The  flock64 is an alias for flock.

#### **RETURN VALUE**

 ​      The function fcntl64 has the same return value as fcntl.


#### **ERRORS**

​       The function fcntl64 has the same error codes as fcntl.
​

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| fcntl64()               | Thread safety | MT-safe  |
|                         | Signal safety | Safe     |

#### HISTORY

​       -- 2026

#### NOTES

​       This feature is designed specifically for when musl_extended_function is true.

#### EXAMPLES

```c
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;
    struct flock64 lock;
    int ret;

    fd = open("testfile.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    int flags = fcntl64(fd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl64 F_GETFL failed");
        close(fd);
        return 1;
    }

    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 100;

    ret = fcntl64(fd, F_SETLK64, &lock);
    if (ret < 0) {
        perror("fcntl64 F_SETLK64 failed");
    }

    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 100;

    ret = fcntl64(fd, F_GETLK64, &lock);
    if (ret < 0) {
        perror("fcntl64 F_GETLK64 failed");
    }

    int fd_flags = fcntl64(fd, F_GETFD, 0);
    if (fd_flags >= 0) {
        ret = fcntl64(fd, F_SETFD, fd_flags | FD_CLOEXEC);
        if (ret < 0) {
            perror("fcntl64 F_SETFD failed");
        }
    }

    lock.l_type = F_UNLCK;
    fcntl64(fd, F_SETLK64, &lock);

    close(fd);
    unlink("testfile.txt");

    return 0;
}
```


#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).