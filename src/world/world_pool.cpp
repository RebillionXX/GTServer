#include <world/world_pool.h>
#include <player/player.h>
#include <proton/utils/world_menu.h>

namespace GTServer {
    WorldPool::~WorldPool() {
        for (auto &world : m_worlds)
            delete world;
        m_worlds.clear();
    }

    void WorldPool::send_default_offers(std::shared_ptr<Player> invoker) {
        WorldMenu menu{};
        menu.set_default("START")
            ->add_filter()
            ->set_max_rows(2)
            ->add_heading("Active Worlds")
            ->add_floater("START", 0, 0.7, Color{ 0xB4, 0xBD, 0xC2 });
        invoker->v_sender.OnRequestWorldSelectMenu(menu.get());
    }
    void WorldPool::send_category_selection() {

    }
}