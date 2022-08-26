#include <player/player.h>
#include <database/database.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>

namespace GTServer {
    Player::Player(ENetPeer* peer) : 
        m_peer(peer),
        m_login_info{ std::make_shared<LoginInformation>() },
        v_sender{ peer }
    {
        if(!m_peer)
            return;
        m_peer->data = std::malloc(sizeof(uint32_t));
        std::memcpy(m_peer->data, &m_peer->connectID, sizeof(uint32_t));

        m_ip_address.reserve(16);
        enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
    }
    Player::~Player() {
        m_login_info.reset();
    }

    bool Player::is_flag_on(const ePlayerFlags& bits) {
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

    void Player::send_packet(TankUpdatePacket tank_packet, uintmax_t data_size) {
        if (!this->get_peer())
            return;
        ENetPacket* packet = enet_packet_create(nullptr, data_size, ENET_PACKET_FLAG_RELIABLE);
        if (!packet)
            return;
        std::memcpy(packet->data, &tank_packet, data_size);
        if (enet_peer_send(m_peer, 0, packet) != 0)
            enet_packet_destroy(packet);
    }
    void Player::send_packet(TankUpdatePacket* tank_packet, uintmax_t data_size) {
        if (!this->get_peer())
            return;
        GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(&tank_packet->data); 
        ENetPacket* packet = enet_packet_create(nullptr, data_size, ENET_PACKET_FLAG_RELIABLE);
        if (!packet || !update_packet)
            return;
        std::memcpy(packet->data, &tank_packet->type, 4);
        std::memcpy(packet->data + 4, update_packet, sizeof(GameUpdatePacket) + update_packet->data_size);

        if (enet_peer_send(m_peer, 0, packet) != 0)
            enet_packet_destroy(packet);
    }
    void Player::send_packet(int32_t type, const void* data, uintmax_t data_size) {
        if (!this->get_peer())
            return;
        ENetPacket* packet = enet_packet_create(nullptr, 5 + data_size, ENET_PACKET_FLAG_RELIABLE);
        if (!packet)
            return;
        std::memcpy(packet->data, &type, 4);
        packet->data[data_size + 4] = 0;
        
        if (data)
            std::memcpy(packet->data + 4, data, data_size);

        if (enet_peer_send(m_peer, 0, packet) != 0)
            enet_packet_destroy(packet);
    }

    void Player::send_dialog(const eDialogType& type, text_scanner parser) {
        switch (type) {
            case DIALOG_TYPE_REGISTRATION: {
                DialogBuilder db{};
                db.set_default_color('o')
                    ->add_label_with_icon("`wGTServer V0.0.1``", 0, DialogBuilder::LEFT, DialogBuilder::BIG)
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