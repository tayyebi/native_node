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

void RulesetBuilder::add_path_with_access(const std::string& path, uint64_t access_flags) {
    // Store as a serialized string: path||' '||access
    paths_.push_back(path + " " + std::to_string(access_flags));
}

bool RulesetBuilder::load_policy(const std::string& policy_file) {
    FILE* f = fopen(policy_file.c_str(), "r");
    if (!f) {
        std::cerr << "[sandbox/ruleset] could not open policy file: " << policy_file << std::endl;
        return false;
    }
    char* line = nullptr;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, f)) != -1) {
        // Trim newline
        while (read > 0 && (line[read-1] == '\n' || line[read-1] == '\r')) {
            line[--read] = '\0';
        }
        std::string s(line);
        // Skip comments and empty lines
        if (s.empty() || s[0] == '#') continue;
        // Expect: <path> <ro|rw>
        size_t sp = s.find_first_of(" \t");
        std::string path = s.substr(0, sp);
        std::string flag = (sp == std::string::npos) ? "ro" : s.substr(sp+1);
        // Normalize flag
        if (flag.find("rw") != std::string::npos) {
            uint64_t access = LANDLOCK_ACCESS_FS_READ_FILE | LANDLOCK_ACCESS_FS_WRITE_FILE |
                LANDLOCK_ACCESS_FS_READ_DIR | LANDLOCK_ACCESS_FS_REMOVE_FILE;
            add_path_with_access(path, access);
        } else {
            // default to read-only
            uint64_t access = LANDLOCK_ACCESS_FS_READ_FILE | LANDLOCK_ACCESS_FS_READ_DIR;
            add_path_with_access(path, access);
        }
    }
    if (line) free(line);
    fclose(f);
    return true;
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

    // If there are per-path entries, add them to the ruleset
    for (const auto& entry : paths_) {
        // entry is either "path" or "path access" where access is numeric
        size_t sp = entry.find_first_of(' ');
        std::string path = (sp == std::string::npos) ? entry : entry.substr(0, sp);
        uint64_t access = 0;
        if (sp != std::string::npos) {
            access = static_cast<uint64_t>(std::stoull(entry.substr(sp+1)));
        } else {
            access = LANDLOCK_ACCESS_FS_READ_FILE | LANDLOCK_ACCESS_FS_READ_DIR;
        }

        // Open path with O_PATH to get fd
        int path_fd = open(path.c_str(), O_PATH | O_CLOEXEC);
        if (path_fd < 0) {
            std::cerr << "[sandbox/ruleset] failed to open path " << path << " : " << strerror(errno) << std::endl;
            continue;
        }

        struct landlock_path_beneath_attr pb = {};
        pb.allowed_access = access;
        pb.parent_fd = path_fd;

        long r = syscall(SYS_landlock_add_rule, ruleset_fd_, LANDLOCK_RULE_PATH_BENEATH, &pb, 0);
        if (r != 0) {
            std::cerr << "[sandbox/ruleset] failed to add rule for " << path << " : " << strerror(errno) << std::endl;
        }

        close(path_fd);
    }
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
