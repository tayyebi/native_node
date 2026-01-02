// Only compile the real implementation when SQLite3 headers are available
#if defined(HAVE_SQLITE3)
#include "sqlite_pool.h"
#include <iostream>
#include <cassert>
#include <thread>

namespace services {

SQLiteConnectionPool::SQLiteConnectionPool(const std::string& db_path, size_t pool_size)
    : db_path_(db_path), pool_size_(pool_size) {}

SQLiteConnectionPool::~SQLiteConnectionPool() {
    shutdown();
}

bool SQLiteConnectionPool::initialize() {
    std::lock_guard<std::mutex> lk(mtx_);
    if (initialized_) return true;

    conns_.resize(pool_size_, nullptr);
    in_use_.assign(pool_size_, false);

    for (size_t i = 0; i < pool_size_; ++i) {
        sqlite3* db = nullptr;
        int rc = sqlite3_open(db_path_.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "[sqlite_pool] failed to open db: " << sqlite3_errstr(rc) << std::endl;
            // close previously opened
            for (auto c : conns_) if (c) sqlite3_close(c);
            return false;
        }

        // Enable WAL mode
        char* errmsg = nullptr;
        rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, &errmsg);
        if (rc != SQLITE_OK) {
            std::cerr << "[sqlite_pool] failed to set WAL mode: " << (errmsg ? errmsg : "") << std::endl;
            if (errmsg) sqlite3_free(errmsg);
            sqlite3_close(db);
            for (auto c : conns_) if (c) sqlite3_close(c);
            return false;
        }

        conns_[i] = db;
    }

    initialized_ = true;
    std::cout << "[sqlite_pool] initialized with " << pool_size_ << " connections (WAL enabled)" << std::endl;
    return true;
}

void SQLiteConnectionPool::shutdown() {
    std::lock_guard<std::mutex> lk(mtx_);
    for (auto c : conns_) {
        if (c) sqlite3_close(c);
    }
    conns_.clear();
    in_use_.clear();
    initialized_ = false;
}

sqlite3* SQLiteConnectionPool::acquire() {
    std::unique_lock<std::mutex> lk(mtx_);
    cv_.wait(lk, [this]() {
        for (bool b : in_use_) if (!b) return true; return false;
    });
    for (size_t i = 0; i < conns_.size(); ++i) {
        if (!in_use_[i]) {
            in_use_[i] = true;
            return conns_[i];
        }
    }
    return nullptr; // should not reach
}

void SQLiteConnectionPool::release(sqlite3* conn) {
    std::lock_guard<std::mutex> lk(mtx_);
    for (size_t i = 0; i < conns_.size(); ++i) {
        if (conns_[i] == conn) {
            in_use_[i] = false;
            cv_.notify_one();
            return;
        }
    }
}

// SQLiteConnHandle
SQLiteConnHandle::SQLiteConnHandle(SQLiteConnectionPool& pool, sqlite3* conn)
    : pool_(pool), conn_(conn) {}

SQLiteConnHandle::~SQLiteConnHandle() {
    if (conn_) pool_.release(conn_);
}

sqlite3* SQLiteConnHandle::get() { return conn_; }

} // namespace services
#endif
