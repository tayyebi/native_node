#include "seccomp.h"
#include "sandbox.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#if defined(__linux__)
#ifdef USE_LIBSECCOMP
#include <seccomp.h>
#include <errno.h>
#endif
#endif

namespace sandbox {

bool load_seccomp_whitelist(const std::string& whitelist_file) {
#if !defined(__linux__)
    std::cerr << "[sandbox/seccomp] seccomp not supported on non-Linux platforms" << std::endl;
    return false;
#endif

    std::ifstream ifs(whitelist_file);
    if (!ifs.is_open()) {
        std::cerr << "[sandbox/seccomp] could not open whitelist file: " << whitelist_file << std::endl;
        return false;
    }

    std::vector<std::string> syscalls;
    std::string line;
    while (std::getline(ifs, line)) {
        // Trim whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        if (line[start] == '#') continue;
        size_t end = line.find_last_not_of(" \t\r\n");
        std::string token = line.substr(start, end - start + 1);
        syscalls.push_back(token);
    }

#ifdef USE_LIBSECCOMP
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ERRNO(EPERM));
    if (!ctx) {
        std::cerr << "[sandbox/seccomp] failed to init seccomp ctx" << std::endl;
        return false;
    }

    for (const auto& s : syscalls) {
        int nr = seccomp_syscall_resolve_name(s.c_str());
        if (nr == __NR_syscall) {
            std::cerr << "[sandbox/seccomp] unknown syscall name: " << s << std::endl;
            seccomp_release(ctx);
            return false;
        }
        if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, nr, 0) < 0) {
            std::cerr << "[sandbox/seccomp] failed to add rule for: " << s << std::endl;
            seccomp_release(ctx);
            return false;
        }
    }

    if (seccomp_load(ctx) < 0) {
        std::cerr << "[sandbox/seccomp] seccomp_load failed" << std::endl;
        seccomp_release(ctx);
        return false;
    }

    seccomp_release(ctx);
    std::cout << "[sandbox/seccomp] seccomp whitelist applied" << std::endl;
    return true;
#else
    std::cerr << "[sandbox/seccomp] libseccomp not available at compile time; cannot apply seccomp" << std::endl;
    return false;
#endif
}

} // namespace sandbox
