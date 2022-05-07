#include <events/event_manager.h>
#include <proton/variant.h>
#include <proton/utils/dialog_builder.h>
#include <proton/utils/text_scanner.h>
#include <player/login_information.h>

namespace GTServer::events {
    void requested_name(event_manager::context& ctx) {
        text_scanner* parser = static_cast<text_scanner*>(ctx.m_data);
        if (!(
            parser->try_get("platformID", ctx.m_local->m_platform)
        )) {
            ctx.m_local->disconnect(0U);    
            return;
        }
        switch (ctx.m_local->m_platform) {
            case NetAvatar::PLATFORM_ID_WINDOWS: {
                ctx.m_local->m_login_info = (void*)(new WindowsPlayer());
                const auto& platform = static_cast<WindowsPlayer*>(ctx.m_local->m_login_info);
                if (!(
                    parser->try_get("fz", platform->m_fz) &&
                    parser->try_get("wk", platform->m_wk) &&
                    parser->try_get("zf", platform->m_zf)
                )) {
                    ctx.m_local->disconnect(0U);    
                    return;
                }
                break;
            }
            default: { //todo more platform checks
                ctx.m_local->disconnect(0U);    
                return;
            }
        }
        
        ctx.m_local->send_dialog(NetAvatar::dialog_type::REGISTRATION, new text_scanner{
            { 
                { "name", ""}, 
                { "password", "" },
                { "verify_password", "" },
                { "email", "" },
                { "discord", "" }
            }
        });
    }
}