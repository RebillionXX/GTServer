#ifndef PLAYER__PLAYER_H
#define PLAYER__PLAYER_H
#include <atomic>
#include <string>
#include <enet/enet.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <server/server.h>
#include <proton/packet.h>
#include <proton/variant.h>
#include <proton/utils/dialog_builder.h>
#include <proton/utils/text_scanner.h>

namespace GTServer {
    class Player {
    public:
        enum : int32_t {
            PLATFORM_ID_UNKNOWN = -1,
            PLATFORM_ID_WINDOWS,
            PLATFORM_ID_IOS,
            PLATFORM_ID_OSX,
            PLATFORM_ID_LINUX,
            PLATFORM_ID_ANDROID,
            PLATFORM_ID_WINDOWS_MOBILE,
            PLATFORM_ID_WEBOS,
            PLATFORM_ID_BBX,
            PLATFORM_ID_FLASH,
            PLATFORM_ID_HTML5
        };
    public:
        Player(ENetPeer* peer, server* server) 
        : m_peer(peer), m_server(server) {
            if(!m_peer)
                return;
            m_peer->data = this;

            m_ip_address.reserve(16);
            enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
        }
        ~Player() = default;

        [[nodiscard]] ENetPeer* get_peer() const { return m_peer; }
        [[nodiscard]] const char* get_ip_address() const { return m_ip_address.data(); }

        void disconnect(const enet_uint32& data) {
            enet_peer_disconnect_later(this->get_peer(), data);
        }

        bool update_internet_protocol() {
            return enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16) < 0 ? false : true;
        }

        void send(TankUpdatePacket tank_packet, uintmax_t data_size) {
            if (!this->get_peer())
                return;
            ENetPacket* packet = enet_packet_create(nullptr, data_size, ENET_PACKET_FLAG_RELIABLE);
            if (!packet)
                return;
            std::memcpy(packet->data, &tank_packet, data_size);
            if (enet_peer_send(m_peer, 0, packet) != 0)
                enet_packet_destroy(packet);
        }
        void send(TankUpdatePacket* tank_packet, uintmax_t data_size) {
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
        void send(int32_t type, const void* data, uintmax_t data_size) {
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
        void send_var(const variantlist_t& var, int32_t delay = 0, int32_t net_id = -1) {
            size_t alloc = 1;
            for(const auto& v : var.get_objects())
                alloc += v.get_memory_allocate() + 1;
            const uint8_t* var_data = var.serialize();

            TankUpdatePacket* tank_packet = (TankUpdatePacket*)std::malloc(sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + alloc);
            std::memset(tank_packet, 0, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + alloc);
            tank_packet->type = NET_MESSAGE_GAME_PACKET;
            tank_packet->data = (char*)std::malloc(sizeof(GameUpdatePacket) + alloc);
            
            GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(&tank_packet->data);
            update_packet->type = NET_GAME_PACKET_CALL_FUNCTION;
            update_packet->net_id = net_id;
            update_packet->delay = delay;
            update_packet->flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
            update_packet->data_size = alloc;
            std::memcpy(static_cast<uint8_t*>(&update_packet->data), var_data, alloc);

            this->send(tank_packet, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + update_packet->data_size);
            std::free(tank_packet);
        }
        void send_log(const std::string& msg) {
            const auto& data = fmt::format("action|log\nmsg|{}", msg);
            this->send(NET_MESSAGE_GAME_MESSAGE, data.data(), data.size());
        }
    public:
        enum class dialog_type {
            REGISTRATION
        };
        void send_dialog(const dialog_type& type, text_scanner parser) {
            using namespace proton::utils;
            switch (type) {
                case dialog_type::REGISTRATION: {
                    dialog_builder db{};
                    db.set_default_color('o')
                        ->add_label_with_icon("`wGTServer V0.0.1``", 0, dialog_builder::LEFT, dialog_builder::BIG)
                        ->add_spacer();
                    if (parser.contain("extra"))
                        db.add_textbox(parser.get("extra"))->add_spacer();
                    db.add_textbox("By choosing a `wGrowID``, you can use a name and password to logon from any devide. Your`` name`` will be shown to other players!")
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
                    this->send_var({ "OnDialogRequest", db.get() });
                    break;
                }
                default:
                    break;
            }
        }
    public:
        int32_t m_platform = PLATFORM_ID_UNKNOWN;
        void* m_login_info;

        std::atomic<bool> m_logged_on;
    private:
        ENetPeer* m_peer;
        server* m_server;

        std::string m_ip_address;
    };
}

#endif // PLAYER__PLAYER_H