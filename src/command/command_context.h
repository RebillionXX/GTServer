#pragma once
#include <functional>
#include <string>
#include <player/player.h>
#include <server/server.h>

namespace GTServer {
    class ServerPool;
    class Database;
    struct CommandContext {
        std::shared_ptr<Player> m_player;
        std::shared_ptr<Server> m_server;
        ServerPool* m_servers;

        std::vector<std::string> m_arguments; 
    };
}