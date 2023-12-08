#include <dlfcn.h>
#include <gtest/gtest.h>
#include <stdint.h>
#include <stdlib.h>

#include "libs/ldso_gtest_util.h"

#define LIB_PATH "/data/tmp/libcgtest/libs/libldso_cfi_lib.so"
#define TYPE_ID_1 1
#define TYPE_ID_2 2
#define TYPE_ID_3 3
#define TYPE_ID_4 4
#define TYPE_ID_5 5

using namespace std;
using namespace testing::ext;
class LdsoCfiTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

struct dso {
    char* mock;
    unsigned char* map;
    size_t len;
};

extern "C" int init_cfi_shadow(struct dso* dso_list, struct dso* ldso);
extern "C" int map_dso_to_cfi_shadow(struct dso* dso);
extern "C" void unmap_dso_from_cfi_shadow(struct dso* dso);
extern "C" void __cfi_slowpath(uint64_t CallSiteTypeId, void* Ptr);
extern "C" void __cfi_slowpath_diag(uint64_t CallSiteTypeId, void* Ptr, void* DiagData);

typedef void (*FuncTypeRetVoid)();
typedef void* (*FuncTypeRetVoidPtr)();
typedef char* (*FuncTypeRetCharPtr)();
typedef size_t (*FuncTypeRetSize)();
typedef uint64_t (*FuncTypeRetUint64)();

FuncTypeRetSize g_getCount = nullptr;
FuncTypeRetUint64 g_getTypeId = nullptr;
FuncTypeRetVoidPtr g_getAddress = nullptr;
FuncTypeRetVoidPtr g_getDiag = nullptr;
FuncTypeRetVoidPtr g_getGlobalAddress = nullptr;
FuncTypeRetCharPtr g_bufCheck = nullptr;

static void TestFunc() {}

