/*
* Copyright (C) 2025 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "adlt_common.h"
#include "a.h"
#include "common.h"

#define PROG "unittest_adlt_ld_musl_so_mem_share_chain_a"
#define EXECPROG "/data/local/tmp/libc-test/unittest_adlt_ld_musl_so_mem_share_chain_b"

// smoke - standalone app
static void test_standalone() {
  fprintf(stderr, "Test 0 - standalone app\n");
  printvar_a(PROG);
  EXPECT_TRUE(__func__, (getvar() == V_OK));
}

// fork
static void test_fork() {
  pid_t child;
  fprintf(stderr, "Test 1 - fork test\n");
  printvar_a(PROG);
  EXPECT_TRUE(__func__, (getvar() == V_OK));
  child = fork();
  EXPECT_TRUE(__func__, (child >= 0));
  if (child) {
    int status;
    waitpid(child, &status, 0);
    printvar_a(PROG "parent");
    EXPECT_TRUE(__func__, (getvar() == V_OK));
    EXPECT_EQ(__func__, WEXITSTATUS(status), 0);
  } else {
    printvar_a(PROG "forked child");
    EXPECT_TRUE(__func__, (getvar() == V_OK));
  }
}

// fork/exec
static void test_fork_exec() {
  pid_t child;
  fprintf(stderr, "Test 2 - fork/exec test\n");
  printvar_a(PROG);
  EXPECT_TRUE(__func__, (getvar() == V_OK));
  child = fork();
  EXPECT_TRUE(__func__, (child >= 0));
  if (child) {
    int status;
    waitpid(child, &status, 0);
    printvar_a(PROG " (parent)");
    EXPECT_TRUE(__func__, (getvar() == V_OK));
    sleep(1);
    EXPECT_EQ(__func__, WEXITSTATUS(status), 0);
  } else {
    char *argv_b[] = {const_cast<char *>(EXECPROG), nullptr};
    int ret;
    fprintf(stderr, "forked child - trying to exec\n");
    ret = execvp(EXECPROG, argv_b);
    if (ret) {
      perror("execve error");
    } else {
      fprintf(stderr, "ret 0 but exited from execve\n");
    }
    EXPECT_TRUE(__func__, 0);
  }
}

static void LdMuslSoMemShareTests_TestChainA(int testno) {
  switch (testno) {
  case 0:
    test_standalone();
    break;
  case 1:
    test_fork();
    break;
  case 2:
    test_fork_exec();
    break;
  default:
    fprintf(stderr, PROG ": unknown test case\n");
  }
}

int main(int argc, char **argv) {
  if (argc > 1) {
    LdMuslSoMemShareTests_TestChainA(atoi(argv[1]));
    return t_status;
  }

  int ret = 0;
  for (int i = 0; i < 2; ++i) {
    ret = ret || run_self_command(std::to_string(i));
  }
  return ret;
}
