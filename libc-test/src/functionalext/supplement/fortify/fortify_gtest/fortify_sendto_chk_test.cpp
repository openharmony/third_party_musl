#include <sys/socket.h>
#include <fortify/socket.h>
#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/wait.h>

constexpr int BUF_SIZE = 10;

using namespace testing::ext;

class FortifySendtochkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static void SignalHandler(int signum)
{
    kill(getpid(), SIGSTOP);
}

/**
 * @tc.name: __sendto_chk_001
 * @tc.desc: Verify if the __sendto_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifySendtochkTest, __sendto_chk_001, TestSize.Level1)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, nullptr);

    size_t dataLen = atoi("11");
    char buf[BUF_SIZE];
    int status;
    int pid = fork();
    switch (pid) {
        case 0:
            __sendto_chk(0, buf, dataLen, dataLen, 0, nullptr, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            EXPECT_TRUE(WIFEXITED(status));
            EXPECT_FALSE(WIFSTOPPED(status));
            kill(pid, SIGCONT);
            break;
    }
}
