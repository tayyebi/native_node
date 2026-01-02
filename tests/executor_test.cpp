#include <iostream>
#include "sandbox/executor.h"

int main() {
    std::cout << "executor_test: starting" << std::endl;
    sandbox::CgroupLimits limits;
    limits.cpu_max = "max";
    limits.memory_max = "max";
    limits.pids_max = "max";

    // Run /bin/true
    std::vector<std::string> args = {"/bin/true"};
    auto r = sandbox::run_command_in_cgroup(args, limits, 5);
    if (!r.success || r.exit_code != 0) {
        std::cerr << "executor_test: /bin/true failed (exit=" << r.exit_code << ") output='" << r.output << "'" << std::endl;
        return 2;
    }

    // Run /bin/sh -c 'exit 3'
    std::vector<std::string> args2 = {"/bin/sh", "-c", "exit 3"};
    auto r2 = sandbox::run_command_in_cgroup(args2, limits, 5);
    if (!r2.success || r2.exit_code != 3) {
        std::cerr << "executor_test: expected exit 3, got " << r2.exit_code << " output='" << r2.output << "'" << std::endl;
        return 2;
    }

    std::cout << "executor_test: succeeded" << std::endl;
    return 0;
}
