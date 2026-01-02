#pragma once

#include <string>

namespace sandbox {

// Detect whether cgroup v2 is available on this system (unified hierarchy)
bool is_cgroup_v2_available();

// Create a transient cgroup under the given base (e.g., "/sys/fs/cgroup") with the
// provided name. Returns the full path of the created cgroup on success, empty string on failure.
// Note: Requires appropriate privileges to write to the cgroup filesystem.
std::string create_transient_cgroup(const std::string& name);

// Add the current process (or PID) to the given cgroup path (full path), returns true on success.
bool add_pid_to_cgroup(const std::string& cgroup_path, pid_t pid = 0);

// Remove the transient cgroup; best effort.
bool remove_transient_cgroup(const std::string& cgroup_path);

} // namespace sandbox
