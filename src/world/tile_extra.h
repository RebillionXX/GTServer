#pragma once
#include <string>

namespace GTServer {
    class TileExtra {
    public:
        enum eType : uint8_t {
            TYPE_NONE,
            TYPE_DOOR,
            TYPE_SIGN
        };
    
    public:
        eType m_extra_type;

        typedef struct Door {
            std::string m_label;
            uint32_t m_unknown;

            void set_label(const std::string& label) { m_label = label; }
            std::string get_label() const { return m_label; }
        } door_data;
    };
}