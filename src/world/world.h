#pragma once
#include <string>
#include <vector>
#include <functional>
#include <player/player.h>
#include <world/tile.h>

namespace GTServer {
    class World {
    public:
        explicit World(const std::string& name, const uint32_t& width = 100, const uint32_t& height = 60);
        ~World();

        uint32_t add_player(const std::shared_ptr<Player>& player);
        void remove_player(const std::shared_ptr<Player>& player);
        bool has_player(const std::shared_ptr<Player>& player);
        void foreach_player(const std::function<void(const std::shared_ptr<Player>&)>& func);

        void set_world_size(const uint32_t& width, const uint32_t& height) { m_width = width; m_height = height; }
        [[nodiscard]] std::pair<uint32_t, uint32_t> get_world_size() const { return { m_width, m_height }; }
        void generate();

        std::size_t get_memory_usage() const;
        uint8_t* serialize() const;

    private:
        uint32_t m_flags;

        std::string m_name;
        uint32_t m_width;
        uint32_t m_height;

        std::vector<Tile> m_tiles;

        uint32_t m_net_id;
        std::unordered_map<uint32_t, std::shared_ptr<Player>> m_players;
    };
}