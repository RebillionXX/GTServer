#pragma once
#include <string>
#include <vector>
#include <proton/utils/common.h>

namespace GTServer {
    class Tile {
    public:
        Tile();
        ~Tile() = default;

        void set_position(const CL_Vec2i& pos) { m_position = pos; }
        [[nodiscard]] CL_Vec2i get_position() const { return m_position; }
        void set_foreground(const uint16_t& fg) { m_foreground = fg; }
        [[nodiscard]] uint16_t get_foreground() const { return m_foreground; }
        void set_background(const uint16_t& bg) { m_background = bg; }
        [[nodiscard]] uint16_t get_background() const { return m_background; }
        void set_parent(const uint16_t& parent) { m_parent = parent; }
        [[nodiscard]] uint16_t get_parent() const { return m_parent; }
        
        std::size_t calculate_memory_usage();
        std::vector<uint8_t> serialize();

    private:
        CL_Vec2i m_position;
        uint16_t m_foreground;
        uint16_t m_background;
        uint16_t m_parent;
        uint16_t m_flags;
    };
}