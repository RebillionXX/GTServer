#pragma once
#include <event/event_pool.h>
#include <proton/variant.h>
#include <proton/utils/dialog_builder.h>
#include <proton/utils/text_scanner.h>
#include <player/login_information.h>

namespace GTServer::events {
    void requested_name(EventContext& ctx) {
        if (!ctx.m_parser.try_get<int32_t>("platformID", ctx.m_player->m_platform) ||
            ctx.m_parser.get("meta", 1).empty() || ctx.m_parser.get("rid", 1).empty() || ctx.m_parser.get("mac", 1).empty()) {
            ctx.m_player->disconnect(0U);
            return;
        }
        auto platform{ ctx.m_player->get_login_info() };
        platform->m_mac = ctx.m_parser.get("mac", 1);
        
        switch (ctx.m_player->m_platform) {
            case Player::PLATFORM_ID_WINDOWS: {
                if (!(
                    ctx.m_parser.try_get("fz", platform->m_fz) &&
                    ctx.m_parser.try_get("wk", platform->m_wk) &&
                    ctx.m_parser.try_get("zf", platform->m_zf)
                )) {
                    ctx.m_player->disconnect(0U);    
                    return;
                }
                break;
            }
            case Player::PLATFORM_ID_IOS: {
                if (!(
                    ctx.m_parser.try_get("vid", platform->m_vid)
                )) {
                    ctx.m_player->disconnect(0U);    
                    return;
                }
                break;
            }
            default: { //todo more platform checks
                ctx.m_player->disconnect(0U);    
                return;
            }
        }
        ctx.m_player->send_dialog(Player::dialog_type::REGISTRATION, text_scanner{});
    }
}