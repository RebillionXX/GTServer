#pragma once
#include <unordered_map>
#include <player/objects/packet_sender.h>
#include <database/item/item_database.h>
#include <utils/binary_reader.h>
#include <utils/binary_writer.h>

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

        std::vector<uint8_t> pack() const {
            std::vector<uint8_t> ret{};
            std::size_t alloc{ 8 + (4 * m_items.size()) };
            ret.resize(alloc);
            
            BinaryWriter buffer{ ret.data() };
            buffer.write<uint8_t>(0x1); // inventory version??
            buffer.write<uint32_t>(m_size);
            buffer.write<uint16_t>(static_cast<uint16_t>(m_items.size()));

            for (const auto& item : m_items) {
                uint8_t flags = INVENTORY_ITEM_FLAGS_NONE; // irrelevant
                buffer.write<uint16_t>(item.first);
                buffer.write<uint8_t>(item.second);
                buffer.write<uint8_t>(flags);
            }
            return ret;
        }
        void serialize(const std::vector<uint8_t>& data) {
            BinaryReader br{ data };
            br.skip(1); // inventory version??
            m_size = br.read<uint32_t>();
            const uint16_t& items{ br.read<uint16_t>() };

            for (uint16_t i = 0; i < items; i++) {
                const uint16_t item_id{ br.read<uint16_t>() };
                const uint8_t count{ br.read<uint8_t>() };
                br.skip(1); // flags irrelevant
                m_items.insert_or_assign(item_id, count);
            }
        }

        void send() {
            auto data{ this->pack() };
            GameUpdatePacket* update_packet{ static_cast<GameUpdatePacket*>(std::malloc(sizeof(GameUpdatePacket) + data.size())) };
            std::memset(update_packet, 0, data.size());

            update_packet->m_type = NET_GAME_PACKET_SEND_INVENTORY_STATE;
            update_packet->m_flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
            update_packet->m_data_size = static_cast<uint32_t>(data.size());
            std::memcpy(&update_packet->m_data, data.data(), data.size());

            this->send_packet(NET_MESSAGE_GAME_PACKET, update_packet, sizeof(GameUpdatePacket) + data.size());
            std::free(update_packet);
        }
    private:
        uint32_t m_size{ 20 };
        std::unordered_map<uint16_t, uint8_t> m_items{};
    };
}
