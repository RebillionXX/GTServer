#include <events/event_manager.h>
#include <proton/utils/text_scanner.h>
#include <database/database.h>
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
            case "growid"_qh: {
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
                    case database::RegistrationResult::SUCCESS: {
                        if (!ctx.m_local->update_internet_protocol()) {
                            ctx.m_local->send_log("`4Oops!`` Couldn't create your account, please try again later.");
                            return;
                        }
                        ctx.m_local->send_log(fmt::format("DEBUG: my ip_address -> {}", ctx.m_local->get_ip_address()));
                        break;
                    }
                    case database::RegistrationResult::EXIST_GROWID: {
                        ctx.m_local->send_dialog(Player::dialog_type::REGISTRATION, text_scanner
                        ({ 
                            { "name", name }, 
                            { "password", password },
                            { "verify_password", verify_password },
                            { "email", email },
                            { "discord", discord },
                            { "extra", fmt::format("`4Oops!`` The name `w{}`` is so cool someone else has already taken it.  Please choose a different name.", name) }
                        }));
                        break;
                    }
                    case database::RegistrationResult::INVALID_GROWID: {
                        ctx.m_local->send_dialog(Player::dialog_type::REGISTRATION, text_scanner
                        ({
                            { "name", name }, 
                            { "password", password },
                            { "verify_password", verify_password },
                            { "email", email },
                            { "discord", discord },
                            { "extra", "`4Oops!``  the name is includes invalid characters." }
                        }));
                        break;
                    }
                    case database::RegistrationResult::INVALID_EMAIL_OR_DISCORD: {
                        ctx.m_local->send_dialog(Player::dialog_type::REGISTRATION, text_scanner
                        ({
                            { "name", name }, 
                            { "password", password },
                            { "verify_password", verify_password },
                            { "email", email },
                            { "discord", discord },
                            { "extra", "`4Oops!``  Look, if you'd like to be able try retrieve your password if you lose it, you'd better enter a real email and discord id.  We promise to keep your data 100% private and never spam you." }
                        }));
                        break;
                    }
                    case database::RegistrationResult::INVALID_GROWID_LENGTH: {
                        ctx.m_local->send_dialog(Player::dialog_type::REGISTRATION, text_scanner
                        ({ 
                            { "name", name }, 
                            { "password", password },
                            { "verify_password", verify_password },
                            { "email", email },
                            { "discord", discord },
                            { "extra", "`4Oops!``  Your `wGrowID`` must be between `$3`` and `$18`` characters long." }
                        }));
                        break;
                    }
                    case database::RegistrationResult::INVALID_PASSWORD_LENGTH: {
                        ctx.m_local->send_dialog(Player::dialog_type::REGISTRATION, text_scanner
                        ({ 
                            { "name", name }, 
                            { "password", "" },
                            { "verify_password", "" },
                            { "email", email },
                            { "discord", discord },
                            { "extra", "`4Oops!``  Your password must be between `$8`` and `$24`` characters long." }
                        }));
                        break;
                    }
                    case database::RegistrationResult::MISMATCH_VERIFY_PASSWORD: {
                        ctx.m_local->send_dialog(Player::dialog_type::REGISTRATION, text_scanner
                        ({   
                            { "name", name }, 
                            { "password", password },
                            { "verify_password", verify_password },
                            { "email", email },
                            { "discord", discord },
                            { "extra", "`4Oops!``  Passwords don't match.  Try again." }
                        }));
                        break;
                    }
                    case database::RegistrationResult::BAD_CONNECTION: {
                        ctx.m_local->send_log("`4Oops,`` server's database had bad connection, re-connecting...");
                        ctx.m_local->send_log("`e>>`` please try again later.");
                        ctx.m_local->disconnect(0U);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default: {
                ctx.m_local->send_log(fmt::format("unhandled events::dialog_return: `wdialog_name`` -> `w{}``", dialog_name));
                break;
            }
        }
    }
}