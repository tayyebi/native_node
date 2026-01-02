#include <iostream>
#include "sandbox/cgroups.h"

int main() {
    std::cout << "cgroups_test: starting" << std::endl;
    if (!sandbox::is_cgroup_v2_available()) {
        std::cout << "cgroup v2 not available; skipping test" << std::endl;
        return 0;
    }

    const std::string name = "native_node_test_12345";
    std::string path = sandbox::create_transient_cgroup(name);
    if (path.empty()) {
        std::cerr << "failed to create transient cgroup (maybe insufficient privileges)" << std::endl;
        return 2;
    }

    if (!sandbox::add_pid_to_cgroup(path)) {
        std::cerr << "failed to add pid to cgroup" << std::endl;
        sandbox::remove_transient_cgroup(path);
        return 2;
    }

    if (!sandbox::remove_transient_cgroup(path)) {
        std::cerr << "failed to remove cgroup (may need cleanup)" << std::endl;
        return 2;
    }

    std::cout << "cgroups_test: succeeded" << std::endl;
    return 0;
}
