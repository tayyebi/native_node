#include "engine.h"
#include <iostream>
#include <atomic>

namespace engine {

static std::atomic<bool> g_initialized{false};


bool initialize(const std::string& preload_path) {
    std::cout << "[engine] initializing (preload: " << preload_path << ")" << std::endl;
    // Placeholder for JIT initialization (Cling/LLVM)
    g_initialized = true;
    // Run a quick JIT smoke test if JIT is enabled
#ifdef ENGINE_JIT
    if (!jit_smoke_test()) {
        std::cerr << "[engine] JIT smoke test failed" << std::endl;
        return false;
    }
#endif
    return true;
}

void shutdown() {
    std::cout << "[engine] shutdown" << std::endl;
    g_initialized = false;
}

bool is_initialized() {
    return g_initialized.load();
}


} // namespace engine
