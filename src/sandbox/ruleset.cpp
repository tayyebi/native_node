#include "ruleset.h"
#include "sandbox.h"
#include <iostream>

#if defined(__linux__)
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#if !defined(SYS_landlock_create_ruleset)
// SYS_landlock_create_ruleset may not be defined on older headers; define if missing
# if defined(__x86_64__)
#  define SYS_landlock_create_ruleset 444
#  define SYS_landlock_add_rule 445
#  define SYS_landlock_restrict_self 446
# endif
#endif
#include <linux/landlock.h>
#endif

namespace sandbox {

RulesetBuilder::RulesetBuilder() = default;

RulesetBuilder::~RulesetBuilder() {
    if (ruleset_fd_ >= 0) {
        close(ruleset_fd_);
        ruleset_fd_ = -1;
    }
}

void RulesetBuilder::add_path(const std::string& path) {
    paths_.push_back(path);
}

bool RulesetBuilder::create_ruleset() {
#if !defined(__linux__)
    std::cerr << "[sandbox/ruleset] Landlock not supported on non-Linux platforms" << std::endl;
    return false;
#endif

    if (!is_landlock_available()) {
        std::cerr << "[sandbox/ruleset] Landlock not available on this kernel" << std::endl;
        return false;
    }

    // Create a ruleset that handles common FS accesses. This is a minimal skeleton.
    struct landlock_ruleset_attr attr = {};
    attr.handled_access_fs = LANDLOCK_ACCESS_FS_READ_FILE |
                              LANDLOCK_ACCESS_FS_WRITE_FILE |
                              LANDLOCK_ACCESS_FS_EXECUTE |
                              LANDLOCK_ACCESS_FS_READ_DIR |
                              LANDLOCK_ACCESS_FS_REMOVE_FILE;

    // Create ruleset via syscall to avoid depending on glibc wrappers
    long fd = syscall(SYS_landlock_create_ruleset, &attr, sizeof(attr), 0);
    if (fd < 0) {
        std::cerr << "[sandbox/ruleset] failed to create ruleset: " << strerror(errno) << std::endl;
        return false;
    }

    ruleset_fd_ = static_cast<int>(fd);
    return true;
}

bool RulesetBuilder::apply() {
#if !defined(__linux__)
    std::cerr << "[sandbox/ruleset] Landlock apply not supported on non-Linux platforms" << std::endl;
    return false;
#endif

    if (ruleset_fd_ < 0) {
        std::cerr << "[sandbox/ruleset] no ruleset created" << std::endl;
        return false;
    }

    // Apply the ruleset to the current process
    int rc = static_cast<int>(syscall(SYS_landlock_restrict_self, ruleset_fd_, 0));
    if (rc != 0) {
        std::cerr << "[sandbox/ruleset] failed to apply ruleset: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

} // namespace sandbox
