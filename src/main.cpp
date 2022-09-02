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
#include <command/command_manager.h>

using namespace GTServer;
std::shared_ptr<ServerPool> g_servers;
std::shared_ptr<EventPool> g_events;

int main() {
    fmt::print("starting {} V{}\n", SERVER_NAME, SERVER_VERSION);
    fmt::print(" - {}\n", std::chrono::system_clock::now());
#ifdef HTTP_SERVER
    auto http_server{ std::make_unique<HTTPServer>(
        std::string{ config::http::address.begin(), config::http::address.end() }, 
        config::http::port
    ) };
    if (!http_server->listen())
        fmt::print("failed to starting http server, please run an external http service.\n");
#endif

    Database& database{ Database::get() };
    if (!database.connect())
        fmt::print(" - failed to connect MySQL server, please check server configuration.\n");

    PlayerTribute& player_tribute{ PlayerTribute::get() }  ;
    if (!player_tribute.build())
        fmt::print(" - failed to build PlayerTribute\n");
    else 
        fmt::print(" - PlayerTribute is built with hash {}\n", player_tribute.get_hash());
    ItemDatabase& items{ ItemDatabase::get() };
    if (!items.serialize())
        fmt::print(" - failed to serialization items.dat\n");
    else
        fmt::print(" - items.dat -> {} items loaded with hash {}\n", items.get_items().size(), items.get_hash());
        
    CommandManager& commands{ CommandManager::get() };
    commands.register_commands();

    g_events = std::make_shared<EventPool>();
    g_events->load_events();

    g_servers = std::make_shared<ServerPool>(g_events);
    if (!g_servers->initialize_enet()) {
        fmt::print("failed to initialize enet, shutting down the server.\n");
        return EXIT_FAILURE;
    }
    g_servers->start_service();
   
    while (g_servers->is_running()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return EXIT_SUCCESS;
}