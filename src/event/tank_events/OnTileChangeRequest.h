#pragma once
#include <world/world.h>
#include <world/world_pool.h>
#include <event/tank_events/tile_change_req/on_punch.h>

namespace GTServer::events {
    void OnTileChangeRequest(EventContext& ctx) {
        if (!ctx.m_player->is_flag_on(PLAYERFLAG_IS_IN))
            return;
        std::shared_ptr<WorldPool> world_pool{ ctx.m_server->get_world_pool() };
        std::shared_ptr<World> world{ world_pool->get_world(ctx.m_player->get_world()) };
        if (!world)
            return;
        GameUpdatePacket* packet{ ctx.m_update_packet };
        Tile* tile{ world->get_tile(packet->m_int_x, packet->m_int_y) };
        if (!tile)
            return;
        switch (packet->m_item_id) {
        case ITEM_FIST: {
            tile_change_req::OnPunch(ctx, world, tile);
        } break;
        default:
            break;
        }
    }
}