#pragma once
#include <unordered_map>
#include <proton/packet.h>
#include <utils/text.h>
#include <event/event_context.h>

namespace GTServer {
    class EventPool {
    public:
        using event_list = std::list<std::pair<uint32_t, std::function<void(EventContext&)>>>;
        using event_type = std::unordered_map<EventType, event_list>;

    public:
        EventPool();
        ~EventPool();

        void load_events();
        void unload_events();

        void reg_generic(const std::string& ev, std::function<void(EventContext&)> fn);
        void reg_action(const std::string& ev, std::function<void(EventContext&)> fn);
        void reg_packet(const uint8_t& ev, std::function<void(EventContext&)> fn);

        std::size_t get_registered_event(const EventType& type) const;
        
        template <typename T>
        bool execute(const EventType& type, const T& data, EventContext& ctx) {
            const auto& it = m_events.find(type);
            if (it == m_events.end())
                return false;

            const uint32_t& ev_hash{ utils::quick_hash(std::string{ data }) };
            for (const auto& ev : it->second) {
                if (ev_hash != ev.first)
                    continue;
                ev.second(ctx);
                return true;
            }
            return false;
        }

    private:
        event_type m_events;
    };
}