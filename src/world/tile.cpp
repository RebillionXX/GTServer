#include <world/tile.h>
#include <utils/binary_writer.h>

namespace GTServer {
    Tile::Tile() : 
        m_position{},
        m_foreground{ 0 }, 
        m_background{ 0 }, 
        m_parent{ 0 }, 
        m_flags{ 0 },
        TileExtra{} {}

    std::size_t Tile::get_memory_usage() const {
        std::size_t size{ 
            sizeof(uint16_t) // foreground
            + sizeof(uint16_t) // background
            + sizeof(uint16_t) // parent
            + sizeof(uint16_t) // flags
        }; 
        return size;
    }
    void Tile::serialize(BinaryWriter& buffer) const {
        buffer.write<uint16_t>(m_foreground);
        buffer.write<uint16_t>(m_background);
        buffer.write<uint16_t>(m_parent);
        buffer.write<uint16_t>(m_flags);
    }
}