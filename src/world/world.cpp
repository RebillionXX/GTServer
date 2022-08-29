#include <world/world.h>
#include <database/item/item_component.h>
#include <database/item/item_database.h>
#include <utils/binary_writer.h>
#include <utils/random.h>

namespace GTServer {
    World::World(const std::string& name, const uint32_t& width, const uint32_t& height) :
        m_flags(0),
        m_name{ std::move(name) },
        m_width(width),
        m_height(height),
        m_net_id{ 0 } {

    }
    World::~World() {
        m_tiles.clear();
    }

    uint32_t World::add_player(const std::shared_ptr<Player>& player) {
        m_players[m_net_id] = player;
        return ++m_net_id;
    }
    void World::remove_player(const std::shared_ptr<Player>& player) {
        m_players.erase(player->get_net_id());
    }
    bool World::has_player(const std::shared_ptr<Player>& player) {
        for (const auto& pair : m_players) {
            auto ply{ pair.second };
            if (ply->get_user_id() != player->get_user_id())
                continue;
            return true;
        }
        return false;
    }
    std::vector<std::shared_ptr<Player>> World::get_players() {
        std::vector<std::shared_ptr<Player>> ret{};
        for (const auto& pair : m_players)
            ret.push_back(pair.second);
        return ret;
    }
    void World::foreach_player(const std::function<void(const std::shared_ptr<Player>&)>& func) {
        for (const auto& pair : m_players)
            func(pair.second);
    }

    void World::generate() {
        static randutils::pcg_rng gen{ utils::random::get_generator_local() };
        const auto& world_size = this->get_world_size();
        const CL_Vec2i& dirt_layer {
            24, // START
            52  // END
        };
        const CL_Vec2i& maindoor_pos {
            gen.uniform(0, static_cast<int>(world_size.first - 5)) + 5,
            dirt_layer.x
        };
        const int& lava_layer{ 47 };
        const int& bedrock_layer{ 51 };

        m_tiles.reserve(world_size.first * world_size.second);
        for (int i = 0; i < world_size.first * world_size.second; ++i) {
            CL_Vec2i pos{ i % 100, i / 100 };

            Tile tile{};
            tile.set_position(pos);

            if (pos.y > dirt_layer.x && pos.y < dirt_layer.y) {
                if (pos.y > lava_layer && gen.uniform(0.0f, 2.0f) < 1.5f) {
                    tile.set_foreground(ITEM_LAVA);
                }
                else {
                    if (pos.y > 27 && gen.uniform(0.0f, 1.0f) < 0.04f) {
                        tile.set_foreground(ITEM_ROCK);
                    }
                    else {
                        tile.set_foreground(ITEM_DIRT);
                    }
                }
            }
            else if (pos.x == maindoor_pos.x && pos.y == maindoor_pos.y) {
                tile.set_foreground(ITEM_MAIN_DOOR);
                tile.set_background(ITEM_CAVE_BACKGROUND);

                tile.set_flag(TILEFLAG_TILEEXTRA);
                tile.set_extra_type(TileExtra::TYPE_DOOR);
                tile.door_data = TileExtra::Door {
                    .m_label = "EXIT",
                    .m_unknown = 0
                };
            }
            else if (pos.y > bedrock_layer)
                tile.set_foreground(ITEM_BEDROCK);

            if (pos.y > dirt_layer.x)
                tile.set_background(ITEM_CAVE_BACKGROUND);

            m_tiles.push_back(std::move(tile));
        }
    }

    CL_Vec2i World::get_tile_pos(const uint16_t& id) const {
        for (int i = 0; i < m_tiles.size(); i++) {
            if (m_tiles[i].get_foreground() != id)
                continue;
            return { i % m_width, i / m_width };
        }
        return { 0, 0 };
    }
    CL_Vec2i World::get_tile_pos(const eItemTypes& type) const {
        for (int i = 0; i < m_tiles.size(); i++) {
            const auto& item = ItemDatabase::get_item(m_tiles[i].get_foreground());
            if (item->m_item_type != type)
                continue;
            return { i % m_width, i / m_width };
        }
        return { 0, 0 };
    }

    std::size_t World::get_memory_usage() const {
        std::size_t size{ sizeof(uint16_t) }; // version
        size += sizeof(uint32_t); // flags
        size += sizeof(uint16_t); // name length
        size += m_name.length(); // name
        size += sizeof(uint32_t); // width
        size += sizeof(uint32_t); // height
        size += sizeof(uint32_t); // tile count

        for (auto& tile : m_tiles)
            size += tile.get_memory_usage();

        size += sizeof(uint32_t); // object count
        size += sizeof(uint32_t); // object last id
        size += sizeof(uint32_t); // weather
        size += sizeof(uint32_t); // base weather
        return size;
    }
    uint8_t* World::serialize() const {
        const auto& alloc = this->get_memory_usage();
        uint8_t* data{ (uint8_t*)std::malloc(alloc) };
        BinaryWriter buffer{ alloc };

        std::memset(buffer.get(), 0, alloc);
        buffer.write<uint16_t>(0xF);
        buffer.write<uint32_t>(0);
        buffer.write(m_name, sizeof(uint16_t));
        buffer.write<uint32_t>(m_width);
        buffer.write<uint32_t>(m_height);
        buffer.write<uint32_t>(static_cast<uint32_t>(m_tiles.size()));

        for (auto& tile : m_tiles)
            tile.serialize(buffer);
 
        buffer.write<uint32_t>(0);
        buffer.write<uint32_t>(0);
        buffer.write<uint32_t>(0);
        buffer.write<uint32_t>(0);

        std::memcpy(data, buffer.get(), alloc);
        return data;
    }
}