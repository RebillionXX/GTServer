#pragma once
#include <event/event_context.h>
#include <proton/utils/text_scanner.h>
#include <database/database.h>
#include <utils/text.h>

namespace GTServer::events {
    void dialog_return(EventContext& ctx) {
        std::string dialog_name;
        if (ctx.m_parser.get("dialog_name", 1).empty())
            return;
        const auto& dialog_hash = utils::quick_hash(ctx.m_parser.get("dialog_name"));
        switch (dialog_hash) {
            case "growid_apply"_qh: {
                std::string 
                    name{ ctx.m_parser.get("logon", 1) },
                    password{ ctx.m_parser.get("password", 1) },
                    verify_password{ ctx.m_parser.get("verify_password", 1) },
                    email{ ctx.m_parser.get("email", 1) },
                    discord{ ctx.m_parser.get("discord", 1) }
                ;
                PlayerTable* db = (PlayerTable*)Database::get_table(Database::DATABASE_PLAYER_TABLE);
                const auto& result{ db->register_player(name, password, verify_password, email, discord) };
                if (result.first != PlayerTable::RegistrationResult::SUCCESS) {
                    ctx.m_player->send_dialog(Player::DIALOG_TYPE_REGISTRATION, TextScanner({ 
                        { "logon", name }, 
                        { "password", password },
                        { "verify_password", verify_password },
                        { "email", email },
                        { "discord", discord },
                        { "extra", result.second }
                    }));
                    break;
                }
                ctx.m_player->send_log("That feature isn't implemented yet");
                break;
            }
            default: {
                ctx.m_player->send_log("unhandled events::dialog_return: `wdialog_name`` -> `w{}``", dialog_name);
                break;
            }
        }
    }
}