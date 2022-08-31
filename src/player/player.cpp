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
        m_inventory{ std::make_shared<Inventory>(peer) },
        v_sender{ peer },
        PacketSender{ peer },
        CharacterState{}
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
    
    TextScanner Player::get_spawn_data(const bool& local) const {
        TextScanner text_parse{};
        text_parse.add("spawn", "avatar")
            ->add<uint32_t>("netID", get_net_id())
            ->add<uint32_t>("userID", get_user_id())
            ->add("colrect", CL_Recti{ 0, 0, 20, 30 })
            ->add("posXY", CL_Vec2i{ get_position().x, get_position().y })
            ->add("name", get_display_name())
            ->add("country", m_login_info->m_country)
            ->add<uint8_t>("invis", is_flag_on(PLAYERFLAG_IS_INVISIBLE))
            ->add<uint8_t>("mstate", is_flag_on(PLAYERFLAG_IS_MOD))
            ->add<uint8_t>("smstate", is_flag_on(PLAYERFLAG_IS_SUPER_MOD))
            ->add("onlineID", "");
        if (local)
            text_parse.add("type", "local");
        return text_parse;
    }

    void Player::send_dialog(const eDialogType& type, TextScanner parser) {
        switch (type) {
            case DIALOG_TYPE_REGISTRATION: {
                DialogBuilder db{};
                db.set_default_color('o')
                    ->add_label_with_icon("`wGet a GrowID!``", 206, DialogBuilder::LEFT, DialogBuilder::BIG)
                    ->add_spacer();
                if (parser.contain("extra"))
                    db.add_textbox(parser.get("extra"))->add_spacer();
                db.add_textbox("By choosing a `wGrowID``, you can use a name and password to logon from any device. Your`` name`` will be shown to other players!")
                    ->add_text_input("logon", "Name ", parser.get("name"), 18)
                    ->add_spacer()
                    ->add_textbox("Your `wpassword`` must contain`` 8 to 18 characters, 1 letter, 1 number`` and`` 1 special character: @#!$^&*.,``")
                    ->add_text_input_password("password", "Password ", parser.get("password"), 18)
                    ->add_text_input_password("verify_password", "Password Verify", parser.get("verify_password"), 18)
                    ->add_spacer()
                    ->add_textbox("Your `wemail address `owill only be used for account verification purposes and won't be spammed or shared. If you use a fake email, you'll never be able to recover or change your password.")
                    ->add_text_input("email", "Email ", parser.get("email"), 25)
                    ->add_textbox("Your `wDiscord ID `owill be used for secondary verification if you lost access to your `wemail address`o! Please enter in such format: `wdiscordname#tag`o. Your `wDiscord Tag `ocan be found in your `wDiscord account settings`o.")
                    ->add_text_input("discord", "Discord ", parser.get("discord"), 25)
                    ->add_textbox("We will never ask you for your password, email or discord, never share it with anyone!")
                    ->add_spacer()
                    ->end_dialog("growid_apply", "Cancel", "`wGet a GrowID!``");
                v_sender.OnDialogRequest(db.get(), 0);
                break;
            }
            default:
                break;
        }
    }

    
    void Player::send_character_state(std::shared_ptr<Player> player) {
        GameUpdatePacket packet{};
        packet.m_type = NET_GAME_PACKET_SET_CHARACTER_STATE;
        packet.m_punch_id = get_punch_id();
        packet.m_build_range = get_build_range();
        packet.m_punch_range = get_punch_range();
        packet.m_net_id = this->get_net_id();
        packet.m_flags = NET_GAME_PACKET_FLAGS_NONE;
        packet.m_water_speed = get_water_speed();
        packet.m_effect_flags = get_flags();
        packet.m_acceleration = get_acceleration();
        packet.m_punch_strength = get_punch_strength();
        packet.m_speed_out = get_speed();
        packet.m_gravity_out = get_gravity();
        packet.m_pupil_color = get_pupil_color().get_uint();
        packet.m_hair_color = get_hair_color().get_uint();
        packet.m_eyes_color = get_eyes_color().get_uint();

        player->send_packet(NET_MESSAGE_GAME_PACKET, &packet, sizeof(GameUpdatePacket));
    }
}