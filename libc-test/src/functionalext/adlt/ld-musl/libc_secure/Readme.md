# libc.secure

## Purpose
ld-musl makes difference for plain executables and for insecure ones (with SUID/SGID bits set).
Partly, it disables the lookup in LD_LIBRARY_PATH directories for insecure ones.
That test checks that functionality,

## Special notes

1. That test uses not a qemu binary but a shell script running on host.
2. The qemu binary runs twice - before and after moving libraries. In second case, the LD_LIBRARY_PATH
is set accordingly. That should work on plain executables but should fail ob SUID/SGID ones.
3. For some reason, all executables are secure under qemu-static, so the test fails (both cases work). The difference is seen when running test on the ARM VM or device.

