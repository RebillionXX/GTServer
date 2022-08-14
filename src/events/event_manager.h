#ifndef EVENTS__EVENT_MANAGER_H
#define EVENTS__EVENT_MANAGER_H

#include <functional>
#include <unordered_map>
#include <player/player.h>
#include <server/server.h>
#include <proton/packet.h>
namespace GTServer {
    class Database;
    class event_manager {
    public:
        enum class text_event {
            TEXT,
            ACTION
        };

        struct context {
            std::shared_ptr<Player> m_player;
            Server* m_server;
            event_manager* m_event_manager;
            Database* m_database;

            text_scanner m_parser;
        };
    public:
        event_manager();
        ~event_manager();

        void load_events();

        void register_event(const std::pair<std::string, text_event> data, std::function<void(event_manager::context&)> fn);
        void register_event(const uint8_t& data, std::function<void(event_manager::context&)> fn);

        bool call(const std::pair<std::string, text_event> data, event_manager::context& ctx);

        [[nodiscard]] size_t get_action_events() const { return m_action_event.size(); }
        [[nodiscard]] size_t get_text_events() const { return m_text_events.size(); }
        [[nodiscard]] size_t get_packet_events() const { return m_packet_events.size(); }
    private:
        using text_event_value = std::unordered_map<uint64_t, std::function<void(event_manager::context&)>>; 
        using packet_event_value = std::unordered_map<uint8_t, std::function<void(event_manager::context&)>>;

        text_event_value m_action_event;
        text_event_value m_text_events;
        packet_event_value m_packet_events;
    };
}

#endif // EVENTS__EVENT_MANAGER_H