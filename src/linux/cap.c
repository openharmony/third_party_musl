#include "syscall.h"
#include "sys/capability.h"

int capget(cap_user_header_t hdr_ptr, cap_user_data_t data_ptr)
{
	return syscall(SYS_capget, hdr_ptr, data_ptr);
}

int capset(cap_user_header_t hdr_ptr, const cap_user_data_t data_ptr)
{
	return syscall(SYS_capset, hdr_ptr, data_ptr);
}
