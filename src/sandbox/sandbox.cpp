#include "sandbox.h"
#include <iostream>

namespace sandbox {

bool apply_default_policy() {
    std::cout << "[sandbox] applying default Landlock/seccomp policy (requires kernel >= 5.13)" << std::endl;
    if (!is_landlock_available()) {
        std::cerr << "[sandbox] Landlock not available on this kernel; aborting (Landlock required)" << std::endl;
        return false;
    }
    // TODO: implement ruleset builder and seccomp loader
    std::cout << "[sandbox] Landlock is available (stub)" << std::endl;
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
