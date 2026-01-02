// Minimal JIT bootstrap and smoke-test for ClangREPL/LLVM integration
#include "engine.h"
#include <iostream>
#include <cstdio>
#include <memory>
#include <array>
#include <string>

namespace engine {

bool jit_smoke_test() {
#ifdef USE_CLANGREPL
    std::cout << "[engine/jit] ClangREPL backend requested; checking availability..." << std::endl;
    // Check whether a clang-repl binary is available and responsive
    const char* cmd = "clang-repl --version 2>&1";
    std::array<char, 256> buffer;
    std::string output;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        std::cerr << "[engine/jit] Failed to run clang-repl (not found on PATH)" << std::endl;
        return false;
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        output += buffer.data();
    }
    int rc = pclose(pipe.release());
    if (rc != 0) {
        std::cerr << "[engine/jit] clang-repl returned non-zero status: " << rc << std::endl;
        std::cerr << "[engine/jit] output: " << output << std::endl;
        return false;
    }
    std::cout << "[engine/jit] clang-repl available: " << output << std::endl;
    return true;
#else
    std::cout << "[engine/jit] ClangREPL support not compiled in; skipping smoke test" << std::endl;
    return true;
#endif
}

} // namespace engine
