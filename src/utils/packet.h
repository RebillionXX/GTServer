#pragma once
#include <memory>
#include <string>
#include <proton/packet.h>

using namespace GTServer;
namespace utils {
    inline std::string get_generic_text(ENetPacket* packet) {
        packet->data[packet->dataLength - 1] = 0;
        return reinterpret_cast<char*>(packet->data + sizeof(TankUpdatePacket::m_type));
    }
    inline GameUpdatePacket* get_game_update_packet(ENetPacket *packet) {
        if (packet->dataLength >= sizeof(GameUpdatePacket)) {
            GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(packet->data + 4);
            if (!(update_packet->m_flags & NET_GAME_PACKET_FLAGS_EXTENDED))
                return update_packet;
            if (packet->dataLength < update_packet->m_data_size + sizeof(GameUpdatePacket))
                return nullptr;
            return update_packet;
        }
        return nullptr;
    }
}