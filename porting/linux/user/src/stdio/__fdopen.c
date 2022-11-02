/* Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stdio_impl.h"
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "libc.h"

static size_t get_bufsize(int fd)
{
	struct stat st;
	size_t buf_size = 0;

	if (fstat(fd, &st) < 0) {
		buf_size = BUFSIZ;
	} else if (st.st_blksize == 0) {
		buf_size = BUFSIZ;
	} else {
		buf_size = st.st_blksize;
	}

	return buf_size;
}

FILE *__fdopen(int fd, const char *mode)
{
	FILE *f;
	struct winsize wsz;
	size_t buf_size = 0;

	/* Check for valid initial mode character */
	if (!strchr("rwa", *mode)) {
		errno = EINVAL;
		return 0;
	}

	/* get buffer size via file stat */
	buf_size = get_bufsize(fd);

	/* Allocate FILE+buffer or fail */
	if (!(f = malloc(sizeof *f + UNGET + buf_size))) {
		return 0;
	}

	/* Zero-fill only the struct, not the buffer */
	memset(f, 0, sizeof *f);

	/* Impose mode restrictions */
	if (!strchr(mode, '+')) {
		f->flags = (*mode == 'r') ? F_NOWR : F_NORD;
	}

	/* Apply close-on-exec flag */
	if (strchr(mode, 'e')) {
		__syscall(SYS_fcntl, fd, F_SETFD, FD_CLOEXEC);
	}

	/* Set append mode on fd if opened for append */
	if (*mode == 'a') {
		int flags = __syscall(SYS_fcntl, fd, F_GETFL);
		if (!(flags & O_APPEND))
			__syscall(SYS_fcntl, fd, F_SETFL, flags | O_APPEND);
		f->flags |= F_APP;
	}

	f->fd = fd;
	f->buf = (unsigned char *)f + sizeof *f + UNGET;
	f->buf_size = buf_size;

	/* Activate line buffered mode for terminals */
	f->lbf = EOF;
	if (!(f->flags & F_NOWR) && !__syscall(SYS_ioctl, fd, TIOCGWINSZ, &wsz)) {
		f->lbf = '\n';
	}

	/* Initialize op ptrs. No problem if some are unneeded. */
	f->read = __stdio_read;
	f->write = __stdio_write;
	f->seek = __stdio_seek;
	f->close = __stdio_close;

	if (!libc.threaded) {
		f->lock = -1;
	}

	/* Add new FILE to open file list */
	return __ofl_add(f);
}

weak_alias(__fdopen, fdopen);
