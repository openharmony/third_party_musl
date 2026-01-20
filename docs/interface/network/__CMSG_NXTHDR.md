## __CMSG_NXTHDR &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual

#### **NAME**

​      __cmsg_nxthdr - access ancillary data.

#### **SYNOPSIS**​

```
    #include <sys/socket.h>

    struct cmsghdr *__cmsg_nxthdr(struct msghdr *mhdr, struct cmsghdr *cmsg);
```

#### **DESCRIPTION**

​      The __cmsg_nxthdr() function returns the next valid cmsghdr after the passed cmsghdr. It returns NULL when there isn't enough space left in the buffer.

​      When initializing a buffer that will contain a series of cmsghdr structures, that buffer should first be zero-initialized to ensure the correct operation of __cmsg_nxthdr().

#### **PARAMETERS**

​      **mhdr**: Points to the msghdr structure containing the control message buffer. Which includes:

​     msg_control: Pointer to the buffer storing control messages.

​     msg_controllen: Length of the control message buffer in bytes.

​      **cmsg**: Pointer to the current control message being processed. it returns the next control message after cmsg in the buffer. Note: cmsg can not be NULL, to retrieve the first control message, use the CMSG_FIRSTHDR(mhdr) macro, which returns a pointer to the message header.

#### **RETURN VALUE**

​      Returns a pointer to the next control message header in the buffer.

​      Returns NULL if no more control messages are available.

#### ATTRIBUTES

| Interface       | Attribute     | Value    |
| --------------- | ------------- | -------- |
| __cmsg_nxthdr() | Thread safety | MT-safe  |
|                 | Signal safety | Not Safe |

#### HISTORY

​      -- 2026 

#### NOTES

​      Commonly used for advanced socket operations (e.g., sending/receiving file descriptors, credentials, or network parameters).
Platform-specific behavior may vary in non-Linux systems.

​      This feature is designed specifically for when musl_extended_function is true.

​      The function itself is thread-safe (no global state), but the msghdr buffer must be accessed exclusively.

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### EXAMPLES

```c
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

void fill_control_buffer(char *control_buf, size_t control_len) {
    struct cmsghdr *cmsg;
    int *data;
    cmsg = (struct cmsghdr *)control_buf;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    data = (int *)CMSG_DATA(cmsg);
    *data = 2025;
}

int main(int argc, char *argv[])
{
    char control_buf[1024];
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;

    fill_control_buffer(control_buf, sizeof(control_buf));
    msg.msg_control = control_buf;
    msg.msg_controllen = CMSG_SPACE(sizeof(int));

    for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = __cmsg_nxthdr(&msg, cmsg)) {
        if (*(int *)CMSG_DATA(cmsg) != 2025) {
            printf("%s data error data=%d \n", __func__, *(int *)CMSG_DATA(cmsg));
        }
    }
    return 0;
}
```

#### COLOPHON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).