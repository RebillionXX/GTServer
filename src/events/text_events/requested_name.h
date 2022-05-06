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
        
        using namespace proton::utils;
        dialog_builder db{};
        db.set_default_color('o')
            ->add_label_with_icon("`wGTServer V0.0.1``", 0, dialog_builder::LEFT, dialog_builder::BIG)
            ->add_spacer()
            ->add_textbox("By choosing a `wGrowID``, you can use a name and password to logon from any devide. Your`` name`` will be shown to other players!")
            ->add_text_input("name", "GrowID:", "", 18)
            ->add_spacer()
            ->add_textbox("Your `wpassword`` must contain`` 8 to 18 characters, 1 letter, 1 number`` and`` 1 special character: @#!$^&*.,``")
            ->add_text_input_password("password", "Password:", "", 18)
            ->add_text_input_password("verify_password", "Verify Password:", "", 18)
            ->add_spacer()
            ->add_textbox("Your `wemail address `owill only be used for account verification purposes and won't be spammed or shared. If you use a fake email, you'll never be able to recover or change your password.")
            ->add_text_input("email", "Email:", "", 25)
            ->add_textbox("Your `wDiscord ID `owill be used for secondary verification if you lost access to your `wemail address`o! Please enter in such format: `wdiscordname#tag`o. Your `wDiscord Tag `ocan be found in your `wDiscord account settings`o.")
            ->add_text_input("discord", "Discord:", "", 25)
            ->add_textbox("We will never ask you for your password, email or discord, never share it with anyone!")
            ->add_spacer()
            ->end_dialog("growid", "Disconnect", "Create!");
        ctx.m_local->send_var({ "OnDialogRequest", db.get() });
    }
}