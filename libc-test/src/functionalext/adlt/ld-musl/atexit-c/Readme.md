The ld-musl/atexit-c test checks `atexit()` from musl poit of view.

It checks proper order of calling handlers which were installed
by `atexit()` and belong to different elf files (to `main`, `liba` and `libb`).

Additionally, there are several handlers in each elf and they are mixed like that:
```C
atexit_main1()
atexit_liba_1()
atexit_libb_1()
atexit_liba_2()
atexit_libb_2()
atexit_main2()
```
Such mixed reordering checks, among other things, the following case:
- in legacy (reference) case, where `liba` and `libb` are separate elf-files,
the last liba-related handler will cause the liba to unload.
That does not beak functionality since there a no more calls to liba handlers.
- in adlt case, where `liba` and `libb` are nDSOs in libadlt, the last liba-related
handler should not unload the whole adlt. If it does then we will see some error
at that point (probably the SEGFAULT).
