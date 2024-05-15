#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct cookie
{
    size_t pos;
    size_t len;
    size_t size;
    char *buf;
};

ssize_t mread(void *f, char *buf, size_t len)
{
    struct cookie *c = f;
    if (c->pos > c->len)
    {
        return 0;
    }
    else if (c->pos + len > c->len)
    {
        len = c->len - c->pos;
    }

    memcpy(buf, c->buf + c->pos, len);
    c->pos += len;
    return len;
}

ssize_t mwrite(void *f, const char *buf, size_t len)
{
    struct cookie *c = f;
    char *new_buf;
    if (c->pos + len > c->size)
    {
        while (c->pos + len > c->size)
        {
            c->size *= 2;
        }
        new_buf = realloc(c->buf, c->size);
        if (new_buf == NULL)
        {
            return -1;
        }
        else
        {
            c->size = c->pos + len;
            c->buf = new_buf;
        }
    }

    memcpy(c->buf + c->pos, buf, len);
    c->pos += len;
    if (c->pos > c->len)
    {
        c->len = c->pos;
    }
    return len;
}

int mseek(void *f, off_t *off, int whence)
{
    struct cookie *c = f;
    off_t new_pos;
    if (whence == 0)
    {
        new_pos = *off;
    }
    else if (whence == 1)
    {
        new_pos = *off + c->pos;
    }
    else if (whence == 2)
    {
        new_pos = *off + c->len;
    }
    else
    {
        return -1;
    }
    if (new_pos < 0)
    {
        return -1;
    }

    c->pos = new_pos;
    *off = new_pos;

    return 0;
}

int mclose(void *f)
{
    struct cookie *c = f;
    free(c->buf);
    c->buf = NULL;
    return 0;
}

int main(int argc, char *argv[])
{
    cookie_io_functions_t io_func = {.read = mread, .write = mwrite, .seek = mseek, .close = mclose};
    FILE *file;
    struct cookie myc;
    size_t nread;
    char buf[50];
    myc.buf = malloc(4);
    if (myc.buf == NULL)
    {
        t_error("fopencookie malloc failed\n");
        return 1;
    }
    myc.pos = 0;
    myc.len = 0;
    myc.size = 4;

    file = fopencookie(&myc, "w+", io_func);
    if (file == NULL)
    {
        t_error("create fopencookie failed\n");
        return 1;
    }
    if (fputs("fopencookie", file) == EOF)
    {
        t_error("fopencookie fputs failed\n");
        return 1;
    }
    if (fseek(file, 0, SEEK_SET) == -1)
    {
        t_error("fopencookie fseek failed\n");
        return 1;
    }
    nread = fread(buf, 1, 2, file);
    if (nread == 0)
    {
        if (ferror(file) != 0)
        {
            t_error("fopencookie fread failed\n");
            return 1;
        }
    }
    fclose(file);
    return 0;
}