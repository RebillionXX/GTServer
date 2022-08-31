#pragma once
#include <world/world.h>
#include <world/world_pool.h>

namespace GTServer::events {
    void OnMovement(EventContext& ctx) {
        if (!ctx.m_player->is_flag_on(PLAYERFLAG_IS_IN))
            return;
        std::shared_ptr<WorldPool> world_pool{ ctx.m_server->get_world_pool() };
        std::shared_ptr<World> world{ world_pool->get_world(ctx.m_player->get_world()) };
        if (!world)
            return;
        GameUpdatePacket* packet{ ctx.m_update_packet };
        if ((int)(packet->m_pos_x / 32) > world->get_size().x || (int)(packet->m_pos_y / 32) > world->get_size().y)
            return;

        packet->m_net_id = ctx.m_player->get_net_id();
        ctx.m_player->set_position(packet->m_pos_x, packet->m_pos_y);
        if (packet->m_flags & NET_GAME_PACKET_FLAGS_FACINGLEFT)
            ctx.m_player->set_flag(PLAYERFLAG_IS_FACING_LEFT);
        else
            ctx.m_player->remove_flag(PLAYERFLAG_IS_FACING_LEFT);

        world->broadcast([&](const std::shared_ptr<Player>& player) {
            player->send_packet(NET_MESSAGE_GAME_PACKET, packet, sizeof(GameUpdatePacket));
        });
    }
}