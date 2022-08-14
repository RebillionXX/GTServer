#ifndef EVENTS_TEXT_EVENTS__ACTION_H
#define EVENTS_TEXT_EVENTS__ACTION_H
#include <events/event_manager.h>
#include <proton/utils/text_scanner.h>

namespace GTServer::events {
    void action(event_manager::context& ctx) {
        if (ctx.m_parser.get("action").empty())
            return;
        const std::string& action = ctx.m_parser.get("action", 1);
        if (!ctx.m_event_manager->call({ action, event_manager::text_event::ACTION }, ctx)) {
            ctx.m_player->send_log("unhandled events::text_events::action: `waction`` -> `w{}``", action);
            return;
        }
    }
}

#endif // EVENTS_TEXT_EVENTS__ACTION_H