#include "sandbox.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "seccomp.h"
#include "ruleset.h"
#include <sys/stat.h>

namespace sandbox {

bool apply_default_policy() {
    std::cout << "[sandbox] applying default Landlock/seccomp policy (requires kernel >= 5.13)" << std::endl;
    if (!is_landlock_available()) {
        std::cerr << "[sandbox] Landlock not available on this kernel; aborting (Landlock required)" << std::endl;
        return false;
    }
    // Try to load a simple policy file (config/landlock_policy.conf). If missing, create an empty ruleset.
    sandbox::RulesetBuilder rb;
    const char* policy_path = "config/landlock_policy.conf";
    struct stat st;
    if (stat(policy_path, &st) == 0) {
        std::cout << "[sandbox] loading Landlock policy from: " << policy_path << std::endl;
        if (!rb.load_policy(policy_path)) {
            std::cerr << "[sandbox] failed to load Landlock policy" << std::endl;
            return false;
        }
    } else {
        std::cout << "[sandbox] no Landlock policy file found at '" << policy_path << "'; proceeding with default minimal ruleset" << std::endl;
    }

    if (!rb.create_ruleset()) {
        std::cerr << "[sandbox] failed to create Landlock ruleset" << std::endl;
        return false;
    }

    if (!rb.apply()) {
        std::cerr << "[sandbox] failed to apply Landlock ruleset" << std::endl;
        return false;
    }

    std::cout << "[sandbox] Landlock ruleset applied" << std::endl;
    // Apply seccomp whitelist if present
    const char* seccomp_conf = "config/syscalls.conf";
    if (access(seccomp_conf, R_OK) == 0) {
        std::cout << "[sandbox] loading seccomp whitelist from: " << seccomp_conf << std::endl;
        if (!load_seccomp_whitelist(seccomp_conf)) {
            std::cerr << "[sandbox] failed to apply seccomp whitelist" << std::endl;
            return false;
        }
    } else {
        std::cout << "[sandbox] no seccomp whitelist found at '" << seccomp_conf << "'; skipping seccomp setup" << std::endl;
    }

    return true;
}

void revoke_policy() {
    std::cout << "[sandbox] revoke policy" << std::endl;
}

bool is_landlock_available() {
#if defined(__linux__)
    // Minimal detection: check for presence of /sys/kernel/security/landlock (kernel 5.13+)
    FILE* f = fopen("/sys/kernel/security/landlock", "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
#else
    return false;
#endif
}

} // namespace sandbox
