#ifndef _SYS_IPC_H
#define _SYS_IPC_H
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/features.h>
#include <sys/types.h>

#define __ipc_perm_key __key
#define __ipc_perm_seq __seq

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __key key
#define __seq seq
#endif

#include <bits/ipc.h>
#include <bits/ipcstat.h>

#define IPC_CREAT  01000
#define IPC_EXCL   02000
#define IPC_NOWAIT 04000

#define IPC_RMID 0
#define IPC_SET  1
#define IPC_INFO 3

#define IPC_64 0x0100

#define IPC_PRIVATE ((key_t) 0)

key_t ftok (const char *, int);

#ifdef __cplusplus
}
#endif
#endif
