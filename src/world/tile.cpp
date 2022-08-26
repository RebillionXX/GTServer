#include <world/tile.h>
#include <utils/binary_writer.h>

namespace GTServer {
    Tile::Tile() : m_position{}, m_foreground{ 0 }, m_background{ 0 }, m_flags{} {}

    std::size_t Tile::calculate_memory_usage()
    {
        std::size_t size{ 
            sizeof(uint16_t) // foreground
            + sizeof(uint16_t) // background
            + sizeof(uint16_t) // parent
            + sizeof(uint16_t) // flags
        }; 
        return size;
    }

    std::vector<uint8_t> Tile::serialize()
    {
        std::vector<uint8_t> mem(calculate_memory_usage(), 0);

        BinaryWriter buffer{ mem.data() };
        buffer.write<uint16_t>(m_foreground);
        buffer.write<uint16_t>(m_background);
        buffer.write<uint16_t>(m_parent);
        buffer.write<uint16_t>(m_flags);
        return mem;
    }
}