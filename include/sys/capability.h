#ifndef _SYS_CAPABILITY_H
#define _SYS_CAPABILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/capability.h>

int capget(cap_user_header_t hdr_ptr, cap_user_data_t data_ptr);
int capset(cap_user_header_t hdr_ptr, const cap_user_data_t data_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_CAPABILITY_H */
