#pragma
#include <world/world.h>

namespace GTServer::events::tile_change_req {
    void OnPunch(EventContext& ctx, std::shared_ptr<World> world, Tile* tile) {
        ctx.m_player->send_log("{} {}", world->get_size().x, world->get_size().y);
    }
}