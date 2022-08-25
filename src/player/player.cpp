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
        m_peer->data = this;

        m_ip_address.reserve(16);
        enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
    }

    bool Player::is_bit_on(const ePlayerBits& bits) {
        if (m_bits & bits)
            return true;
        return false;
    }
    void Player::set_bit(const ePlayerBits& bits) {
        m_bits |= bits;
    }
    void Player::remove_bit(const ePlayerBits& bits) {
        m_bits &= ~bits;
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
}