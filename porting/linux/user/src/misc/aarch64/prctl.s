.text
.balign 16
.hidden __syscall_ret
.global __libc_prctl
.type __libc_prctl,%function
__libc_prctl:
	mov x8,#167 //__NR_prctl
	svc #0
	b __syscall_ret

