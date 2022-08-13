#ifndef SERVER__HTTP_H
#define SERVER__HTTP_H
#include <string>
#include <iostream>
#include <httplib.h>

namespace GTServer {
    class http_server {
    public:
        typedef struct {
            std::string m_host;
            uint16_t m_port;
        } configuration;
    public:
        http_server(const http_server::configuration&);
        ~http_server();

        [[nodiscard]] http_server::configuration get() const { return this->m_config; }
        bool listen();
        void stop();
    private:
        std::unique_ptr<httplib::SSLServer> m_server;
        http_server::configuration m_config;
    };
}

#endif // SERVER__HTTP_H