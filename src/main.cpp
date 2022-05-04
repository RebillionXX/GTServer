#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <httplib.h>

#include <constants.h>
#include <server/server.h>
#include <server/server_pool.h>

server_pool* g_servers;
std::vector<std::thread*> g_threads;
httplib::Server g_http{};

void exit_handler(int sig) {
    for (auto& pair : g_servers->get_servers())
        g_servers->stop_instance(pair.first);
    g_http.stop();
    enet_deinitialize();
    exit(EXIT_SUCCESS);
}

int main() {
    fmt::print("starting GTServer version 0.0.1\n");
    signal (SIGINT, exit_handler);
    fmt::print("initializing threads\n");
    g_threads.push_back(new std::thread([&]() {
        g_http.Post("/growtopia/server_data.php", [&](const httplib::Request &req, httplib::Response &res) {
            if (req.body.empty() ||
            req.body.find("version") == std::string::npos ||
            req.body.find("platform") == std::string::npos ||
            req.body.find("protocol") == std::string::npos) {
                res.set_content("my man, please stop reading my server_data.php", "text/html");
                return false;
            }
            res.set_content(fmt::format(
                "server|{}\n"
                "port|{}\n"
                "type|1\n"
                "#maint|Server is under maintenance. We will be back online shortly. Thank you for your patience!\n"
                "meta|TOLOL\n"
                "RTENDMARKERBS1001",
                constants::http::address.data(),
                constants::http::port),
            "text/html");
            return true;
        });
        fmt::print("http server listening to 127.0.0.1:80, server -> {}:{}\n", constants::http::address.data(), constants::http::port);
        g_http.listen("127.0.0.1", 80);
    }));

    fmt::print("initializing server pool\n");
    if (enet_initialize() != 0) {
        fmt::print("failed to initialize enet service\n");
        return EXIT_FAILURE;
    }
    g_servers = new server_pool();
    ENetServer* server = g_servers->start_instance();
    if (!server->start()) {
        fmt::print("failed to start enet server -> {}:{}", server->get_host().first, server->get_host().second);
        return EXIT_FAILURE;
    }

    for (const auto& thread : g_threads)
        thread->detach();
    server->start_service();
    while(true);
}