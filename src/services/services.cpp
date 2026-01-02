#include "services.h"
#include <iostream>
#include <atomic>
#include "sqlite_pool.h"

static std::unique_ptr<services::SQLiteConnectionPool> g_sqlite_pool;

namespace services {

static std::atomic<bool> g_initialized{false};


bool initialize() {
    std::cout << "[services] initializing (SQLite, MailApp, PropertyStore stubs)" << std::endl;
    // Initialize SQLite connection pool (WAL mode)
    g_sqlite_pool = std::make_unique<services::SQLiteConnectionPool>("./data/native_node.db", 4);
    if (!g_sqlite_pool->initialize()) {
        std::cerr << "[services] failed to initialize SQLite pool" << std::endl;
        return false;
    }

    g_initialized = true;
    return true;
}

void shutdown() {
    std::cout << "[services] shutdown" << std::endl;
    if (g_sqlite_pool) {
        g_sqlite_pool->shutdown();
        g_sqlite_pool.reset();
    }
    g_initialized = false;
}

bool is_initialized() {
    return g_initialized.load();
}

} // namespace services
