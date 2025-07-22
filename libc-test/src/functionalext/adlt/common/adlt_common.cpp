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
#include "adlt_common.h"

std::string read_pipe(int pipe_id) {
    std::string str;
    char buf[1024];
    int ret;
    while ((ret = read(pipe_id, buf, sizeof(buf))) > 0) {
        str.append(buf, ret);
    }
    return str;
}

int wait_process(int pid, int expect_ret) {
    int wstatus;
    pid_t w = waitpid(pid, &wstatus, 0);
    if (w == -1) {
        t_error("%s: wait for child process failed\n", __func__);
        return errno;
    }

    int err = WEXITSTATUS(wstatus);
    if (WIFEXITED(wstatus)) {
        if (err != expect_ret) {
            printf("%s: exited with status=%d\n", __func__, err);
        }
    } else if (WIFSIGNALED(wstatus)) {
        printf("%s: killed by signal %d\n", __func__, WTERMSIG(wstatus));
    } else if (WIFSTOPPED(wstatus)) {
        printf("%s: stopped by signal %d\n", __func__, WSTOPSIG(wstatus));
    } else {
        printf("%s: stopped by signal %d\n", __func__, WSTOPSIG(wstatus));
    }

    return err;
}

int run_subprocess(std::function<void()> func, int &child_pid) {
    pid_t pid = fork();
    if (pid < 0) {
        t_error("%s: failed to fork child process %d\n", __func__, errno);
        return errno;
    }

    // child process
    if (pid == 0) {
        func();
        int ret = errno;
        exit(ret);
    }

    // parent process
    child_pid = pid;
    return wait_process(pid);
}

int run_subprocess(std::function<void()> func, int &child_pid, const std::string &file) {
    pid_t pid = fork();
    if (pid < 0) {
        t_error("%s: failed to fork child process %d\n", __func__, errno);
        return errno;
    }

    // child process
    if (pid == 0) {
        int fd = open((file + "." + std::to_string(getpid())).c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
        if (fd < 0) {
            int ret = errno;
            exit(ret);
        }

        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        func();

        fflush(stdout);
        fflush(stderr);

        int ret = errno;
        exit(ret);
    }

    // parent process
    child_pid = pid;
    return wait_process(pid);
}

int run_self_command(const std::string &args, const std::string &env) {
    char *self_command_path = realpath("/proc/self/exe", nullptr);
    if (!self_command_path) {
        t_error("%s: failed to get self command path\n", __func__);
        return -1;
    }

    std::stringstream ss;
    ss << env << ' ' << self_command_path << ' ' << args;
    free(self_command_path);

    return system(ss.str().c_str());
}