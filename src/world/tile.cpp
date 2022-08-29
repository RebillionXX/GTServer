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

    bool Tile::is_flag_on(const eTileFlags& bits) const {
        if (m_flags & bits)
            return true;
        return false;
    }
    void Tile::set_flag(const eTileFlags& bits) {
        m_flags |= bits;
    }
    void Tile::remove_flag(const eTileFlags& bits) {
        m_flags &= ~bits;
    }

    std::size_t Tile::get_memory_usage() const {
        std::size_t size{ 
            sizeof(uint16_t) // foreground
            + sizeof(uint16_t) // background
            + sizeof(uint16_t) // parent
            + sizeof(uint16_t) // flags
        }; 
        if (is_flag_on(TILEFLAG_TILEEXTRA))
            size += TileExtra::get_memory_usage();
        return size;
    }
    void Tile::serialize(BinaryWriter& buffer) const {
        buffer.write<uint16_t>(m_foreground);
        buffer.write<uint16_t>(m_background);
        buffer.write<uint16_t>(m_parent);
        buffer.write<uint16_t>(m_flags);
        
        if (is_flag_on(TILEFLAG_TILEEXTRA))
            TileExtra::serialize(buffer);
    }
}