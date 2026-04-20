## FREAD &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   



#### **NAME**

​       fread - binary stream output.

#### **SYNOPSIS**

       #include <stdio.h> 

       size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

#### **DESCRIPTION**

​The function fread() reads nmemb items of data, each size bytes long, from the stream pointed to by stream, storing them at the location given by ptr.

#### **RETURN VALUE**

​On success, fread() return the number of items read. If an error occurs, or the end of the file is reached, the return value is a short item count (or zero).

The file position indicator for the stream is advanced by the number of bytes successfully read.

fread() does not distinguish between end-of-file and error, and callers must use feof(3) and ferror(3) to determine which occurred.

#### **ERRORS**

The fread() return value is a short item count (or zero)

**end-of-file**: only feof is true, ferror is false

**error**: Both feof and ferror are true

#### ATTRIBUTES

| Interface               | Attribute     | Value    |
| ----------------------- | ------------- | -------- |
| fread()       | Thread safety | MT-Safe  |

STANDARDS
       C11, POSIX.1-2008.

HISTORY
       POSIX.1-2001, C89.

#### EXAMPLES

```c
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main(void)
{
    FILE           *fp;
    size_t         ret;
    unsigned char  buffer[4];

    fp = fopen("/bin/sh", "rb");
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    ret = fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), fp);
    if (ret != ARRAY_SIZE(buffer)) {
        fprintf(stderr, "fread() failed: %zu\n", ret);
        exit(EXIT_FAILURE);
    }
    printf("ELF magic: %#04x%02x%02x%02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);

    ret = fread(buffer, 1, 1, fp);
    if (ret != 1) {
        // To ensure compatibility with multiple platforms, the order of judging feof and ferror must be considered.
        if (ferror(fp)) {
            fprintf(stderr, "fread() failed by err %d\n", errno);
        } else if (feof(fp)) {
            fprintf(stderr, "fread() failed by eof \n");
        }
        exit(EXIT_FAILURE);
    }

    printf("Class: %#04x\n", buffer[0]);

    fclose(fp);

    exit(EXIT_SUCCESS);
}
```


#### COLOPHON

​       this page is part of the C library user-space interface documentation.
​       Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/).