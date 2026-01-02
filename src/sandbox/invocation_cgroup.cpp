#include "invocation_cgroup.h"
#include "cgroups.h"
#include <unistd.h>
#include <chrono>
#include <sstream>
#include <random>
#include <iostream>

namespace sandbox {

static std::string make_unique_name() {
    pid_t pid = getpid();
    auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937_64 rng(static_cast<unsigned long>(now ^ pid));
    uint64_t r = rng();
    std::ostringstream ss;
    ss << "native_node_inv_" << pid << "_" << now << "_" << (r & 0xffffffffULL);
    return ss.str();
}

InvocationCgroup::InvocationCgroup(const CgroupLimits& limits) {
    if (!is_cgroup_v2_available()) {
        created_ = false;
        return;
    }

    std::string name = make_unique_name();
    std::string p = create_transient_cgroup(name);
    if (p.empty()) {
        created_ = false;
        return;
    }
    // Apply limits if provided
    if (!limits.cpu_max.empty()) set_cgroup_cpu_max(p, limits.cpu_max);
    if (!limits.memory_max.empty()) set_cgroup_memory_max(p, limits.memory_max);
    if (!limits.pids_max.empty()) set_cgroup_pids_max(p, limits.pids_max);

    path_ = p;
    created_ = true;
}

InvocationCgroup::~InvocationCgroup() {
    if (created_) {
        // best-effort: try to remove
        remove_transient_cgroup(path_);
    }
}

InvocationCgroup::InvocationCgroup(InvocationCgroup&& o) noexcept {
    path_ = std::move(o.path_);
    created_ = o.created_;
    o.created_ = false;
}

InvocationCgroup& InvocationCgroup::operator=(InvocationCgroup&& o) noexcept {
    if (this != &o) {
        if (created_) remove_transient_cgroup(path_);
        path_ = std::move(o.path_);
        created_ = o.created_;
        o.created_ = false;
    }
    return *this;
}

bool InvocationCgroup::valid() const { return created_ && !path_.empty(); }

const std::string& InvocationCgroup::path() const { return path_; }

bool InvocationCgroup::add_pid(pid_t pid) {
    if (!created_) return false;
    return add_pid_to_cgroup(path_, pid);
}

} // namespace sandbox
