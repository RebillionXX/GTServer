#include <events/event_manager.h>
#include <proton/utils/text_scanner.h>
#include <database/database.h>
#include <utils/text.h>

namespace GTServer::events {
    void dialog_return(event_manager::context& ctx) {
        std::string dialog_name;
        if (ctx.m_parser.get("dialog_name", 1).empty())
            return;
        using namespace utils::text;
        const auto& dialog_hash = quick_hash(ctx.m_parser.get("dialog_name"));
        switch (dialog_hash) {
            case "growid"_qh: {
                std::string 
                    name{ ctx.m_parser.get("name", 1) },
                    password{ ctx.m_parser.get("password", 1) },
                    verify_password{ ctx.m_parser.get("verify_password", 1) },
                    email{ ctx.m_parser.get("email", 1) },
                    discord{ ctx.m_parser.get("discord", 1) }
                ;
                const std::pair<Database::RegistrationResult, std::string>& result = 
                    ctx.m_database->register_player(name, password, verify_password, email, discord);
                if (result.first != Database::RegistrationResult::SUCCESS) {
                    ctx.m_player->send_dialog(Player::dialog_type::REGISTRATION, text_scanner({ 
                        { "name", name }, 
                        { "password", password },
                        { "verify_password", verify_password },
                        { "email", email },
                        { "discord", discord },
                        { "extra", result.second }
                    }));
                    break;
                }
                ctx.m_player->send_log("TODO: register");
                break;
            }
            default: {
                ctx.m_player->send_log("unhandled events::dialog_return: `wdialog_name`` -> `w{}``", dialog_name);
                break;
            }
        }
    }
}