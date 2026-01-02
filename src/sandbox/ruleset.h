#pragma once

#include <string>
#include <vector>

namespace sandbox {

// Minimal Landlock RulesetBuilder skeleton.
// This provides a small, testable API that attempts to create a Landlock ruleset
// and apply it to the current process. Full rule addition is left for later.
class RulesetBuilder {
public:
    RulesetBuilder();
    ~RulesetBuilder();

    // Add a path intended to be allowed (placeholder for future per-path rules)
    void add_path(const std::string& path);
    // Add a path with explicit access flags (LANDLOCK_ACCESS_FS_* flags)
    void add_path_with_access(const std::string& path, uint64_t access_flags);

    // Load a simple policy file (each non-empty line: <path> <ro|rw>)
    // Returns true on successful parse (does not create/apply ruleset)
    bool load_policy(const std::string& policy_file);

    // Create the ruleset (returns true on success)
    bool create_ruleset();

    // Apply the ruleset to the current process (returns true on success)
    bool apply();

private:
    int ruleset_fd_ = -1;
    std::vector<std::string> paths_;
};

} // namespace sandbox
