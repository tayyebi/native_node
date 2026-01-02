#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sys/stat.h>
#include "sandbox/ruleset.h"
#include "sandbox/sandbox.h"

int main() {
    std::cout << "landlock_policy_test: starting" << std::endl;
    if (!sandbox::is_landlock_available()) {
        std::cout << "landlock not available on this kernel; skipping test" << std::endl;
        return 0;
    }

    // Create a temporary directory for testing
    std::string tmpdir = "/tmp/native_node_landlock_test";
    mkdir(tmpdir.c_str(), 0700);

    // Create a simple policy file pointing to the temp dir as read-only
    std::string policy = "/tmp/native_node_landlock_test ro\n";
    std::string policy_file = "/tmp/native_node_landlock_policy.conf";
    std::ofstream ofs(policy_file);
    ofs << policy;
    ofs.close();

    sandbox::RulesetBuilder rb;
    if (!rb.load_policy(policy_file)) {
        std::cerr << "Failed to load policy file" << std::endl;
        return 2;
    }

    if (!rb.create_ruleset()) {
        std::cerr << "create_ruleset failed" << std::endl;
        return 2;
    }

    if (!rb.apply()) {
        std::cerr << "apply() failed" << std::endl;
        return 2;
    }

    std::cout << "landlock_policy_test: succeeded" << std::endl;
    return 0;
}
