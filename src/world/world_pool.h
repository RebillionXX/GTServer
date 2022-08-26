#pragma once
#include <unordered_map>
#include <player/player.h>
#include <world/world.h>

namespace GTServer {
    class WorldPool {
    public:
        WorldPool() = default;
        ~WorldPool();

        void send_default_offers(std::shared_ptr<Player> invoker);
        void send_category_selection();

        std::shared_ptr<World> new_world(const std::string& name);
        void remove_world(const std::string& name);
        std::shared_ptr<World> get_world(const std::string& name);

    private:
        std::unordered_map<std::string, std::shared_ptr<World>> m_worlds{};
    };
}