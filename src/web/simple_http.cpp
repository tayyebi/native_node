#include "simple_http.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <atomic>
#include <unordered_map>

static std::atomic<bool> server_running{false};
static int server_fd = -1;
static std::thread server_thread;
static std::string g_web_root;
static time_t g_start_time = 0;

static std::string read_file(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs)
        return {};
    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

static void handle_client(int client) {
    constexpr size_t BUF = 8192;
    char buf[BUF];
    ssize_t n = recv(client, buf, BUF - 1, 0);
    if (n <= 0) { close(client); return; }
    buf[n] = '\0';
    std::string req(buf);

    // Very naive parsing
    std::istringstream rs(req);
    std::string method, path, proto;
    rs >> method >> path >> proto;

    if (path == "/" ) path = "/index.html";

    // API endpoints
    if (path == "/api/status") {
        time_t now = time(nullptr);
        long uptime = g_start_time ? (long)(now - g_start_time) : 0;
        // Query subsystem health
        bool engine_ok = false;
        bool services_ok = false;
        // Lazy-query via external symbols (nullable if not linked yet)
        extern bool engine::is_initialized();
        extern bool services::is_initialized();
        engine_ok = engine::is_initialized();
        services_ok = services::is_initialized();

        std::string body = "{\"status\":\"ok\", \"uptime\": " + std::to_string(uptime) +
                           ", \"engine\": \"" + (engine_ok ? "ok" : "down") +
                           "\", \"services\": \"" + (services_ok ? "ok" : "down") + "\" }";
        std::string resp = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(body.size()) + "\r\nContent-Type: application/json\r\n\r\n" + body;
        send(client, resp.c_str(), resp.size(), 0);
        close(client);
        return;
    }

    std::string full = g_web_root + path;
    std::string body = read_file(full);
    if (body.empty()) {
        std::string resp = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        send(client, resp.c_str(), resp.size(), 0);
        close(client);
        return;
    }

    std::string resp = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(body.size()) + "\r\nContent-Type: text/html\r\n\r\n" + body;
    send(client, resp.c_str(), resp.size(), 0);
    close(client);
}

static void server_loop(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); close(server_fd); server_fd = -1; return;
    }

    if (listen(server_fd, 10) < 0) { perror("listen"); close(server_fd); server_fd = -1; return; }

    server_running = true;
    while (server_running) {
        sockaddr_in client_addr{}; socklen_t len = sizeof(client_addr);
        int client = accept(server_fd, (sockaddr*)&client_addr, &len);
        if (client < 0) continue;
        std::thread t(handle_client, client);
        t.detach();
    }

    if (server_fd >= 0) { close(server_fd); server_fd = -1; }
}

namespace web::http {

bool start(const std::string& web_root, int port) {
    if (server_running) return false;
    g_web_root = web_root;
    g_start_time = time(nullptr);
    server_thread = std::thread(server_loop, port);
    // give it a moment
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return true;
}

void stop() {
    server_running = false;
    if (server_fd >= 0) shutdown(server_fd, SHUT_RDWR);
    if (server_thread.joinable()) server_thread.join();
}

} // namespace web::http
