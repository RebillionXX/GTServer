#pragma once
#include <vector>
#include <player/player.h>
#include <world/world.h>

namespace GTServer {
    class WorldPool {
    public:
        WorldPool() = default;
        ~WorldPool();

        void send_default_offers(std::shared_ptr<Player> invoker);
        void send_category_selection();
    private:
        std::vector<World*> m_worlds{};
    };
}