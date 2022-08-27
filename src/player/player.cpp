#include <player/player.h>
#include <database/database.h>
#include <database/item/item_component.h>
#include <proton/utils/dialog_builder.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>

namespace GTServer {
    Player::Player(ENetPeer* peer) : 
        m_peer(peer),
        m_login_info{ std::make_shared<LoginInformation>() },
        v_sender{ peer },
        PacketSender{ peer }
    {
        if(!m_peer)
            return;
        m_peer->data = std::malloc(sizeof(uint32_t));
        std::memcpy(m_peer->data, &m_peer->connectID, sizeof(uint32_t));

        m_ip_address.reserve(16);
        enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
        this->set_connect_id(m_peer->connectID);

        for (uint8_t index = 0; index < NUM_BODY_PARTS; index++)
            this->set_cloth(index, (int32_t)ITEM_BLANK);

    }
    Player::~Player() {
        m_login_info.reset();
    }

    bool Player::is_flag_on(const ePlayerFlags& bits) const {
        if (m_flags & bits)
            return true;
        return false;
    }
    void Player::set_flag(const ePlayerFlags& bits) {
        m_flags |= bits;
    }
    void Player::remove_flag(const ePlayerFlags& bits) {
        m_flags &= ~bits;
    }
    
    TextParse Player::get_spawn_data(const bool& local) const {
        TextParse text_parse{};
        text_parse.add("spawn", "avatar")
            ->add<uint32_t>("netID", get_net_id())
            ->add<uint32_t>("userID", get_user_id())
            ->add("colrect", CL_Recti{ 0, 0, 20, 30 })
            ->add("posXY", CL_Vec2i{ get_position().x, get_position().y })
            ->add("name", get_display_name())
            ->add("country", m_login_info->m_country)
            ->add<uint8_t>("invis", is_flag_on(PLAYER_FLAG_IS_INVISIBLE))
            ->add<uint8_t>("mstate", is_flag_on(PLAYER_FLAG_IS_MOD))
            ->add<uint8_t>("smstate", is_flag_on(PLAYER_FLAG_IS_SUPER_MOD))
            ->add<uint32_t>("onlineID", get_connect_id());
        if (local)
            text_parse.add("type", "local");
        return text_parse;
    }

    void Player::send_dialog(const eDialogType& type, TextParse parser) {
        switch (type) {
            case DIALOG_TYPE_REGISTRATION: {
                DialogBuilder db{};
                db.set_default_color('o')
                    ->add_label_with_icon(fmt::format("`w{} V{}``", SERVER_NAME, SERVER_VERSION), 0, DialogBuilder::LEFT, DialogBuilder::BIG)
                    ->add_spacer();
                if (parser.contain("extra"))
                    db.add_textbox(parser.get("extra"))->add_spacer();
                db.add_textbox("By choosing a `wGrowID``, you can use a name and password to logon from any device. Your`` name`` will be shown to other players!")
                    ->add_text_input("name", "GrowID:", parser.get("name"), 18)
                    ->add_spacer()
                    ->add_textbox("Your `wpassword`` must contain`` 8 to 18 characters, 1 letter, 1 number`` and`` 1 special character: @#!$^&*.,``")
                    ->add_text_input_password("password", "Password:", parser.get("password"), 18)
                    ->add_text_input_password("verify_password", "Verify Password:", parser.get("verify_password"), 18)
                    ->add_spacer()
                    ->add_textbox("Your `wemail address `owill only be used for account verification purposes and won't be spammed or shared. If you use a fake email, you'll never be able to recover or change your password.")
                    ->add_text_input("email", "Email:", parser.get("email"), 25)
                    ->add_textbox("Your `wDiscord ID `owill be used for secondary verification if you lost access to your `wemail address`o! Please enter in such format: `wdiscordname#tag`o. Your `wDiscord Tag `ocan be found in your `wDiscord account settings`o.")
                    ->add_text_input("discord", "Discord:", parser.get("discord"), 25)
                    ->add_textbox("We will never ask you for your password, email or discord, never share it with anyone!")
                    ->add_spacer()
                    ->end_dialog("growid", "Disconnect", "Create!");
                v_sender.OnDialogRequest(db.get(), 0);
                break;
            }
            default:
                break;
        }
    }
}