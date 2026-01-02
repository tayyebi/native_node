#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include "sandbox/invocation_cgroup.h"
#include "sandbox/cgroups.h"

int main() {
    std::cout << "invocation_cgroup_test: starting" << std::endl;
    if (!sandbox::is_cgroup_v2_available()) {
        std::cout << "cgroup v2 not available; skipping test" << std::endl;
        return 0;
    }

    sandbox::CgroupLimits limits;
    limits.cpu_max = "100000 100000";
    limits.memory_max = "33554432"; // 32MB
    limits.pids_max = "10";

    sandbox::InvocationCgroup ig(limits);
    if (!ig.valid()) {
        std::cerr << "failed to create invocation cgroup (privileges?)" << std::endl;
        return 2;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork failed" << std::endl;
        return 2;
    }
    if (pid == 0) {
        // child: sleep a bit and exit; pause to allow parent to add pid
        sleep(2);
        _exit(0);
    }

    // Parent: add child pid to cgroup
    if (!ig.add_pid(pid)) {
        std::cerr << "failed to add pid to invocation cgroup" << std::endl;
        kill(pid, SIGKILL);
        waitpid(pid, nullptr, 0);
        return 2;
    }

    // Read cgroup.procs and check presence
    std::string procs_path = ig.path() + "/cgroup.procs";
    std::string content = sandbox::read_cgroup_file(procs_path);
    if (content.find(std::to_string(pid)) == std::string::npos) {
        std::cerr << "pid not found in cgroup.procs: '" << content << "'" << std::endl;
        kill(pid, SIGKILL);
        waitpid(pid, nullptr, 0);
        return 2;
    }

    // cleanup: wait for child
    waitpid(pid, nullptr, 0);

    std::cout << "invocation_cgroup_test: succeeded" << std::endl;
    return 0;
}
