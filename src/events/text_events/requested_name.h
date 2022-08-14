#include <events/event_manager.h>
#include <proton/variant.h>
#include <proton/utils/dialog_builder.h>
#include <proton/utils/text_scanner.h>
#include <player/login_information.h>

namespace GTServer::events {
    void requested_name(event_manager::context& ctx) {
        if (!ctx.m_parser.try_get<int32_t>("platformID", ctx.m_player->m_platform) ||
            ctx.m_parser.get("meta", 1).empty() || ctx.m_parser.get("rid", 1).empty() || ctx.m_parser.get("mac", 1).empty()) {
            ctx.m_player->disconnect(0U);    
            return;
        }
        switch (ctx.m_player->m_platform) {
            case Player::PLATFORM_ID_WINDOWS: {
                ctx.m_player->m_login_info = (void*)(new WindowsPlayer());
                const auto& platform = static_cast<WindowsPlayer*>(ctx.m_player->m_login_info);
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
            default: { //todo more platform checks
                ctx.m_player->disconnect(0U);    
                return;
            }
        }
        ctx.m_player->send_dialog(Player::dialog_type::REGISTRATION, text_scanner{});
    }
}