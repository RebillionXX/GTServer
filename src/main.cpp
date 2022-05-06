#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <httplib.h>

#include <constants.h>
#include <database/database.h>
#include <database/item/item_database.h>
#include <events/event_manager.h>
#include <server/server.h>
#include <server/server_pool.h>

using namespace GTServer;
database* g_database;
server_pool* g_servers;
event_manager* g_events;
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
    fmt::print("Starting GTServer version: 0.0.2\n");
    signal (SIGINT, exit_handler);
    fmt::print("Initializing server pool...\n"); {
        if (enet_initialize() != 0) {
            fmt::print("Failed to initialize enet service.\n");
            return EXIT_FAILURE;
        }
        g_servers = new server_pool();
    }
    fmt::print("Initializing database...\n"); {
        g_database = new database();
        fmt::print(" - {} mysql server: {}@{} -> {}\n", g_database->init() ? "Connected to" : "Failed to connect to", constants::mysql::host, constants::mysql::username, constants::mysql::schema);
        if (g_database->serialize_server_data(g_servers))
            fmt::print("   |-> Server_data: [(user_identifier: {})]\n", g_servers->get_user_id(false));
        fmt::print(" - items.dat serialization -> {} -> hash: {}.\n", item_database::instance().init() ? "Succeed" : "Failed", item_database::instance().get_hash());
    } 
    fmt::print("Initializing events manager...\n"); {
        g_events = new event_manager();
        g_events->load_events();
        fmt::print(" - {} text events | {} game packet events registered.\n", g_events->get_text_events(), g_events->get_packet_events());
    }

    fmt::print("Initializing HTTP client...\n"); {
        g_threads.push_back(new std::thread([&]() -> void {
            g_http.Post("/growtopia/server_data.php", [&](const httplib::Request &req, httplib::Response &res) {
                if (req.body.empty() ||
                req.body.find("version") == std::string::npos ||
                req.body.find("platform") == std::string::npos ||
                req.body.find("protocol") == std::string::npos) {
                    res.set_content("https://discord.gg/wHVSrHKbYH", "text/html");
                    return true;
                }
                res.set_content(fmt::format(
                    "server|{}\n"
                    "port|{}\n"
                    "type|1\n"
                    "#maint|Server is under maintenance. We will be back online shortly. Thank you for your patience!\n"
                    "beta_server|beta.growtopiagame.com\n"
                    "beta_port|26999\n"
                    "beta_type|1\n"
                    "beta2_server|beta2.growtopiagame.com\n"
                    "beta2_port|26999\n"
                    "beta2_type|1\n"
                    "type2|1\n"
                    "meta|TLddQ2jYAo\n"
                    "RTENDMARKERBS1001",
                    constants::http::address.data(),
                    constants::http::port),
                "text/html");
                return true;
            });
            fmt::print(" - HTTP server listening to: 0.0.0.0:80, server -> {}:{}\n", constants::http::address.data(), constants::http::port);
            g_http.listen("0.0.0.0", 80);
        }));
    }

    fmt::print("Initializing server & Starting threads...\n"); {
        ENetServer* server = g_servers->start_instance();
        if (!server->start()) {
            fmt::print(" - Failed to start ENet client -> {}:{}", server->get_host().first, server->get_host().second);
            return EXIT_FAILURE;
        }
        server->set_event_manager(g_events);

        for (const auto& thread : g_threads)
            thread->detach();
        server->start_service();
    }
    while(true);
}