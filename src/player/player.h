#ifndef PLAYER__PLAYER_H
#define PLAYER__PLAYER_H
#include <string>
#include <enet/enet.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <proton/packet.h>
#include <proton/variant.h>
#include <server/server.h>
#include <database/item/item_database.h>

namespace GTServer {
    class NetAvatar {
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
        NetAvatar(ENetPeer* peer, ENetServer* server) 
        : m_peer(peer), m_server(server) {
            if(!m_peer)
                return;
            m_peer->data = this;

            m_ip_address.reserve(16);
            enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
        }
        ~NetAvatar() = default;

        [[nodiscard]] ENetPeer* get_peer() const { return m_peer; }
        [[nodiscard]] const char* get_ip_address() const { return m_ip_address.data(); }

        void disconnect(const enet_uint32& data) {
            enet_peer_disconnect_later(this->get_peer(), data);
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
            //DEBUG
            std::vector<char> array;
            for(auto i = 0; i < update_packet->data_size; i++)
                array.push_back(static_cast<char>(reinterpret_cast<uint8_t*>(&update_packet->data)[i]));
            fmt::print("array: {}\n", array);
            std::memcpy(packet->data + 4, update_packet, sizeof(GameUpdatePacket) + update_packet->data_size);

            if (enet_peer_send(m_peer, 0, packet) != 0)
                enet_packet_destroy(packet);
        }
        void send_text(const std::string& packet) {
            TankUpdatePacket* tank_packet = (TankUpdatePacket*)std::malloc(sizeof(TankUpdatePacket) + packet.size());
            tank_packet->type = NET_MESSAGE_GAME_MESSAGE;
            tank_packet->data = (char*)std::malloc(packet.size());
            std::memcpy(reinterpret_cast<uint8_t*>(&tank_packet->data), packet.data(), packet.size());
            reinterpret_cast<uint8_t*>(&tank_packet->data)[packet.size()] = 0;

            this->send(tank_packet, sizeof(TankUpdatePacket) + packet.size());
            std::free(tank_packet);
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
            this->send_text(fmt::format("action|log\nmsg|{}", msg));
        }
    public:
        int32_t m_platform = PLATFORM_ID_UNKNOWN;
        void* m_login_info;

        std::string m_ip_address;
    private:

        ENetPeer* m_peer;
        ENetServer* m_server;
    };
}

#endif // PLAYER__PLAYER_H