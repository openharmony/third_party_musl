#ifndef _MUSL_FDTRACK_HOOK_H
#define _MUSL_FDTRACK_HOOK_H

#include <stdatomic.h>
#include <sys/cdefs.h>
#include "common_def.h"
#include "musl_fdtrack.h"

extern _Atomic(fdtrack_hook) __fdtrack_hook;
extern bool __fdtrack_enabled;

#ifdef __cplusplus
extern "C" {
#endif

int FDTRACK_START_HOOK(int fd_value)
{
	int fd = fd_value;
	if (fd != -1 && __predict_false(__fdtrack_enabled) && __predict_false(__fdtrack_hook)) {
		struct fdtrack_event event;
		event.fd = fd;
		event.type = FDTRACK_EVENT_TYPE_CREATE;
		atomic_load(&__fdtrack_hook)(&event);
	}
	return fd;
}

#ifdef __cplusplus
}
#endif

#endif