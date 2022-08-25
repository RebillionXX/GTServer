#include <algorithm>
#include <event/event_pool.h>
#include <fmt/core.h>
#include <utils/text.h>

#include <event/text_events/action.h>
#include <event/text_events/requested_name.h>

#include <event/text_events/action/dialog_return.h>
#include <event/text_events/action/enter_game.h>
#include <event/text_events/action/quit.h>
#include <event/text_events/action/refresh_item_data.h>
#include <event/text_events/action/refresh_player_tribute_data.h>

namespace GTServer {
    EventPool::EventPool() {
        fmt::print("Initializing EventPool\n");
    }
    EventPool::~EventPool() {
        this->unload_events();
    }

    void EventPool::load_events() {
        reg_generic("requestedName", events::requested_name);
        reg_generic("action", events::action);

        reg_action("dialog_return", events::dialog_return);
        reg_action("enter_game", events::enter_game);
        reg_action("quit", events::quit);
        reg_action("refresh_item_data", events::refresh_item_data);
        reg_action("refresh_player_tribute_data", events::refresh_player_tribute_data);

        fmt::print(" - registered events, {} genetric texts {} actions {} game packets\n",
            this->get_registered_event(EVENT_TYPE_GENERIC_TEXT),
            this->get_registered_event(EVENT_TYPE_ACTION),
            this->get_registered_event(EVENT_TYPE_GAME_PACKET));
    }
    void EventPool::unload_events() {
        m_events.clear();
    }

    void EventPool::reg_generic(const std::string& ev, std::function<void(EventContext&)> fn) {
        m_events[EVENT_TYPE_GENERIC_TEXT].push_back({ static_cast<uint32_t>(utils::quick_hash(ev)), fn });
    }
    void EventPool::reg_action(const std::string& ev, std::function<void(EventContext&)> fn) {
         m_events[EVENT_TYPE_ACTION].push_back({ static_cast<uint32_t>(utils::quick_hash(ev)), fn });
    }
    void EventPool::reg_packet(const uint8_t& ev, std::function<void(EventContext&)> fn) {
         m_events[EVENT_TYPE_GAME_PACKET].push_back({ static_cast<uint32_t>(ev), fn });
    }

    std::size_t EventPool::get_registered_event(const EventType& type) const {
        if (auto it = m_events.find(type); it == m_events.end())
            return std::size_t{ 0 };
        return m_events.at(type).size();
    }
}