#include <world/world_pool.h>
#include <player/player.h>
#include <proton/utils/world_menu.h>

namespace GTServer {
    WorldPool::~WorldPool() {
        const auto& worlds = m_worlds;
        for (auto& world : worlds)
            remove_world(world.first);
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

    std::shared_ptr<World> WorldPool::new_world(const std::string& name) {
        m_worlds[name] = std::make_shared<World>(name);
        m_worlds[name]->generate();
        return m_worlds[name];
    }
    void WorldPool::remove_world(const std::string& name) {
        m_worlds[name].reset();
        m_worlds.erase(name);
    }
    std::shared_ptr<World> WorldPool::get_world(const std::string& name) {
        for (auto& world : m_worlds) {
            if (world.first == name) {
                return world.second;
            }
        }
        return nullptr;
    }
}