#ifndef EVENTS__EVENT_MANAGER_H
#define EVENTS__EVENT_MANAGER_H

#include <functional>
#include <unordered_map>
#include <player/player.h>
#include <server/server.h>
#include <proton/packet.h>

namespace GTServer {
    class event_manager {
    public:
        struct context {
            NetAvatar* m_local;
            ENetServer* m_server;

            void* m_data;
        };
        event_manager();
        ~event_manager();

        void load_events();

        void register_event(const std::string& data, std::function<void(event_manager::context&)> fn);
        void register_event(const uint8_t& data, std::function<void(event_manager::context&)> fn);

        bool call(const std::string& data, event_manager::context& ctx);

        [[nodiscard]] size_t get_text_events() const { return m_text_events.size(); }
        [[nodiscard]] size_t get_packet_events() const { return m_packet_events.size(); }
    private:
        std::unordered_map<uint64_t, std::function<void(event_manager::context&)>> m_text_events;
        std::unordered_map<uint8_t, std::function<void(event_manager::context&)>> m_packet_events;
    };
}

#endif // EVENTS__EVENT_MANAGER_H