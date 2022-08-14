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

    private:
        std::size_t m_size;
        char* m_data;

        uint32_t m_hash;
        uint16_t m_version;
        uint32_t m_item_count;

        std::size_t m_packet_size;
        TankUpdatePacket* m_packet;

    private:
        std::vector<ItemInfo*> m_items;
    };
}