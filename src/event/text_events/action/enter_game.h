#pragma once
#include <fmt/chrono.h>
#include <world/world_pool.h>

namespace GTServer::events {
    void enter_game(EventContext& ctx) {
        if (!ctx.m_player->is_flag_on(PLAYERFLAG_LOGGED_ON))
            return;

        ctx.m_player->set_flag(PLAYERFLAG_IS_IN);
        ctx.m_player->send_log("`oWelcome back ``{}`o, {} `wV{}``", 
            ctx.m_player->get_display_name(), 
            SERVER_NAME, SERVER_VERSION);

        ctx.m_server->get_world_pool()->send_default_offers(ctx.m_player);
    }
}