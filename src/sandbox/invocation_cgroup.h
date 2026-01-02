#pragma once

#include <string>
#include <optional>

namespace sandbox {

struct CgroupLimits {
    std::string cpu_max;    // e.g., "100000 100000" or "max"
    std::string memory_max; // bytes or "max"
    std::string pids_max;   // integer or "max"
};

// RAII helper for per-invocation transient cgroups.
// Creates a uniquely named cgroup and applies optional resource limits.
class InvocationCgroup {
public:
    explicit InvocationCgroup(const CgroupLimits& limits);
    ~InvocationCgroup();

    // Non-copyable
    InvocationCgroup(const InvocationCgroup&) = delete;
    InvocationCgroup& operator=(const InvocationCgroup&) = delete;

    // Moveable
    InvocationCgroup(InvocationCgroup&&) noexcept;
    InvocationCgroup& operator=(InvocationCgroup&&) noexcept;

    bool valid() const;
    const std::string& path() const;

    // Add a pid to this invocation cgroup (defaults to current process)
    bool add_pid(pid_t pid = 0);

private:
    std::string path_;
    bool created_ = false;
};

} // namespace sandbox
