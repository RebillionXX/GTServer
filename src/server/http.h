#ifndef SERVER__HTTP_H
#define SERVER__HTTP_H
#include <future>
#include <string>
#include <iostream>
#include <httplib.h>

namespace GTServer {
    class http_server {
    public:
        explicit http_server(const std::string&, const uint16_t&);
        ~http_server();

        bool listen();
        void stop();
        void thread();

        bool bind_to_port(const std::pair<std::string, uint16_t>& val) {
            return m_server->bind_to_port(val.first.c_str(), val.second);
        }
        
    private:
        std::unique_ptr<httplib::SSLServer> m_server{};
        std::pair<std::string, uint16_t> m_config{};
    };
}

#endif // SERVER__HTTP_H