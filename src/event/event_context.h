#pragma once
#include <functional>
#include <player/player.h>
#include <server/server.h>

namespace GTServer {
    enum EventType : uint8_t {
        EVENT_TYPE_GENERIC_TEXT,
        EVENT_TYPE_ACTION,
        EVENT_TYPE_GAME_PACKET
    };
    struct EventContext {
        Server* m_server;
        std::shared_ptr<Player> m_player;
        std::shared_ptr<Database> m_database;

        text_scanner m_parser;
    };
}