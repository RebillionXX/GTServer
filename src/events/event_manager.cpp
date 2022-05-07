#include <events/event_manager.h>
#include <fmt/core.h>
#include <utils/text.h>

#include <events/text_events/action.h>
#include <events/text_events/requested_name.h>

#include <events/text_events/action/dialog_return.h>

namespace GTServer {
    event_manager::event_manager() {
        m_action_event = std::unordered_map<uint64_t, std::function<void(event_manager::context&)>>();
        m_text_events = std::unordered_map<uint64_t, std::function<void(event_manager::context&)>>();
        m_packet_events = std::unordered_map<uint8_t, std::function<void(event_manager::context&)>>();
    }
    event_manager::~event_manager() {
        m_action_event.clear();
        m_text_events.clear();
        m_packet_events.clear();
    }

    void event_manager::load_events() {
        this->register_event({ "requestedName", text_event::TEXT }, events::requested_name);
        this->register_event({ "action", text_event::TEXT }, events::action);

        this->register_event({ "dialog_return", text_event::ACTION }, events::dialog_return);
    }

    void event_manager::register_event(const std::pair<std::string, text_event> data, std::function<void(event_manager::context&)> fn) {
        switch (data.second) {
            case event_manager::text_event::TEXT: {
                m_text_events.insert_or_assign(utils::text::quick_hash(data.first), fn);
            } break;
            case event_manager::text_event::ACTION: {
                m_action_event.insert_or_assign(utils::text::quick_hash(data.first), fn);
            } break;
            default:
                break;
        }
    }
    void event_manager::register_event(const uint8_t& data, std::function<void(event_manager::context&)> fn) {
        m_packet_events.insert_or_assign(data, fn);
    }
    
    bool event_manager::call(const std::pair<std::string, text_event> data, event_manager::context& ctx) {
        switch (data.second) {
            case event_manager::text_event::TEXT: {
                if (auto it = m_text_events.find(utils::text::quick_hash(data.first)); it != m_text_events.end()) {
                    it->second(ctx);
                    return true;
                }
                return false;
            } break;
            case event_manager::text_event::ACTION: {
                if (auto it = m_action_event.find(utils::text::quick_hash(data.first)); it != m_action_event.end()) {
                    it->second(ctx);
                    return true;
                }
                return false;
            } break;
            default:
                return false;
        }
    }
}