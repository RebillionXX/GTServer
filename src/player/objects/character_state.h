#pragma once
#include <array>
#include <string>
#include <utils/color.h>

namespace GTServer {
    enum eStateFlags {
        STATEFLAG_NOCLIP,
        STATEFLAG_DOUBLE_JUMP,
        STATEFLAG_INVISIBLE,
        STATEFLAG_NO_HAND,
        STATEFLAG_NO_EYE,
        STATEFLAG_NO_BODY,
        STATEFLAG_DEVIL_HORN,
        STATEFLAG_GOLDEN_HALO,
        STATEFLAG_UNKNOWN_8,
        STATEFLAG_UNKNOWN_9,
        STATEFLAG_UNKNOWN_10,
        STATEFLAG_FROZEN,
        STATEFLAG_CURSED,
        STATEFLAG_DUCT_TAPE,
        STATEFLAG_CIGAR,
        STATEFLAG_SHINING,
        STATEFLAG_ZOMBIE,
        STATEFLAG_RED_BODY,
        STATEFLAG_HAUNTED_SHADOWS,
        STATEFLAG_GEIGER_RADIATION,
        STATEFLAG_SPOTLIGHT,
        STATEFLAG_YELLOW_BODY,
        STATEFLAG_PINEAPPLE_FLAG,
        STATEFLAG_FLYING_PINEAPPLE,
        STATEFLAG_SUPER_SUPPORTER_NAME,
        STATEFLAG_SUPER_PINEAPPLE,
        NUM_STATE_FLAGS
    };
    class CharacterState {
    public:
        CharacterState() {
            for (uint8_t index = 0; index < NUM_STATE_FLAGS; index++)
                this->remove_flag((eStateFlags)index);
        }
        ~CharacterState() = default;

        bool is_flag_on(const eStateFlags& flag) const {
            if (m_flags[flag])
                return true;
            return false;
        }
        void set_flag(const eStateFlags& flag) { m_flags[flag] = true; }
        void remove_flag(const eStateFlags& flag) { m_flags[flag] = false; }

        uint8_t get_punch_id() const { return m_punch_id; }
        uint8_t get_punch_range() const { return m_punch_range * 32; }
        uint8_t get_build_range() const { return m_build_range * 32; }
        float get_water_speed() const { return m_water_speed; }
        float get_speed() const { return m_speed; }
        float get_gravity() const { return m_gravity; }
        float get_acceleration() const { return m_acceleration; }
        float get_punch_strength() const { return m_punch_strength; }
        uint32_t get_flags() const {
            uint32_t flags = 0;
            for (auto index = 0; index < 26; index++)
                flags |= m_flags[index] << index;
            return flags;
        }
    
        Color get_pupil_color() const { return m_pupil_color; }
        Color get_hair_color() const { return m_hair_color; }
        Color get_eyes_color() const { return m_eyes_color; }
        
    private:
        uint8_t m_punch_id = 0;
        uint8_t m_punch_range = 1;
        uint8_t m_build_range = 2;

        float m_water_speed = 150.0f;
        float m_speed = 260.0f;
        float m_gravity = 1000.0f;
        float m_acceleration = 1000.0f;
        float m_punch_strength = 350.0f;
        std::array<bool, NUM_STATE_FLAGS> m_flags;

        Color m_pupil_color{ 0xFF, 0xFF, 0xFF, 0xFF };
        Color m_hair_color{ 0xFF, 0xFF, 0xFF, 0xFF };
        Color m_eyes_color{ 0xFF, 0xFF, 0xFF, 0xFF };
    };
}