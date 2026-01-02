// Minimal JIT bootstrap stub for Cling/LLVM integration
#include "engine.h"
#include <iostream>

namespace engine {

bool jit_smoke_test() {
#ifdef USE_CLING
    std::cout << "[engine/jit] Cling backend enabled (stub)\n";
    // Real implementation will initialize Cling's interpreter here.
    return true;
#else
    std::cout << "[engine/jit] Cling backend not enabled; skipping smoke test\n";
    return true;
#endif
}

} // namespace engine
