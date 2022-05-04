#ifndef DATABASE_ITEM__ITEM_DATABASE_H
#define DATABASE_ITEM__ITEM_DATABASE_H

#include <vector>
#include <filesystem>

#include <database/item/item.h>
#include <proton/packet.h>

namespace GTServer
{
    class item_database
    {
    public:
        static item_database& instance() { static item_database items; return items; }

        static bool init() { return instance().interface__init(); }
    private:
        item_database() = default;
        ~item_database();

        bool interface__init();
    private:
        uint32_t m_hash;
        char* m_data;
        size_t m_data_size;

        uint32_t m_item_count;
        uint16_t m_version;

        TankUpdatePacket* m_packet;
    private:
        std::vector<item> m_items;
    };
}

#endif // DATABASE_ITEM__ITEM_DATABASE_H