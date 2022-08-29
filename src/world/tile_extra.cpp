#include <world/tile_extra.h>
#include <fmt/core.h>

namespace GTServer {
    std::size_t TileExtra::get_memory_usage() const {
        std::size_t ret{ 1 };
        switch (get_extra_type()) {
        case TYPE_DOOR: {
            const auto& door = Door{ door_data };
            ret += sizeof(uint16_t) + door.get_label().length();
            ret += sizeof(door.m_unknown);
            return ret;
        }
        default:
            return ret;
        }
    }
    void TileExtra::serialize(BinaryWriter& buffer) const {
        buffer.write<uint8_t>(this->get_extra_type());

        switch (get_extra_type()) {
        case TYPE_DOOR: {
            const auto& door = Door{ door_data };
            buffer.write(door.get_label(), sizeof(uint16_t));
            buffer.write<uint8_t>(door.m_unknown);
            break;
        }
        default:
            break;
        }
    }
}