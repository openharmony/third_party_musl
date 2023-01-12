#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t g_count = 0;
static uint64_t g_type_id = 0;
static void *g_address = NULL;
static void *g_diag = NULL;

// Make sure the library crosses at least one kLibraryAlignment(=256KB) boundary.
char buf[1024 * 1024];

// Mock a CFI-enabled library without relying on the compiler.
__attribute__((aligned(4096))) void __cfi_check(uint64_t CallSiteTypeId,
                                                void *TargetAddr, void *Diag) {
    printf("["__FILE__"][Line: %d][%s]: entry\n", __LINE__, __func__);
    ++g_count;
    g_type_id = CallSiteTypeId;
    g_address = TargetAddr;
    g_diag = Diag;
}

size_t get_count() {
    return g_count;
}

uint64_t get_type_id() {
    return g_type_id;
}

void *get_address() {
    return g_address;
}

void *get_diag() {
    return g_diag;
}

void *get_global_address() {
    return &g_count;
}
