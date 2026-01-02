#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#if defined(HAVE_SQLITE3)
#include <sqlite3.h>

namespace services {

// A simple fixed-size SQLite connection pool that opens multiple connections
// to the same database file and sets WAL mode on each connection.
class SQLiteConnectionPool {
public:
    explicit SQLiteConnectionPool(const std::string& db_path, size_t pool_size = 4);
    ~SQLiteConnectionPool();

    // Initialize the pool (open connections and set PRAGMA journal_mode=WAL)
    bool initialize();
    void shutdown();

    // Acquire a sqlite3* connection from the pool. Blocks until available.
    sqlite3* acquire();
    // Release a connection back to the pool
    void release(sqlite3* conn);

private:
    std::string db_path_;
    size_t pool_size_;
    std::vector<sqlite3*> conns_;
    std::vector<bool> in_use_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool initialized_ = false;
};

// RAII handle that returns connection to pool on destruction
class SQLiteConnHandle {
public:
    SQLiteConnHandle(SQLiteConnectionPool& pool, sqlite3* conn);
    ~SQLiteConnHandle();
    sqlite3* get();
private:
    SQLiteConnectionPool& pool_;
    sqlite3* conn_ = nullptr;
};

} // namespace services
#else
namespace services {
// Stubbed fallback when SQLite3 development headers are not available
class SQLiteConnectionPool {
public:
    explicit SQLiteConnectionPool(const std::string& /*db_path*/, size_t /*pool_size*/ = 4) {}
    ~SQLiteConnectionPool() {}
    bool initialize() { return false; }
    void shutdown() {}
    void* acquire() { return nullptr; }
    void release(void* /*conn*/) {}
};
class SQLiteConnHandle {
public:
    SQLiteConnHandle(SQLiteConnectionPool& /*pool*/, void* /*conn*/) {}
    ~SQLiteConnHandle() {}
    void* get() { return nullptr; }
};
} // namespace services
#endif
