.text
.balign 16
.hidden __syscall_ret
.global fstat64
.global fstat
.type fstat,%function
.type fstat64,%function
fstat64:
fstat:
	mov x8,#80 //__NR_fstat
	svc #0
	b __syscall_ret

