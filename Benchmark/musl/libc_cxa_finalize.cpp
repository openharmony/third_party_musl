#include "util.h"

extern "C" int __cxa_atexit(void (*f)(void *), void *, void *);
extern "C" void __cxa_finalize(void *);

static void call(void *p)
{
	if (p != NULL)
		((void (*)(void))(uintptr_t)p)();
}

static void DoNothing() {}

static void Bm_function_cxa_finalize(benchmark::State &state)
{
    uintptr_t dummy;
    for (auto _ : state) {
        state.PauseTiming();
        __cxa_atexit(call, (void *)(uintptr_t)DoNothing, (void*)&dummy);
        state.ResumeTiming();
        __cxa_finalize((void*)&dummy);
    }
}

MUSL_BENCHMARK(Bm_function_cxa_finalize);