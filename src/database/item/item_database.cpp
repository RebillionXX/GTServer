#include <database/item/item_database.h>
#include <filesystem>
#include <fstream>
#include <fmt/core.h>
#include <utils/binary_reader.h>
#include <utils/text.h>
#include <proton/utils/misc_utils.h>

namespace GTServer {
    ItemDatabase::ItemDatabase() : m_size{}, m_hash{}, m_version{}, m_item_count{}, m_packet_size{} {}
    ItemDatabase::~ItemDatabase() {
        for (auto item : m_items)
            delete item;
        m_items.clear();
        std::free(m_data);
        std::free(m_packet);
    }

    bool ItemDatabase::serialize() {
        if (!std::filesystem::exists("utils/items.dat"))
            return false;
        m_size = std::filesystem::file_size("utils/items.dat");
        m_data = (char*)std::malloc(m_size);

        std::ifstream file("utils/items.dat", std::ios::binary);
        if (file.bad())
            return false;
        file.read(reinterpret_cast<char*>(m_data), static_cast<std::streamsize>(m_size));
        file.close();
        m_hash = proton::utils::hash(reinterpret_cast<char*>(m_data), static_cast<int32_t>(m_size));

        BinaryReader br(reinterpret_cast<uint8_t*>(m_data));
        m_version = br.read<uint16_t>();
        m_item_count = br.read<uint32_t>();

        m_items.reserve(m_item_count);
        for (uint32_t i = 0; i < m_item_count; i++) {
            m_items.push_back(new ItemInfo{});
            m_items[i]->serialize(br);

            if (i != m_items[i]->m_id) {
                fmt::print(" - unsupported items.dat version -> {}\n", m_version);
                break;
            }
        }

        m_packet = static_cast<TankUpdatePacket*>(std::malloc(sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + m_size));
        std::memset(m_packet, 0, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + m_size);
        m_packet->type = NET_MESSAGE_GAME_PACKET;
        m_packet->data = static_cast<char*>(std::malloc(sizeof(GameUpdatePacket) + m_size));

        GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(m_packet->data);
        update_packet->type = NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA;
        update_packet->net_id = -1;
        update_packet->flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
        update_packet->data_size = (uint32_t)m_size;
        std::memcpy(&update_packet->data, m_data, m_size);
        std::memcpy(&m_packet->data, update_packet, sizeof(GameUpdatePacket) + m_size);
        free(update_packet);
        return true;
    }
}