/**
 * @tc.name: __cfi_slowpath_001
 * @tc.desc: Load a so twice and verify it can be mapped to cfi shadow successfully.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, __cfi_slowpath_001, TestSize.Level1)
{
    uint64_t callSiteTypeId1 = TYPE_ID_1;
    uint64_t callSiteTypeId2 = TYPE_ID_2;
    void* handle = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    g_getGlobalAddress = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle, "GetGlobalAddress"));
    __cfi_slowpath(callSiteTypeId1, (*g_getGlobalAddress)());

    void* handle2 = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_EQ(handle, handle2);
    g_getCount = reinterpret_cast<FuncTypeRetSize>(dlsym(handle2, "GetCount"));
    g_getTypeId = reinterpret_cast<FuncTypeRetUint64>(dlsym(handle2, "GetTypeId"));
    g_getAddress = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle2, "GetAddress"));
    g_getGlobalAddress = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle2, "GetGlobalAddress"));

    size_t count = (*g_getCount)();
    __cfi_slowpath(callSiteTypeId2, (*g_getGlobalAddress)());
    EXPECT_EQ(callSiteTypeId2, (*g_getTypeId)());
    EXPECT_EQ((*g_getGlobalAddress)(), (*g_getAddress)());
    EXPECT_EQ(++count, (*g_getCount)());

    dlclose(handle);
    dlclose(handle2);
}

/**
 * @tc.name: __cfi_slowpath_002
 * @tc.desc: Loading a so that contains __cfi_check() symbol, call the __cfi_slowpath() function with address inside
 *           the DSO, the __cfi_check() function is called.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, __cfi_slowpath_002, TestSize.Level1)
{
    uint64_t expectedCallSiteTypeId = TYPE_ID_2;
    void* handle = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    g_getCount = reinterpret_cast<FuncTypeRetSize>(dlsym(handle, "GetCount"));
    g_getTypeId = reinterpret_cast<FuncTypeRetUint64>(dlsym(handle, "GetTypeId"));
    g_getAddress = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle, "GetAddress"));
    g_getDiag = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle, "GetDiag"));
    g_getGlobalAddress = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle, "GetGlobalAddress"));

    size_t count = (*g_getCount)();

    __cfi_slowpath(expectedCallSiteTypeId, (*g_getGlobalAddress)());
    EXPECT_EQ(expectedCallSiteTypeId, (*g_getTypeId)());
    EXPECT_EQ((*g_getGlobalAddress)(), (*g_getAddress)());
    EXPECT_EQ(nullptr, (*g_getDiag)());
    EXPECT_EQ(++count, (*g_getCount)());

    dlclose(handle);
}

/**
 * @tc.name: __cfi_slowpath_003
 * @tc.desc: load a so that contains the __cfi_check() symbol and call the __cfi_slowpath() function whose address
 *           belongs to another so that is not Cross so enabled and the cfi_check function will not called.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, __cfi_slowpath_003, TestSize.Level1)
{
    uint64_t expectedCallSiteTypeId = TYPE_ID_3;
    uint64_t unexpectedCallSiteTypeId = TYPE_ID_4;
    void* handle = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    g_getCount = reinterpret_cast<FuncTypeRetSize>(dlsym(handle, "GetCount"));
    g_getGlobalAddress = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle, "GetGlobalAddress"));

    __cfi_slowpath(expectedCallSiteTypeId, (*g_getGlobalAddress)());
    size_t count = (*g_getCount)();
    __cfi_slowpath(unexpectedCallSiteTypeId, reinterpret_cast<void*>(&TestFunc));

    EXPECT_EQ(count, (*g_getCount)());
    dlclose(handle);
}

/**
 * @tc.name: __cfi_slowpath_004
 * @tc.desc: Call __cfi_slowpath() with nullptr addr and verify the cfi_check function will not called.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, __cfi_slowpath_004, TestSize.Level1)
{
    uint64_t callSiteTypeId = TYPE_ID_3;
    void* handle = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    g_getCount = reinterpret_cast<FuncTypeRetSize>(dlsym(handle, "GetCount"));

    size_t count = (*g_getCount)();
    __cfi_slowpath(callSiteTypeId, nullptr);
    EXPECT_EQ(count, (*g_getCount)());

    dlclose(handle);
}

/**
 * @tc.name: __cfi_slowpath_005
 * @tc.desc: passing a invalid address to the slowpath() function expect Call__ Cfi_ Coredump occurs.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, __cfi_slowpath_005, TestSize.Level1)
{
    uint64_t callSiteTypeId = TYPE_ID_3;
    void* handle = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    g_getCount = reinterpret_cast<FuncTypeRetSize>(dlsym(handle, "GetCount"));

    size_t count = (*g_getCount)();
    ASSERT_EXIT(__cfi_slowpath(callSiteTypeId, static_cast<void*>(&count)), ::testing::KilledBySignal(SIGILL), "");

    dlclose(handle);
}

/**
 * @tc.name: __cfi_slowpath_006
 * @tc.desc: close handle then call the cfi_slowpath and the cfi_slowpath will be killed by signal.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, __cfi_slowpath_006, TestSize.Level1)
{
    uint64_t callSiteTypeId = TYPE_ID_3;
    void* handle = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_NE(handle, nullptr);
    g_getGlobalAddress = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle, "GetGlobalAddress"));
    dlclose(handle);

    ASSERT_EXIT(__cfi_slowpath(callSiteTypeId, (*g_getGlobalAddress)()), ::testing::KilledBySignal(SIGSEGV), "");
}

/**
 * @tc.name: __cfi_slowpath_007
 * @tc.desc: Load a so greater than 1 LIBRARY_ALIGNMENT, call the __cfi_slowpath() function in a different
 *           LIBRARY_ALIGNMENT range, and verify that __cfi_check() is called.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, __cfi_slowpath_007, TestSize.Level1)
{
    uint64_t callSiteTypeId = TYPE_ID_5;
    void* handle = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    g_getCount = reinterpret_cast<FuncTypeRetSize>(dlsym(handle, "GetCount"));
    g_bufCheck = reinterpret_cast<FuncTypeRetCharPtr>(dlsym(handle, "g_buf"));

    size_t count = (*g_getCount)();
    const size_t size = SIZE_1024_SQUARE;

    for (size_t i = 0; i < size; ++i) {
        __cfi_slowpath(callSiteTypeId, reinterpret_cast<char*>(g_bufCheck) + i);
        EXPECT_EQ(++count, (*g_getCount)());
    }

    dlclose(handle);
}

/**
 * @tc.name: __cfi_slowpath_diag_function_test_001
 * @tc.desc: Loading with The so file of the __cfi_check() symbol calls the internal address of the so file__
 *           cfi_slowpath_ The diag() function__cfi_the check() function is called.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, __cfi_slowpath_diag_function_test_001, TestSize.Level1)
{
    uint64_t callSiteTypeId = TYPE_ID_1;
    uint64_t diagInfo = 0;
    void* handle = dlopen(LIB_PATH, RTLD_NOW);
    ASSERT_NE(handle, nullptr);

    g_getCount = reinterpret_cast<FuncTypeRetSize>(dlsym(handle, "GetCount"));
    g_getTypeId = reinterpret_cast<FuncTypeRetUint64>(dlsym(handle, "GetTypeId"));
    g_getDiag = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle, "GetDiag"));
    g_getGlobalAddress = reinterpret_cast<FuncTypeRetVoidPtr>(dlsym(handle, "GetGlobalAddress"));

    size_t count = (*g_getCount)();
    __cfi_slowpath_diag(callSiteTypeId, nullptr, &diagInfo);
    EXPECT_EQ(count, (*g_getCount)());

    __cfi_slowpath_diag(callSiteTypeId, (*g_getGlobalAddress)(), &diagInfo);
    EXPECT_EQ(++count, (*g_getCount)());
    EXPECT_EQ(callSiteTypeId, (*g_getTypeId)());
    EXPECT_EQ((*g_getDiag)(), &diagInfo);

    dlclose(handle);
}

/**
 * @tc.name: map_dso_to_cfi_shadow_001
 * @tc.desc: If so is nullptr while mapping to the CFI shadow, do nothing and return true.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, map_dso_to_cfi_shadow_001, TestSize.Level1)
{
    EXPECT_EQ(map_dso_to_cfi_shadow(nullptr), 0);
}

/**
 * @tc.name: cfi_init_test_001
 * @tc.desc: check all args is null when initializing the CFI shadow. do nothing and return true.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, cfi_init_test_001, TestSize.Level1)
{
    EXPECT_EQ(init_cfi_shadow(nullptr, nullptr), 0);
}

/**
 * @tc.name: unmap_dso_to_cfi_shadow_001
 * @tc.desc: If so is nullptr while unmapping from the CFI shadow the program can be run success.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoCfiTest, unmap_dso_to_cfi_shadow_001, TestSize.Level1)
{
    unmap_dso_from_cfi_shadow(nullptr);
    SUCCEED();
}
