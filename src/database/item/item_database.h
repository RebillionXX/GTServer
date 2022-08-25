#pragma once
#include <vector>
#include <filesystem>

#include <database/item/item_info.h>
#include <proton/packet.h>

namespace GTServer
{
    class ItemDatabase
    {
    public:
        ItemDatabase();
        ~ItemDatabase();
        
        bool serialize();
        bool deserialize();

        [[nodiscard]] uint32_t get_hash() const { return m_hash; }
        [[nodiscard]] std::pair<std::size_t, TankUpdatePacket*> get_packet() const { return { m_size, m_packet }; }
        [[nodiscard]] std::vector<ItemInfo*> get_items() { return this->m_items; }
    private:
        std::size_t m_size;
        uint8_t* m_data;

        uint32_t m_hash;
        uint16_t m_version;
        uint32_t m_item_count;

        TankUpdatePacket* m_packet;

    private:
        std::vector<ItemInfo*> m_items;
    };
}