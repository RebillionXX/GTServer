#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <fmt/core.h>
#include <fmt/chrono.h>

#include <database/database.h>
#include <database/item/item_database.h>
#include <events/event_manager.h>
#include <server/http.h>
#include <server/server.h>
#include <server/server_pool.h>

using namespace GTServer;
database* g_database;
server_pool* g_servers;
event_manager* g_event_manager;

int main() {
    fmt::print("starting GTServer version 0.0.2\n");
#ifdef HTTP_SERVER
    auto http_server{ std::make_unique<GTServer::http_server>(
        std::string{ constants::http::address.begin(), constants::http::address.end() }, 
        constants::http::port
    ) };
    if (!http_server->listen())
        fmt::print("failed to starting http server, please run an external http service.\n");
#endif
    g_servers = new server_pool();
    if (!g_servers->initialize_enet()) {
        fmt::print("failed to initialize enet, shutting down the server.\n");
        return EXIT_FAILURE;
    }

    g_database = new database(
    {
        constants::mysql::host,
        constants::mysql::username,
        constants::mysql::password,
        constants::mysql::schema
    });
    
    if (g_database->serialize_server_data(g_servers))
        fmt::print("   |-> server_data: [(user_identifier: {})]\n", g_servers->get_user_id(false));
    fmt::print(" - items.dat serialization -> {}\n", item_database::instance().init() ? "succeed" : "failed");

    fmt::print("initializing events manager\n"); {
        g_event_manager = new event_manager();
        g_event_manager->load_events();
        fmt::print(" - {} text events | {} action events | {} game packet events\n", g_event_manager->get_text_events(), g_event_manager->get_action_events(), g_event_manager->get_packet_events());
    }

    fmt::print("initializing server & starting threads\n"); {
        server* server = g_servers->start_instance();
        if (!server->start()) {
            fmt::print("failed to start enet server -> {}:{}", server->get_host().first, server->get_host().second);
            return EXIT_FAILURE;
        }
        server->set_component(g_event_manager, g_database);
        server->start_service();
    }
    while(true);
}