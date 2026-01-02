#include "services.h"
#include <iostream>
#include <atomic>

namespace services {

static std::atomic<bool> g_initialized{false};


bool initialize() {
    std::cout << "[services] initializing (SQLite, MailApp, PropertyStore stubs)" << std::endl;
    g_initialized = true;
    return true;
}

void shutdown() {
    std::cout << "[services] shutdown" << std::endl;
    g_initialized = false;
}

bool is_initialized() {
    return g_initialized.load();
}

} // namespace services
