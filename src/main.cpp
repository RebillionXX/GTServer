#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <fmt/core.h>
#include <fmt/chrono.h>

#include <server/server.h>
#include <server/server_pool.h>

server_pool* g_servers;

void exit_handler(int sig) {
    for (auto& pair : g_servers->get_servers())
        g_servers->stop_instance(pair.first);
    exit(EXIT_SUCCESS);
}

int main() {
    fmt::print("starting GTServer version 0.0.1\n");
    signal (SIGINT, exit_handler);

    fmt::print("initializing server - {}\n", std::chrono::system_clock::now());
    g_servers = new server_pool();
    ENetServer* server = g_servers->start_instance();
    if (!server->start()) {
        fmt::print("failed to start enet server -> {}:{}", server->get_host().first, server->get_host().second);
        return EXIT_FAILURE;
    }
    server->start_service();
}