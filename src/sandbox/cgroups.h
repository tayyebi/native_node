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

// Resource limits (cgroup v2 controller files)
// Set CPU max (value format as in cgroup v2, e.g. "100000 100000" or "max")
bool set_cgroup_cpu_max(const std::string& cgroup_path, const std::string& cpu_max);

// Set memory max in bytes (or "max" for no limit)
bool set_cgroup_memory_max(const std::string& cgroup_path, const std::string& memory_max);

// Set pids max (integer or "max")
bool set_cgroup_pids_max(const std::string& cgroup_path, const std::string& pids_max);

// Read back controller files for verification
std::string read_cgroup_file(const std::string& path);
} // namespace sandbox
