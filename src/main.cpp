#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <fmt/core.h>
#include <fmt/chrono.h>

#include <database/database.h>
#include <database/item/item_database.h>
#include <event/event_pool.h>
#include <server/http.h>
#include <server/server.h>
#include <server/server_pool.h>

using namespace GTServer;
std::shared_ptr<Database> g_database;
std::shared_ptr<ItemDatabase> g_items;
std::shared_ptr<ServerPool> g_servers;
std::shared_ptr<EventPool> g_events;

int main() {
    fmt::print("starting GTServer version 0.0.2\n");
#ifdef HTTP_SERVER
    auto http_server{ std::make_unique<HTTPServer>(
        std::string{ constants::http::address.begin(), constants::http::address.end() }, 
        constants::http::port
    ) };
    if (!http_server->listen())
        fmt::print("failed to starting http server, please run an external http service.\n");
#endif
    g_database = std::make_shared<Database>(
        Database::Settings {
            constants::mysql::host,
            constants::mysql::username,
            constants::mysql::password,
            constants::mysql::schema
        }
    );

    g_items = std::make_shared<ItemDatabase>();
    if (!g_items->serialize())
        fmt::print(" - failed to serialization items.dat\n");
    else
        fmt::print(" - items.dat -> {} items loaded with hash {}\n", g_items->get_items().size(), g_items->get_hash());

    g_events = std::make_shared<EventPool>();
    g_events->load_events();

    g_servers = std::make_shared<ServerPool>(g_events, g_database);
    if (!g_servers->initialize_enet()) {
        fmt::print("failed to initialize enet, shutting down the server.\n");
        return EXIT_FAILURE;
    }
    
    std::shared_ptr<Server> server{ g_servers->start_instance() };
    if (!server->start()) {
        fmt::print("failed to start enet server -> {}:{}", server->get_host().first, server->get_host().second);
        return EXIT_FAILURE;
    }
    server->set_component(g_events, g_database);
    server->start_service();
    while(true);
}