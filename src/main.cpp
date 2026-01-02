#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "engine/engine.h"
#include "sandbox/sandbox.h"
#include "services/services.h"
#include "web/static_server.h"
#include <signal.h>
#include <atomic>

int main(int argc, char** argv) {
static volatile sig_atomic_t g_terminate = 0;

static void signal_handler(int) {
    g_terminate = 1;
}

    std::cout << "native_node (Native C++ Scripting Engine) v0.1.0\n";
    std::cout << "Configuration directory: ./config" << std::endl;
    std::cout << "Scripts directory: ./scripts" << std::endl;

    std::cout << "Starting subsystems..." << std::endl;

    // Support a lightweight smoke-test mode for JIT initialization (used by CTest)
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--jit-smoke") {
            std::cout << "Running JIT smoke test..." << std::endl;
            bool ok = engine::jit_smoke_test();
            std::cout << "JIT smoke test " << (ok ? "succeeded" : "failed") << std::endl;
            return ok ? 0 : 2;
        }
    }

    if (!sandbox::apply_default_policy()) {
        std::cerr << "Failed to apply sandbox policy" << std::endl;
        return 1;
    }

    if (!engine::initialize("./scripts")) {
        std::cerr << "Engine initialization failed" << std::endl;
        return 1;
    }

    if (!services::initialize()) {
        std::cerr << "Services initialization failed" << std::endl;
        return 1;
    }

    if (!web::init_static_server("src/web/ui", 8081)) {
        std::cerr << "Failed to initialize web server" << std::endl;
        return 1;
    }

    std::cout << "All subsystems started. Running... (press Ctrl+C to exit)" << std::endl;

    // Install signal handlers for graceful shutdown
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);


    while (!g_terminate) {
    while (true) {
        // TODO: Run event loop, accept requests, dispatch to JIT handlers
        std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Shutdown requested, cleaning up..." << std::endl;
    }

    // Clean shutdown (unreachable until signal handling is added)
    services::shutdown();
    engine::shutdown();
    sandbox::revoke_policy();

    return 0;
}
