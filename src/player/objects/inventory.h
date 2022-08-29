#pragma once
#include <unordered_map>
#include <player/objects/packet_sender.h>

namespace GTServer {
    class Inventory : public PacketSender {
    public:
        Inventory(ENetPeer* peer) : PacketSender{ peer } {}
        ~Inventory() = default;

        void add(const uint16_t& item_id, const uint8_t& quantity) {
            
        }
    private:
        uint32_t m_size{ 20 };
        std::unordered_map<uint16_t, uint8_t> m_items{};
    };
}
