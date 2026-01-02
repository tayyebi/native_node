#pragma once

#include <string>

namespace engine {

bool initialize(const std::string& preload_path);
void shutdown();
bool is_initialized();
bool jit_smoke_test();

} // namespace engine
