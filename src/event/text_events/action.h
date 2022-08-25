#pragma once
#include <event/event_pool.h>

namespace GTServer::events {
    void action(EventContext& ctx) {
        if (!ctx.m_player->is_flag_on(PLAYER_FLAG_LOGGED_ON) ||
            ctx.m_parser.get("action").empty())
            return;
        const std::string& action = ctx.m_parser.get("action", 1);
        if (!ctx.m_events->execute(EVENT_TYPE_ACTION, action, ctx)) {
            ctx.m_player->send_log("unhandled events::generic_text::action: `waction`` -> `w{}``", action);
            return;
        }
    }
}