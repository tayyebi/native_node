#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>
#include <sqlite3.h>
#include "services/sqlite_pool.h"

int main() {
    const std::string dbpath = "/tmp/native_node_sqlite_pool_test.db";
    // Remove any existing db
    std::remove(dbpath.c_str());

    services::SQLiteConnectionPool pool(dbpath, 4);
    if (!pool.initialize()) {
        std::cerr << "failed to init pool" << std::endl;
        return 2;
    }

    // Create a table using one connection
    {
        sqlite3* c = pool.acquire();
        char* err = nullptr;
        int rc = sqlite3_exec(c, "CREATE TABLE kv(k TEXT PRIMARY KEY, v TEXT);", nullptr, nullptr, &err);
        if (rc != SQLITE_OK) {
            std::cerr << "create table failed: " << (err?err:"") << std::endl;
            if (err) sqlite3_free(err);
            return 2;
        }
        pool.release(c);
    }

    // Launch concurrent writers
    auto writer = [&](int id){
        for (int i = 0; i < 50; ++i) {
            sqlite3* c = pool.acquire();
            std::string sql = "INSERT OR REPLACE INTO kv(k,v) VALUES('k" + std::to_string(id) + "_" + std::to_string(i) + "','v');";
            char* err = nullptr;
            int rc = sqlite3_exec(c, sql.c_str(), nullptr, nullptr, &err);
            if (rc != SQLITE_OK) {
                std::cerr << "insert failed: " << (err?err:"") << std::endl;
                if (err) sqlite3_free(err);
            }
            pool.release(c);
        }
    };

    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t) threads.emplace_back(writer, t);
    for (auto& th : threads) th.join();

    // Verify some reads
    {
        sqlite3* c = pool.acquire();
        sqlite3_stmt* stmt = nullptr;
        int rc = sqlite3_prepare_v2(c, "SELECT COUNT(*) FROM kv;", -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "prepare failed: " << sqlite3_errstr(rc) << std::endl;
            pool.release(c);
            return 2;
        }
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            std::cout << "row count: " << count << std::endl;
            if (count <= 0) {
                std::cerr << "unexpected row count" << std::endl;
                sqlite3_finalize(stmt);
                pool.release(c);
                return 2;
            }
        }
        sqlite3_finalize(stmt);
        pool.release(c);
    }

    pool.shutdown();
    std::cout << "sqlite_pool_test: succeeded" << std::endl;
    return 0;
}
