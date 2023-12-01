#include "util.h"

static int count[8] = {1, 4, 16, 32, 64, 128, 256, 1024};

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
    __cxa_atexit(call, (void *)(uintptr_t)DoNothing, (void*)&dummy);

    for (auto _ : state) {
        __cxa_finalize((void*)&dummy);
    }
}

static void Bm_function_cxa_finalize_dynamic(benchmark::State &state)
{
    uintptr_t dummy;

    for (auto _ : state) {
        state.PauseTiming();
        for (int i = 0; i < count[state.range(0)]; i++) {
            __cxa_atexit(call, (void *)(uintptr_t)DoNothing, (void*)&dummy);
        }
        state.ResumeTiming();
        __cxa_finalize((void*)&dummy);
    }
}

MUSL_BENCHMARK(Bm_function_cxa_finalize);
MUSL_BENCHMARK_WITH_ARG(Bm_function_cxa_finalize_dynamic, "BENCHMARK_8");