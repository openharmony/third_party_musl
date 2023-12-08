#include <dlfcn.h>
#include <gtest/gtest.h>
#include <link.h>

using namespace std;
using namespace testing::ext;

#define TEST_VALUE_100 100
#define TEST_VALUE_300 300

class LdsoDlIteratePhdrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

struct InfoOfDyc {
    u_int32_t type;
    u_int32_t vaddr;
};

struct InfoOfDlpi {
    bool hasVal = false;
    uint64_t dlpiAdds = 0;
    uint64_t dlpiSubs = 0;
};

static int CallBack001(struct dl_phdr_info* info, size_t size, void* data)
{
    int* dataChange = reinterpret_cast<int*>(data);
    *dataChange = TEST_VALUE_300;
    return 0;
}

/**
 * @tc.name: dl_iterate_phdr_001
 * @tc.desc: Test callback function call normal.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlIteratePhdrTest, dl_iterate_phdr_001, TestSize.Level1)
{
    int iData = TEST_VALUE_100;
    int* data = &iData;
    ASSERT_EQ(0, dl_iterate_phdr(&CallBack001, static_cast<void*>(data)));
    EXPECT_EQ(*data, TEST_VALUE_300);
}

void CallBack002(dl_phdr_info* info, size_t size)
{
    EXPECT_EQ(sizeof(dl_phdr_info), size);

    ASSERT_NE(info->dlpi_name, nullptr);

    ASSERT_NE(nullptr, info->dlpi_phdr);

    EXPECT_EQ(0, info->dlpi_subs);

    EXPECT_NE(0, info->dlpi_phnum);

    EXPECT_NE(0, info->dlpi_adds);

    bool foundDyc = false;
    for (ElfW(Half) i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr)* phdr = reinterpret_cast<const ElfW(Phdr)*>(&info->dlpi_phdr[i]);
        if (phdr->p_type == PT_DYNAMIC) {
            InfoOfDyc* ehdr = reinterpret_cast<InfoOfDyc*>(info->dlpi_addr + phdr->p_vaddr);
            ASSERT_NE(0, ehdr->type);
            ASSERT_NE(0, ehdr->vaddr);
            foundDyc = true;
            break;
        }
    }
    ASSERT_EQ(true, foundDyc);
}

int TempFunc(dl_phdr_info* info, size_t size, void* data)
{
    CallBack002(info, size);
    return 0;
}

/**
 * @tc.name: dl_iterate_phdr_002
 * @tc.desc: Test that the parameters in the callback function passed in are normal.
 * @tc.type: FUNC
 */
HWTEST_F(LdsoDlIteratePhdrTest, dl_iterate_phdr_002, TestSize.Level1)
{
    ASSERT_EQ(0, dl_iterate_phdr(TempFunc, nullptr));
}
