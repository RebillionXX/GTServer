#ifndef DATABASE_ITEM__ITEM_H
#define DATABASE_ITEM__ITEM_H

#include <stdint.h>
#include <string>

namespace GTServer {
    struct item {
        uint32_t m_id;
        uint8_t m_editable_type = 0;
        uint8_t m_item_category = 0;
        uint8_t m_action_type = 0;
        uint8_t m_hit_sound_type = 0;

        std::string m_name = "";
        std::string m_texture = "";

        uint32_t m_texture_hash = 0;
        uint8_t m_item_kind = 0;

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
    };
}

#endif //DATABASE_ITEM__ITEM_H