#include <database/item/item_database.h>
#include <filesystem>
#include <fstream>
#include <fmt/core.h>
#include <utils/binary_reader.h>

namespace GTServer {
    item_database::~item_database() {
        free(m_data);
        m_items.clear();
    }

    bool item_database::interface__init() {
        if (!std::filesystem::exists("utils/items.dat"))
            return false;
        uintmax_t data_size = std::filesystem::file_size("utils/items.dat");
        m_data = (char*)std::malloc(data_size);

        std::ifstream file("utils/items.dat", std::ios::binary);
        if (file.bad())
            return false;
        file.read(m_data, data_size);
        file.close();

        binary_reader br(reinterpret_cast<uint8_t*>(m_data));
        m_version = br.read<uint16_t>();
        m_item_count = br.read<uint32_t>();

        for (uint32_t i = 0; i < m_item_count; i++) {
            item item;
            item.m_id = br.read<uint32_t>();

            item.m_editable_type = br.read<uint8_t>();
            item.m_item_category = br.read<uint8_t>();
            item.m_action_type = br.read<uint8_t>();
            item.m_hit_sound_type = br.read<uint8_t>();

            item.m_name = br.read_rt_name(item.m_id);
            item.m_texture = br.read_string();

            item.m_texture_hash = br.read<uint32_t>();
            item.m_item_kind = br.read<uint8_t>();
            item.m_val1 = br.read<uint32_t>();
            item.m_texture_x = br.read<uint8_t>();
            item.m_texture_y = br.read<uint8_t>();
            item.m_spread_type = br.read<uint8_t>();
            item.m_is_stripey_wallpaper = br.read<uint8_t>();
            item.m_collision_type = br.read<uint8_t>();
            item.m_break_hits = br.read<uint8_t>() / 6;
            item.m_reset_time = br.read<uint32_t>();
            item.m_clothing_type = br.read<uint8_t>();
            item.m_rarity = br.read<uint16_t>();
            item.m_max_amount = br.read<uint8_t>();

            item.m_extra_file = br.read_string();

            item.m_extra_file_hash = br.read<uint32_t>();
            item.m_audio_volume = br.read<uint32_t>();

            item.m_pet_name = br.read_string();
            item.m_pet_prefix = br.read_string();
            item.m_pet_suffix = br.read_string();
            item.m_pet_ability = br.read_string();

            item.m_seed_base = br.read<uint8_t>();
            item.m_seed_overlay = br.read<uint8_t>();
            item.m_tree_base = br.read<uint8_t>();
            item.m_tree_leaves = br.read<uint8_t>();
            item.m_seed_color = br.read<uint32_t>();
            item.m_seed_overlay_color = br.read<uint32_t>();
            item.m_ingredient = br.read<uint32_t>();
            item.m_grow_time = br.read<uint32_t>();
            item.m_val2 = br.read<uint16_t>();
            item.m_rayman = br.read<uint16_t>();

            item.m_extra_options = br.read_string();
            item.m_texture2 = br.read_string();
            item.m_extra_options2 = br.read_string();
            br.skip(80);
            
            item.m_punch_options = br.read_string();
            item.m_val3 = br.read<uint32_t>();
            for (auto index = 0; index < 9; index++)
                item.m_bodypart[index] = br.read<uint8_t>();
            item.m_val4 = br.read<uint32_t>();
            item.m_val5 = br.read<uint32_t>();

            if (i != item.m_id)
                fmt::print("items are unordered: {}/{}\n", i, item.m_id);
            m_items.push_back(std::move(item));
        }

        m_packet = static_cast<TankUpdatePacket*>(std::malloc(sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + data_size));
        std::memset(m_packet, 0, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + data_size);
        m_packet->type = NET_MESSAGE_GAME_PACKET;
        m_packet->data = static_cast<char*>(std::malloc(sizeof(GameUpdatePacket) + data_size));

        GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(m_packet->data);
        update_packet->type = NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA;
        update_packet->net_id = -1;
        update_packet->flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
        update_packet->data_size = (uint32_t)data_size;
        std::memcpy(&update_packet->data, m_data, data_size);
        std::memcpy(&m_packet->data, update_packet, sizeof(GameUpdatePacket) + data_size);
        free(update_packet);
        return true;
    }
}