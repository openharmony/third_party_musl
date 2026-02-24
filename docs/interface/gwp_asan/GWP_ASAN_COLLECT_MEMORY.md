## GWP_ASAN_COLLECT_MEMORY &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; OHOS Programmer's Manual   


#### **NAME**

​       libc_gwp_asan_collect_allocations_by_time_range - collect suspicious memory allocated by gwp_asan that  may cause memory leaks. 

#### **SYNOPSIS**

​       #include <gwp_asan.h>

       size_t libc_gwp_asan_collect_allocations_by_time_range(uint64_t  timespan, uintptr_t *buffer, size_t max_count, size_t depth);

#### **DESCRIPTION**

Fills the user-provided buffer with a snapshot of currently live GWP-ASan allocations that have been persistent over a recent time window.

The number of such allocations is capped by **max_count**. The function returns how many entries were written.

**Returned buffer structure**

- **Header** (at the start of buffer), layout:
  - `max_slots` (size_t): State.MaxSimultaneousAllocations.
  - `sample_rate` (size_t): Sample rate .

- **Entry layout** (repeated Count times, one per allocation):
  - `addr` (uintptr_t): allocation address.
  - `size` (size_t): requested allocation size.
  - `lifetime` (uint64_t): time in ms from allocation to the snapshot time (CurrentTime - allocation_time).
  - `stack[]` (uintptr_t[Depth]): allocation site stack trace (program counters). Unused or invalid slots are zero; invalid PCs (0, (uintptr_t)-1, (uintptr_t)-2) are omitted.

**Sizes**

- **HeaderSize** = sizeof(BufferHeader) (two size_t fields).
- **EntrySize** = sizeof(uintptr_t) + sizeof(size_t) + sizeof(uint64_t) + Depth * sizeof(uintptr_t).
- Minimum buffer size (in bytes): HeaderSize + Count * EntrySize. The function never writes more than MaxCount entries, so a safe size is **HeaderSize + MaxCount * EntrySize**.

**Example (pseudo layout)**

```
Buffer: [BufferHeader]  max_slots, sample_rate
        [Entry 0]       addr, size, lifetime, stack[0..Depth-1]
        [Entry 1]       addr, size, lifetime, stack[0..Depth-1]
        ...
        [Entry Count-1] addr, size, lifetime, stack[0..Depth-1]
``` 
       
**Parameters**

| Name| Description| 
| -------- | -------- |
| timespan | collect memory whose lifetime exceeds the given time. Unit: s (seconds). | 
| buffer | the address which store the collected infomation. | 
| max_count | the maxt count of memory can be stored in the address. | 
| depth | maximum backtrace depth for memory allocation. | 

​       Note: This function is MT-Safe(multi-thread safe) and not signal-safe.

#### **RETURN VALUE**

**size_t** the number of accollected memmory.

#### ATTRIBUTES

| Attribute     | Value    |
| ------------- | -------- |
| Thread safety | MT-safe  |
| Signal safety | Not Safe |

#### HISTORY

​       -- 2026

#### EXAMPLES

```c
// This testcase must be compile and execute in system-side

struct BufferHeader {
    size_t max_slot;
    size_t sample_rate;
}

struct AllocationData {
    uintptr_t addr;
    size_t size;
    uint64_t alive_time;
    uintptr_t stack[10];
}

extern "C" size_t libc_gwp_asan_collect_allocations_by_time_range(uint64_t  timespan, uintptr_t *buffer, size_t max_count, size_t depth);

int main()
{
    int* local_ptr = static_cast<int*>(p1);
    sleep(20);

    const size_t MAX_ALLOCATIONS = 100;
    uint64_t timespan = 20;
    size_t depth = 10;

    size_t HeaderSize = sizeof(BufferHeader);
    size_t entry_size = sizeof(AllocationData);
    uintptr_t *buffer = (uintptr_t *)malloc(HeaderSize + MAX_ALLOCATIONS * entry_size);

    if (!buffer) {
       return 1;
    }

    size collected_count = libc_gwp_asan_collect_allocations_by_time_range(timespan, buffer, MAX_ALLOCATIONS, depth);

}
```

#### CONFORMING TO

​      This is a platform-specific extension and is not part of any POSIX standard.

#### COLOPHTON

​      this page is part of the C library user-space interface documentation.
​      Information about the project can be found at (https://gitcode.com/openharmony/third_party_musl/blob/master/docs/)