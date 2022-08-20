#pragma once
#include <event/event_pool.h>
#include <proton/utils/text_scanner.h>

namespace GTServer::events {
    void action(EventContext& ctx) {
        if (ctx.m_parser.get("action").empty())
            return;
        const std::string& action = ctx.m_parser.get("action", 1);
        /*if (!ctx.m_event_manager->call({ action, event_manager::text_event::ACTION }, ctx)) {
            ctx.m_player->send_log("unhandled events::text_events::action: `waction`` -> `w{}``", action);
            return;
        }*/
    }
}