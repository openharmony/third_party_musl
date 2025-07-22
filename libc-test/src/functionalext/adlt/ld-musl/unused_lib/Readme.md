# unused library

## Purpose
The test checks that unused NDSO from libadlt does not cause probs.

## Details

To the moment of writing, the combination of complete adlt and binary
which does not use all NDSOs causes SEGV on some configurations.

More specifically:

libi+libj+libk => libadlt

test1 (depends on libi libj libk) passes
test2 (depends on libi and libj) passes
test3 (depends on libi and libk) crashes

Debugger shows that segfault happens in CTORs processing for NDSO2 (or in DTORs processing if we bypass CTORs under debugger).

Note that `main()` for that test does not use anyting from libraries - it
just linked with different sets of them.

