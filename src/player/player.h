#ifndef PLAYER__PLAYER_H
#define PLAYER__PLAYER_H
#include <string>
#include <enet/enet.h>
#include <proton/packet.h>
#include <server/server.h>

namespace GTServer {
    class NetAvatar {
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

        void send(TankUpdatePacket tank_packet, uintmax_t data_size)
        {
            if (!get_peer())
                return;
            ENetPacket* packet = enet_packet_create(nullptr, 5 + data_size, ENET_PACKET_FLAG_RELIABLE);
            if (!packet)
                return;
            std::memcpy(packet->data, &tank_packet, data_size);
            if (enet_peer_send(m_peer, 0, packet) != 0)
                enet_packet_destroy(packet);
        }

    public:
        std::string m_ip_address;
    private:
        ENetPeer* m_peer;
        ENetServer* m_server;
    };
}

#endif // PLAYER__PLAYER_H