#pragma once
#include <unordered_map>
#include <proton/packet.h>
#include <event/event_context.h>

namespace GTServer {
    class EventPool {
    public:
        using event_type = 
            std::unordered_map<
                EventType,
                std::list<
                    std::pair<uint64_t, std::function<void(EventContext&)>>
                >
            >;

    public:
        EventPool();
        ~EventPool();

        void load_events();
        void unload_events();

        void reg_generic(const std::string& ev, std::function<void(EventContext&)> fn);
        void reg_action(const std::string& ev, std::function<void(EventContext&)> fn);
        void reg_packet(const uint8_t& ev, std::function<void(EventContext&)> fn);

        std::size_t get_registered_event(const EventType& type) const;
    private:
        event_type m_events;
    };
}