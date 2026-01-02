#pragma once

#include <string>

namespace web {

// Start a minimal HTTP server that serves static files from `web_root`
// and exposes a few API endpoints for the admin UI. This is a small
// prototype server for development; later we will integrate Drogon.
bool init_static_server(const std::string& web_root, int port = 8081);
void shutdown();

} // namespace web
