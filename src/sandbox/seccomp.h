#pragma once

#include <string>

namespace sandbox {

// Load a seccomp whitelist from the given file (one syscall name per line).
// Returns true on success (filter installed) or false on failure.
bool load_seccomp_whitelist(const std::string& whitelist_file);

} // namespace sandbox
