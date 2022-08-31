#pragma once
#include <enet/enet.h>
#include <proton/packet.h>
#include <proton/variant.h>

namespace GTServer {
    class PacketSender {
    public:
        PacketSender(ENetPeer* peer) : m_peer(peer) {}
        ~PacketSender() = default;
        [[nodiscard]] ENetPeer* get_peer() { return m_peer; }

        void send_packet(TankUpdatePacket tank_packet, uintmax_t data_size) {
            if (!this->get_peer())
                return;
            ENetPacket* packet = enet_packet_create(nullptr, data_size, ENET_PACKET_FLAG_RELIABLE);
            if (!packet)
                return;
            std::memcpy(packet->data, &tank_packet, data_size);
            if (enet_peer_send(m_peer, 0, packet) != 0)
                enet_packet_destroy(packet);
        }
        void send_packet(TankUpdatePacket* tank_packet, uintmax_t data_size) {
            if (!this->get_peer())
                return;
            GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(&tank_packet->m_data); 
            ENetPacket* packet = enet_packet_create(nullptr, data_size, ENET_PACKET_FLAG_RELIABLE);
            if (!packet || !update_packet)
                return;
            std::memcpy(packet->data, &tank_packet->m_type, 4);
            std::memcpy(packet->data + 4, update_packet, sizeof(GameUpdatePacket) + update_packet->m_data_size);

            if (enet_peer_send(m_peer, 0, packet) != 0)
                enet_packet_destroy(packet);
        }
        void send_packet(int32_t type, const void* data, uintmax_t data_size) {
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
        void send_varlist(const variantlist_t& var, int32_t delay = 0, int32_t net_id = -1) {
            size_t alloc = 1;
            for(const auto& v : var.get_objects()) 
                alloc += v.get_memory_allocate() + 1;
            const uint8_t* var_data = var.serialize();
            
            GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(std::malloc(sizeof(GameUpdatePacket) + alloc));
            update_packet->m_type = NET_GAME_PACKET_CALL_FUNCTION;
            update_packet->m_net_id = net_id;
            update_packet->m_flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
            update_packet->m_delay = delay;
            update_packet->m_data_size = static_cast<uint32_t>(alloc);
            std::memcpy(&update_packet->m_data, var_data, alloc);

            this->send_packet(NET_MESSAGE_GAME_PACKET, update_packet, sizeof(GameUpdatePacket) + update_packet->m_data_size);    
            std::free(update_packet);
        }
    private:
        ENetPeer* m_peer;
    };
}