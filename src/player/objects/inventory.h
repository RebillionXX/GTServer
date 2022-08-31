#pragma once
#include <unordered_map>
#include <player/objects/packet_sender.h>
#include <database/item/item_database.h>

namespace GTServer {
    enum eInventoryItemFlags {
        INVENTORY_ITEM_FLAGS_NONE = 0,
        INVENTORY_ITEM_FLAGS_ACTIVATED = (1 << 0)
    };

    class Inventory : public PacketSender {
    public:
        Inventory(ENetPeer* peer) : PacketSender{ peer } {}
        ~Inventory() = default;

        bool has(const uint16_t& item) {
            return m_items.find(item) != m_items.end();
        }
        bool add(const uint16_t& item_id, const uint8_t& count) {
            if (auto it = m_items.find(item_id); it != m_items.end()) {
                const auto& item = ItemDatabase::get_item(item_id);
                if (!item)
                    return false;
                if ((it->second + count) > item->m_max_amount)
                    return false;
                it->second += count;
            }
            else {
                if (m_items.size() >= m_size)
                    return false;
                m_items.insert_or_assign(item_id, count);
            }
            return true;
        }

        void send() {
            std::size_t alloc{ 8 + (4 * m_items.size()) };
            GameUpdatePacket* update_packet{ static_cast<GameUpdatePacket*>(std::malloc(sizeof(GameUpdatePacket) + alloc)) };
            std::memset(update_packet, 0, alloc);

            update_packet->m_type = NET_GAME_PACKET_SEND_INVENTORY_STATE;
            update_packet->m_flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
            update_packet->m_data_size = static_cast<uint32_t>(alloc);

            BinaryWriter buffer{ alloc };
            buffer.write<uint8_t>(0x1); // inventory version??
            buffer.write<uint32_t>(m_size);
            buffer.write<uint16_t>(static_cast<uint16_t>(m_items.size()));

            for (const auto& item : m_items) {
                uint8_t flags = INVENTORY_ITEM_FLAGS_NONE; // irrelevant
                buffer.write<uint16_t>(item.first);
                buffer.write<uint8_t>(item.second);
                buffer.write<uint8_t>(flags);
            }
            std::memcpy(&update_packet->m_data, buffer.get(), alloc);

            this->send_packet(NET_MESSAGE_GAME_PACKET, update_packet, sizeof(GameUpdatePacket) + alloc);
            std::free(update_packet);
        }
    private:
        uint32_t m_size{ 20 };
        std::unordered_map<uint16_t, uint8_t> m_items{};
    };
}
