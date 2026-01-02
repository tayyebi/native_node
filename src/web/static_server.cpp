#include "static_server.h"
#include <iostream>
#include "simple_http.h"

namespace web {

bool init_static_server(const std::string& web_root) {
    std::cout << "[web] starting prototype static server (root: " << web_root << ")" << std::endl;
    if (!web::http::start(web_root, 8081)) {
        std::cerr << "[web] failed to start http server" << std::endl;
        return false;
    }
    std::cout << "[web] prototype server listening on http://localhost:8081" << std::endl;
    return true;
}

void shutdown() {
    web::http::stop();
    std::cout << "[web] static server shutdown" << std::endl;
}

} // namespace web
