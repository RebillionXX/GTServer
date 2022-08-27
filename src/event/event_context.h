#pragma once
#include <functional>
#include <player/player.h>
#include <server/server.h>
#include <database/item/item_database.h>

namespace GTServer {
    class EventPool;
    enum EventType : uint8_t {
        EVENT_TYPE_GENERIC_TEXT,
        EVENT_TYPE_ACTION,
        EVENT_TYPE_GAME_PACKET,
        TOTAL_EVENTS // used for EventQueue
    };
    struct EventContext {
        Server* m_server;
        std::shared_ptr<Player> m_player;
        std::shared_ptr<Database> m_database;
        std::shared_ptr<ItemDatabase> m_items;
        std::shared_ptr<EventPool> m_events;
        TextParse m_parser;
    };
}