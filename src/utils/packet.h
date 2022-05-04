#ifndef UTILS__PACKET_H
#define UTILS__PACKET_H

#include <memory>
#include <string>
#include <proton/packet.h>

namespace GTServer::utils {
    std::string get_tank_update_data(ENetPacket* packet) {
        packet->data[packet->dataLength - 1] = 0;
        return reinterpret_cast<char*>(packet->data + sizeof(TankUpdatePacket::type));
    }
}

#endif // UTILS__PACKET_H