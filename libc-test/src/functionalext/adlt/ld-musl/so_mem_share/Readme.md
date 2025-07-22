# so_mem_share example

## Task

ensure that processes can not affect each other by corrupting
address space of shared library

## Question to answer

Suppose scenario

1. The ADLT embeds several SOs with their init/fini. For example,
the A, B and their common dependency C
1. Common dependency C has ini/fin code
1. One process opens ADLT for subset A, C. Another one for B, C
1. Contrary to legacy SOs, the init/fini _must_ be executed not for all
SOs embedded into ADLT but only for a subset used.

So, when 1st process opens A, it calls init for C, when 2nd process opens B
it also calls init for C.

The question is whether such double init for C can cause wrong behaviour of any of the processes who uses mmap'ed libraries?

## Comments, code, discussions on the web

1. I did not find any code in musl which allows to share the data between
processes using mmap'ed regions of shared libraries

1. I have seen code in ld-musl which either creates mmap as private copy
of SO (when no MMU available), or creates copy-on-write mapping for a process.

1. I had seen somewhere on the web that process can not affect the mmap'ed
file of SO by changing the contents of mmaped memory. Also, that is
unspecified whether changes in the file *made after mmap* will cause changes in memory of the process.

1. I saw discussion where people stated that sharing happens on kernel level.
That is, the process always maps original library file but the *kernel* skips
copying file contents if it is already in memory. The desision
(copy from file or just use existing map) is made based on
i-node of the file to mmap and also on the offset and size (in file) to mmap.

1. I saw discussions where people explain that linux processes can not just
share changes by changing data in shared library addresses. For such sharing,
the library developer must do that explicitly - e.g., by using SysV shared
memory, explicit mmap of a custom file in RW mode, etc.


## Tests

The question is rather complicated and is critical for ADLT development.
It would be nice if somebody could double-check it.

Meanwhile, I propose to use a set of simple tests which check that
processes can not affect each other by changing data in SO address space

## Test list

Currently implemented:

- TESTNO=0: standalone - just check that chain_a works
- TESTNO=1: fork - chain_a forks, values in both parent and child are checked
- TESTNO=2: fork/exec - chain_a forks, then child exec's chain_b, values in both parent chain_a and in chain_b are checked:

## FYI

The examples in llvm-adlt are built with cmake.
However, the src/Makefile is kept - mainly for reference and for standalone builds.


