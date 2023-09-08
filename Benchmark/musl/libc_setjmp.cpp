#include "setjmp.h"
#include "util.h"

static jmp_buf testJmpBuf;

static void Bm_function_Setjmp(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setjmp(testJmpBuf));
    }
}

MUSL_BENCHMARK(Bm_function_Setjmp);