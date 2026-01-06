/**
 * Copyright (c) 2026 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include "syscall.h"
#include <stdio.h>

#ifdef MUSL_EXTERNAL_FUNCTION
struct cmsghdr *__cmsg_nxthdr(struct msghdr *mhdr, struct cmsghdr *cmsg)
{
	size_t space_size = __MHDR_END(mhdr) - (unsigned char *)cmsg;
	if ((size_t)cmsg->cmsg_len < sizeof(struct cmsghdr) ||
		space_size < sizeof(struct cmsghdr) + CMSG_ALIGN(cmsg->cmsg_len) ||
		space_size < sizeof(struct cmsghdr) + CMSG_ALIGN(cmsg->cmsg_len) - cmsg->cmsg_len) {
		return (struct cmsghdr *)0;
	}
	return (struct cmsghdr *)__CMSG_NEXT(cmsg);
}
#endif