#pragma once
#include <world/world_pool.h>

namespace GTServer::events {
    void enter_game(EventContext& ctx) {
        if (!ctx.m_player->is_flag_on(PLAYER_FLAG_LOGGED_ON))
            return;
        ctx.m_player->set_flag(PLAYER_FLAG_IS_IN);
        ctx.m_player->send_log("`oWelcome back ``{}`o, {} V{}", ctx.m_player->get_display_name(), config::settings::server_name, SERVER_VERSION);
        ctx.m_server->get_world_pool()->send_default_offers(ctx.m_player);
    }
}