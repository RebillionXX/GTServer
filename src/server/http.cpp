#include <future>
#include <fmt/core.h>
#include <config.h>
#include <server/http.h>
#include <proton/utils/text_scanner.h>

namespace GTServer {
    HTTPServer::HTTPServer(const std::string& host, const uint16_t& port)
        : m_config{ std::make_pair(host, port) }
    {
        m_server = std::make_unique<httplib::SSLServer>("./cache/cert.pem", "./cache/key.pem"); 
    }
    HTTPServer::~HTTPServer() { 
        this->stop(); 
    }

    bool HTTPServer::listen() {
        fmt::print("starting HTTPServer, binding ports.\n");
        if (!this->bind_to_port(m_config)) {
            fmt::print(" - failed to bind HTTPServer's port with {}.\n", m_config.second);
            return false;
        }
        std::thread(&HTTPServer::thread, this).detach();
        return true; 
    }
    void HTTPServer::stop() { 
        m_server->stop();
        m_server.release();
    }

    void HTTPServer::thread() {
        m_server->Post("/growtopia/server_data.php", [&](const httplib::Request& req, httplib::Response& res) {
            if (req.params.empty() || req.get_header_value("User-Agent").find("UbiServices_SDK") == std::string::npos) {
                res.status = 403;
                return;
            }
            TextParse parser{};
            parser.add("server", std::string{ config::http::gt::address });
            parser.add<uint16_t>("port", 17091);
            parser.add<int>("type", 1);
            parser.add("#maint", "Server is under maintenance. We will be back online shortly. Thank you for your patience!");
            parser.add("meta", "DIKHEAD");
            res.set_content(std::string{ fmt::format("{}\nRTENDMARKERBS1001\n\n", parser.get_all_raw()) }, "text/html");
            return;
        });

        m_server->listen_after_bind();
        while(true);
    }
}