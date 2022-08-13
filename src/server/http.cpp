#include <future>
#include <fmt/core.h>
#include <server/http.h>
#include <proton/utils/text_scanner.h>

namespace GTServer {
    http_server::http_server(const std::string& host, const uint16_t& port)
        : m_config{ std::make_pair(host, port) }
    {
        m_server = std::make_unique<httplib::SSLServer>("./cache/cert.pem", "./cache/key.pem"); 
    }
    http_server::~http_server() { 
        this->stop(); 
    }

    bool http_server::listen() {
        fmt::print("starting http_server, binding ports.\n");
        if (!this->bind_to_port(m_config)) {
            fmt::print(" - failed to bind http_server's port with {}.\n", m_config.second);
            return false;
        }
        std::thread(&http_server::thread, this).detach();
        return true; 
    }
    void http_server::stop() { 
        m_server->stop();
        m_server.release();
    }

    void http_server::thread() {
        m_server->Post("/growtopia/server_data.php", [&](const httplib::Request& req, httplib::Response& res) {
            if (req.params.empty() || req.get_header_value("User-Agent").find("UbiServices_SDK") == std::string::npos) {
                res.status = 403;
                return;
            }

            text_scanner parser{};
            parser.add("server", "127.0.0.1");
            parser.add<uint16_t>("port", 17091);
            parser.add<int>("type", 1);
            parser.add("#maint", "Server is under maintenance. We will be back online shortly. Thank you for your patience!");
            parser.add<int>("type2", 1);
            parser.add("meta", "DIKHEAD");
            res.set_content(fmt::format("{}\r\nRTENDMARKERBS1001\r\n", parser.get_all_raw()), "text/html");
            return;
        });

        m_server->listen_after_bind();
        while(true);
    }
}