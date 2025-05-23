#include "stdio_impl.h"
#include "aio_impl.h"

static int dummy(int fd)
{
	return fd;
}

weak_alias(dummy, __aio_close);

int __stdio_close(FILE *f)
{
#ifdef __LITEOS__
	return syscall(SYS_close, __aio_close(f->fd));
#else
	__aio_close(f->fd);
	return fdsan_close_with_tag(f->fd, __get_file_tag(f));
#endif
}
