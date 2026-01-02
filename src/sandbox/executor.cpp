#include "executor.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <vector>

namespace sandbox {

static char** make_argv(const std::vector<std::string>& args) {
    char** argv = new char*[args.size() + 1];
    for (size_t i = 0; i < args.size(); ++i) {
        argv[i] = strdup(args[i].c_str());
    }
    argv[args.size()] = nullptr;
    return argv;
}

static void free_argv(char** argv, size_t n) {
    for (size_t i = 0; i < n; ++i) free(argv[i]);
    delete[] argv;
}

ExecResult run_command_in_cgroup(const std::vector<std::string>& args, const CgroupLimits& limits, int timeout_sec) {
    ExecResult res;
    if (args.empty()) return res;

    InvocationCgroup ig(limits);
    if (!ig.valid()) {
        std::cerr << "[executor] failed to create invocation cgroup" << std::endl;
        return res;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "[executor] fork failed: " << strerror(errno) << std::endl;
        return res;
    }

    if (pid == 0) {
        // Child: exec
        char** argv = make_argv(args);
        execv(argv[0], argv);
        // If execv returns, error
        _exit(127);
    }

    // Parent: add child pid to cgroup
    if (!ig.add_pid(pid)) {
        std::cerr << "[executor] failed to add child pid to cgroup" << std::endl;
        // continue: try to wait for child
    }

    int status = 0;
    int waited = 0;
    while (waited < timeout_sec) {
        pid_t w = waitpid(pid, &status, WNOHANG);
        if (w == pid) break;
        if (w == 0) {
            sleep(1);
            ++waited;
            continue;
        }
        if (w < 0) {
            if (errno == EINTR) continue;
            break;
        }
    }

    pid_t w = waitpid(pid, &status, WNOHANG);
    if (w == 0) {
        // timed out; kill child
        kill(pid, SIGKILL);
        waitpid(pid, &status, 0);
        res.success = false;
        res.exit_code = -1;
        return res;
    }

    if (WIFEXITED(status)) {
        res.exit_code = WEXITSTATUS(status);
        res.success = true;
    } else if (WIFSIGNALED(status)) {
        res.term_signal = WTERMSIG(status);
        res.success = false;
    }

    return res;
}

} // namespace sandbox
