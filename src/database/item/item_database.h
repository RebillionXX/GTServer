#pragma once
#include <vector>
#include <filesystem>
#include <database/item/item_info.h>
#include <database/item/item_component.h>
#include <proton/packet.h>

namespace GTServer
{
    class ItemDatabase
    {
    public:
        ItemDatabase() = default;
        ~ItemDatabase();
        
        bool serialize();
        bool deserialize();

        static uint32_t get_hash() { return get().m_hash; }
        static std::pair<std::size_t, TankUpdatePacket*> get_packet() { return { get().m_size, get().m_packet }; }
        static std::vector<ItemInfo*> get_items() { return get().m_items; }

        static ItemInfo* get_item(const uint32_t& item) { return get().get_item__interface(item); }
    public:
        static ItemDatabase& get() { static ItemDatabase ret; return ret; }

    public:
        ItemInfo* get_item__interface(const uint32_t& item);

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