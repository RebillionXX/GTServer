#include <player/player.h>

namespace GTServer {
    Player::Player(ENetPeer* peer) : 
        m_peer(peer),
        m_login_info{ std::make_shared<LoginInformation>() }
    {
        if(!m_peer)
            return;
        m_peer->data = this;

        m_ip_address.reserve(16);
        enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
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
    void Player::send_variant(const variantlist_t& var, int32_t delay, int32_t net_id) {
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

        this->send_packet(tank_packet, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + update_packet->data_size);
        std::free(tank_packet);
    }
}