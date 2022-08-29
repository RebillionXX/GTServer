#pragma once
#include <string>
#include <utils/binary_writer.h>

namespace GTServer {
    class TileExtra {
    public:
        enum eType : uint8_t {
            TYPE_NONE,
            TYPE_DOOR,
            TYPE_SIGN
        };
    
    private:
        eType m_extra_type;
    
    public:
        void set_extra_type(const eType& type) { m_extra_type = type; }
        [[nodiscard]] eType get_extra_type() const { return m_extra_type; }

        struct Door {
            std::string m_label;
            uint8_t m_unknown;

            void set_label(const std::string& label) { m_label = label; }
            std::string get_label() const { return m_label; }
        } door_data;

    public:
        TileExtra() :
            door_data() 
        {}
        ~TileExtra() = default;

        std::size_t get_memory_usage() const;
        void serialize(BinaryWriter& buffer) const;
    };
}