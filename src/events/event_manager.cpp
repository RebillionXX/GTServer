#include <events/event_manager.h>
#include <fmt/core.h>
#include <utils/text.h>
#include <events/text_events/action.h>
#include <events/text_events/requested_name.h>

namespace GTServer {
    event_manager::event_manager() {
        m_text_events = std::unordered_map<uint64_t, std::function<void(event_manager::context&)>>();
        m_packet_events = std::unordered_map<uint8_t, std::function<void(event_manager::context&)>>();
    }
    event_manager::~event_manager() {
        m_text_events.clear();
        m_packet_events.clear();
    }

    void event_manager::load_events() {
        this->register_event("requestedName", events::requested_name);
        this->register_event("action", events::action);
    }

    void event_manager::register_event(const std::string& data, std::function<void(event_manager::context&)> fn) {
        m_text_events.insert_or_assign(utils::text::quick_hash(data), fn);
    }
    void event_manager::register_event(const uint8_t& data, std::function<void(event_manager::context&)> fn) {
        m_packet_events.insert_or_assign(data, fn);
    }
    
    bool event_manager::call(const std::string& data, event_manager::context& ctx) {
        if (auto it = m_text_events.find(utils::text::quick_hash(data)); it != m_text_events.end()) {
            it->second(ctx);
            return true;
        }
        return false;
    }
}