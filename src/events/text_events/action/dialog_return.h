#include <events/event_manager.h>
#include <proton/utils/text_scanner.h>
#include <utils/text.h>

namespace GTServer::events {
    void dialog_return(event_manager::context& ctx) {
        text_scanner* parser = static_cast<text_scanner*>(ctx.m_data);        
        std::string dialog_name;
        if (!parser->try_get("dialog_name", dialog_name))
            return;
        using namespace utils::text;
        const auto& dialog_hash = quick_hash(dialog_name);
        switch (dialog_hash) {
            /*case "growid"_qh: {
                std::string name, password, verify_password, email, discord;
                if(!(
                    parser->try_get("name", name) &&
                    parser->try_get("password", password) &&
                    parser->try_get("verify_password", verify_password) &&
                    parser->try_get("email", email) &&
                    parser->try_get("discord", discord)
                ))
                    return;
                const auto& result = ctx.m_database->register_player(name, password, verify_password, email, discord);
                switch(result) {
                    case database::RegistrationResult::BAD_CONNECTION: {
                        ctx.m_local->disconnect(0U);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }*/
            default: {
                ctx.m_local->send_log(fmt::format("unhandled events::dialog_return: `wdialog_name`` -> `w{}``", dialog_name));
                break;
            }
        }
    }
}