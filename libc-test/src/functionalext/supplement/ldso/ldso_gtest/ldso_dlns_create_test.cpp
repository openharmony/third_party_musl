#include <dlfcn.h>
#include <dlfcn_ext.h>
#include <gtest/gtest.h>

using namespace std;
using namespace testing::ext;

class LdsoDlnsCreateTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dlns_create_001
 * @tc.desc: Check dlns_create flag run success without exception.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlnsCreateTest, dlns_create_001, TestSize.Level1)
{
    Dl_namespace dlns;
    dlns_init(&dlns, "create2_ns");
    void* handle = dlopen_ns(&dlns, "libldso_ns_empty.so", RTLD_LAZY);
    ASSERT_NE(handle, nullptr);
    dlclose(handle);
    dlns_create2(&dlns, "src/test", 0);
    void* handle2 = dlopen_ns(&dlns, "libldso_ns_empty.so", RTLD_LAZY);
    ASSERT_EQ(handle2, nullptr);
}
