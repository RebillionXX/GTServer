#include <future>
#include <server/http.h>

namespace GTServer {
    http_server::http_server(const http_server::configuration& config) : m_config(config) {
        m_server = std::make_unique<httplib::SSLServer>("./cache/cert.pem", "./cache/key.pem");
    }
    http_server::~http_server() {
        this->stop();
    }

    bool http_server::listen() {
        return false;
    }
    void http_server::stop() {

    }
}