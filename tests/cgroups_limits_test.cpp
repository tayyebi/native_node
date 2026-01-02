#include <iostream>
#include "sandbox/cgroups.h"

int main() {
    std::cout << "cgroups_limits_test: starting" << std::endl;
    if (!sandbox::is_cgroup_v2_available()) {
        std::cout << "cgroup v2 not available; skipping test" << std::endl;
        return 0;
    }

    const std::string name = "native_node_limits_test_12345";
    std::string path = sandbox::create_transient_cgroup(name);
    if (path.empty()) {
        std::cerr << "failed to create transient cgroup" << std::endl;
        return 2;
    }

    // Set limits
    if (!sandbox::set_cgroup_cpu_max(path, "100000 100000")) {
        std::cerr << "failed to set cpu.max" << std::endl;
        sandbox::remove_transient_cgroup(path);
        return 2;
    }

    if (!sandbox::set_cgroup_memory_max(path, "33554432")) { // 32MB
        std::cerr << "failed to set memory.max" << std::endl;
        sandbox::remove_transient_cgroup(path);
        return 2;
    }

    if (!sandbox::set_cgroup_pids_max(path, "10")) {
        std::cerr << "failed to set pids.max" << std::endl;
        sandbox::remove_transient_cgroup(path);
        return 2;
    }

    // Read back and verify (best-effort)
    auto cpu = sandbox::read_cgroup_file(path + "/cpu.max");
    auto mem = sandbox::read_cgroup_file(path + "/memory.max");
    auto pids = sandbox::read_cgroup_file(path + "/pids.max");

    std::cout << "cpu.max='" << cpu << "' memory.max='" << mem << "' pids.max='" << pids << "'" << std::endl;

    if (cpu.empty() || mem.empty() || pids.empty()) {
        std::cerr << "failed to read back one or more limits" << std::endl;
        sandbox::remove_transient_cgroup(path);
        return 2;
    }

    // Cleanup
    if (!sandbox::remove_transient_cgroup(path)) {
        std::cerr << "failed to remove cgroup" << std::endl;
        return 2;
    }

    std::cout << "cgroups_limits_test: succeeded" << std::endl;
    return 0;
}
