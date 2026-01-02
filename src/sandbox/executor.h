#pragma once

#include <string>
#include <vector>
#include "invocation_cgroup.h"

namespace sandbox {

struct ExecResult {
    int exit_code = -1;
    int term_signal = 0;
    bool success = false;
    std::string output;
};

// Run a command (args[0] executable, args[1..] argv) inside a transient InvocationCgroup
// with the provided limits. Blocks until command exits or timeout (seconds) elapses.
ExecResult run_command_in_cgroup(const std::vector<std::string>& args, const CgroupLimits& limits, int timeout_sec = 30);

} // namespace sandbox
