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
#ifndef _LIBC_TEST_FUNCTIONALEXT_ADLT_H_
#define _LIBC_TEST_FUNCTIONALEXT_ADLT_H_

#include <cstdio>
#include <cstdlib>

#include <string>
#include <functional>
#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "functionalext.h"

#define EXEC_STATEMENT(pipe_out, statement) { \
    close((pipe_out)[0]);                                                                           \
    dup2((pipe_out)[1], STDOUT_FILENO);                                                             \
    dup2((pipe_out)[1], STDERR_FILENO);                                                             \
    close((pipe_out)[1]);                                                                           \
    (statement);                                                                                    \
    fflush(stdout);                                                                                 \
    fflush(stderr);                                                                                 \
    exit(errno);                                                                                    \
}

#define WAIT_AND_CMP_OUTPUT(func, pipe_out, expect_ret, expect_output) { \
    close((pipe_out)[1]);                                                                           \
    std::string str = read_pipe((pipe_out)[0]);                                                     \
    close((pipe_out)[0]);                                                                           \
    wait_process(pid, expect_ret);                                                                  \
    if (memcmp((expect_output), str.c_str(), str.size())) {                                         \
        t_error("%s: output mismatch: (%s) (%s)\n", (func), (expect_output), str.c_str());          \
    }                                                                                               \
}

#define EXPECT_EXIT(func, statement, expect_ret, expect_output) { \
    int pipe_out[2];                                                                                \
    if (pipe(pipe_out) < 0) { t_error("%s: Failed create pipe\n", (func)); }                        \
    else {                                                                                          \
        pid_t pid = fork();                                                                         \
        if (pid == 0) { EXEC_STATEMENT(pipe_out, statement); }                                      \
        else if (pid > 0) { WAIT_AND_CMP_OUTPUT(func, pipe_out, expect_ret, expect_output); }       \
        else { t_error("%s: failed to fork child process\n", (func)); }                             \
    }                                                                                               \
}

std::string read_pipe(int pipe_id);
int wait_process(int pid, int expect_ret = 0);
int run_subprocess(std::function<void()> func, int &child_pid);
int run_subprocess(std::function<void()> func, int &child_pid, const std::string &file);
int run_self_command(const std::string &args, const std::string &env = "");

#endif