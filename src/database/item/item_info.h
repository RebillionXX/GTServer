#pragma once
#include <stdint.h>
#include <string>
#include <utils/binary_reader.h>

namespace GTServer {
    struct ItemInfo {
        uint32_t m_id;
        uint8_t m_editable_type = 0;
        uint8_t m_item_category = 0;
        uint8_t m_item_type = 0;
        uint8_t m_marterial = 0;

        std::string m_name = "";
        std::string m_texture = "";

        uint32_t m_texture_hash = 0;
        uint8_t m_visual_effect = 0;

        uint32_t m_val1 = 0;
        uint32_t m_ingredient = 0;

        uint8_t m_texture_x = 0;
        uint8_t m_texture_y = 0;
        uint8_t m_spread_type = 0;
        uint8_t m_is_stripey_wallpaper = 0;
        uint8_t m_collision_type = 0;

        uint8_t m_break_hits = 0;

        uint32_t m_reset_time = 0;

        uint32_t m_grow_time = 0;
        uint32_t m_clothing_type = 0;

        uint16_t m_rarity = 0;
        uint8_t m_max_amount = 0;

        std::string m_extra_file = "";
        uint32_t m_extra_file_hash = 0;

        union {
            uint32_t m_audio_volume = 0;
            uint32_t m_weather_id;
        };

        std::string m_pet_name = "";
        std::string m_pet_prefix = "";
        std::string m_pet_suffix = "";
        std::string m_pet_ability = "";

        uint8_t m_seed_base = 0;
        uint8_t m_seed_overlay = 0;
        uint8_t m_tree_base = 0;
        uint8_t m_tree_leaves = 0;

        uint32_t m_seed_color = 0;
        uint32_t m_seed_overlay_color = 0;

        uint16_t m_val2 = 0;
        uint16_t m_rayman = 0;

        std::string m_extra_options = "";
        std::string m_texture2 = "";
        std::string m_extra_options2 = "";
        std::string m_punch_options = "";
        std::string m_description = "This is a seed.";

        uint32_t m_val3 = 0;
        uint32_t m_val4 = 0;
        uint32_t m_val5 = 0;

        uint8_t m_bodypart[9] = { 0 };
        uint8_t m_reserved[80] = { 0 };

        static std::string cypher(const std::string& input, uint32_t item_id) {
            constexpr std::string_view key{ "PBG892FXX982ABC*" };
            std::string ret(input.size(), 0);

            for (uint32_t i = 0; i < input.size(); i++)
                ret[i] = static_cast<char>(input[i] ^ key[(i + item_id) % key.size()]);
            return ret;
        }

        void serialize(BinaryReader& br) {
            m_id = br.read<uint32_t>();
            m_editable_type = br.read<uint8_t>();
            m_item_category = br.read<uint8_t>();
            m_item_type = br.read<uint8_t>();
            m_marterial = br.read<uint8_t>();

            m_name = this->cypher(br.read_string(), m_id);
            m_texture = br.read_string();

            m_texture_hash = br.read<uint32_t>();
            m_visual_effect = br.read<uint8_t>();
            m_val1 = br.read<uint32_t>();
            m_texture_x = br.read<uint8_t>();
            m_texture_y = br.read<uint8_t>();
            m_spread_type = br.read<uint8_t>();
            m_is_stripey_wallpaper = br.read<uint8_t>();
            m_collision_type = br.read<uint8_t>();
            m_break_hits = br.read<uint8_t>() / 6;
            m_reset_time = br.read<uint32_t>();
            m_clothing_type = br.read<uint8_t>();
            m_rarity = br.read<uint16_t>();
            m_max_amount = br.read<uint8_t>();

            m_extra_file = br.read_string();

            m_extra_file_hash = br.read<uint32_t>();
            m_audio_volume = br.read<uint32_t>();

            m_pet_name = br.read_string();
            m_pet_prefix = br.read_string();
            m_pet_suffix = br.read_string();
            m_pet_ability = br.read_string();

            m_seed_base = br.read<uint8_t>();
            m_seed_overlay = br.read<uint8_t>();
            m_tree_base = br.read<uint8_t>();
            m_tree_leaves = br.read<uint8_t>();
            m_seed_color = br.read<uint32_t>();
            m_seed_overlay_color = br.read<uint32_t>();
            m_ingredient = br.read<uint32_t>();
            m_grow_time = br.read<uint32_t>();
            m_val2 = br.read<uint16_t>();
            m_rayman = br.read<uint16_t>();

            m_extra_options = br.read_string();
            m_texture2 = br.read_string();
            m_extra_options2 = br.read_string();
            br.skip(80);
            
            m_punch_options = br.read_string();
            m_val3 = br.read<uint32_t>();
            for (auto index = 0; index < 9; index++)
                m_bodypart[index] = br.read<uint8_t>();
            m_val4 = br.read<uint32_t>();
            m_val5 = br.read<uint32_t>();
        }
    };
}