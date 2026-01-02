// Minimal single-threaded HTTP server to serve files and basic API endpoints.
// This is intentionally tiny and suitable for early prototyping only.
#pragma once

#include <string>

namespace web::http {

bool start(const std::string& web_root, int port = 8081);
void stop();

} // namespace web::http
