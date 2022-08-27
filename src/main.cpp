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
    fmt::print("starting {} V{}\n", SERVER_NAME, SERVER_VERSION);
#ifdef HTTP_SERVER
    auto http_server{ std::make_unique<HTTPServer>(
        std::string{ config::http::address.begin(), config::http::address.end() }, 
        config::http::port
    ) };
    if (!http_server->listen())
        fmt::print("failed to starting http server, please run an external http service.\n");
#endif
    g_database = std::make_shared<Database>();
    if (!g_database->connect())
        fmt::print(" - failed to connect MySQL server, please check server configuration.\n");
    if (!g_database->get_tribute()->build())
        fmt::print(" - failed to build PlayerTribute\n");
    else 
        fmt::print(" - PlayerTribute is built with hash {}\n", g_database->get_tribute()->get_hash());

    g_items = std::make_shared<ItemDatabase>();
    if (!g_items->serialize())
        fmt::print(" - failed to serialization items.dat\n");
    else
        fmt::print(" - items.dat -> {} items loaded with hash {}\n", g_items->get_items().size(), g_items->get_hash());

    g_events = std::make_shared<EventPool>();
    g_events->load_events();

    g_servers = std::make_shared<ServerPool>(g_events, g_database, g_items);
    if (!g_servers->initialize_enet()) {
        fmt::print("failed to initialize enet, shutting down the server.\n");
        return EXIT_FAILURE;
    }
    
    std::shared_ptr<Server> server{ g_servers->start_instance() };
    server->start_service();
    while(true);
}