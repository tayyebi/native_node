#include "cgroups.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <sstream>

namespace sandbox {

static const std::string CGROUP_ROOT = "/sys/fs/cgroup";

bool is_cgroup_v2_available() {
    struct stat st;
    std::string controllers = CGROUP_ROOT + "/cgroup.controllers";
    if (stat(controllers.c_str(), &st) == 0) return true;
    return false;
}

static bool write_file(const std::string& path, const std::string& data) {
    std::ofstream ofs(path);
    if (!ofs.is_open()) return false;
    ofs << data;
    return !ofs.fail();
}

std::string create_transient_cgroup(const std::string& name) {
    if (!is_cgroup_v2_available()) return "";
    std::string path = CGROUP_ROOT + "/" + name;
    // mkdir
    if (mkdir(path.c_str(), 0755) != 0) {
        if (errno == EEXIST) return path;
        std::cerr << "[cgroups] mkdir failed: " << strerror(errno) << std::endl;
        return "";
    }
    return path;
}

bool add_pid_to_cgroup(const std::string& cgroup_path, pid_t pid) {
    if (pid == 0) pid = getpid();
    std::string procs = cgroup_path + "/cgroup.procs";
    std::ostringstream ss;
    ss << pid << "\n";
    if (!write_file(procs, ss.str())) {
        std::cerr << "[cgroups] failed to write pid to " << procs << std::endl;
        return false;
    }
    return true;
}

bool remove_transient_cgroup(const std::string& cgroup_path) {
    // attempt to rmdir (will fail if not empty)
    if (rmdir(cgroup_path.c_str()) != 0) {
        std::cerr << "[cgroups] rmdir failed: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool set_cgroup_cpu_max(const std::string& cgroup_path, const std::string& cpu_max) {
    std::string file = cgroup_path + "/cpu.max";
    if (!write_file(file, cpu_max + "\n")) {
        std::cerr << "[cgroups] failed to write cpu.max" << std::endl;
        return false;
    }
    return true;
}

bool set_cgroup_memory_max(const std::string& cgroup_path, const std::string& memory_max) {
    std::string file = cgroup_path + "/memory.max";
    if (!write_file(file, memory_max + "\n")) {
        std::cerr << "[cgroups] failed to write memory.max" << std::endl;
        return false;
    }
    return true;
}

bool set_cgroup_pids_max(const std::string& cgroup_path, const std::string& pids_max) {
    std::string file = cgroup_path + "/pids.max";
    if (!write_file(file, pids_max + "\n")) {
        std::cerr << "[cgroups] failed to write pids.max" << std::endl;
        return false;
    }
    return true;
}

std::string read_cgroup_file(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) return std::string();
    std::string content;
    std::getline(ifs, content);
    return content;
}

} // namespace sandbox
