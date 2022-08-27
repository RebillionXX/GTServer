#pragma once
#include <world/world.h>

namespace GTServer::events {
    void join_request(EventContext& ctx) {
        if (!ctx.m_player->is_flag_on(PLAYER_FLAG_IS_IN)) {
            ctx.m_player->disconnect(0U);
            return;
        }
        std::string world_name = ctx.m_parser.get("name", 1);

        if (world_name.empty() || world_name.length() > 24) {
            ctx.m_player->v_sender.OnFailedToEnterWorld(true);
            return;
        }
        std::transform(world_name.begin(), world_name.end(), world_name.begin(), ::toupper);

        if (world_name == "EXIT" || !std::regex_match(world_name, std::regex{ "^[A-Z0-9]+$" })) {
            ctx.m_player->v_sender.OnFailedToEnterWorld(true);
            return;
        }

        std::shared_ptr<WorldPool> world_pool{ ctx.m_server->get_world_pool() };
        std::shared_ptr<World> world{ world_pool->get_world(world_name) };
        if (!world)
            world = world_pool->new_world(world_name);
        if (world->has_player(ctx.m_player)) {
            world->remove_player(ctx.m_player); //TODO: remove avatar
            ctx.m_player->v_sender.OnFailedToEnterWorld(true);
            return;
        }
        const std::size_t& alloc = world->get_memory_usage();
        uint8_t* data{ world->serialize() };

        GameUpdatePacket* update_packet{ static_cast<GameUpdatePacket*>(std::malloc(sizeof(GameUpdatePacket) + alloc)) };
        update_packet->type = NET_GAME_PACKET_SEND_MAP_DATA;
        update_packet->net_id = -1;
        update_packet->flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
        update_packet->data_size = static_cast<uint32_t>(alloc);
        std::memcpy(&update_packet->data, data, alloc);
        ctx.m_player->send_packet(NET_MESSAGE_GAME_PACKET, update_packet, sizeof(GameUpdatePacket) + alloc);
        std::free(data);

        ctx.m_player->set_world(world_name);
        ctx.m_player->set_net_id(world->add_player(ctx.m_player));
        ctx.m_player->set_position(32, 32);
        ctx.m_player->v_sender.OnSpawn(ctx.m_player->get_spawn_data(true).get_all_raw());

        ctx.m_player->send_log("joining world: `w{}`o, data_size: {}", world_name, alloc);
        ctx.m_player->send_log("netID: {}, userID: {}, connectID: {}\n", 
            ctx.m_player->get_net_id(), ctx.m_player->get_user_id(), ctx.m_player->get_connect_id());

        world->foreach_player([&](const std::shared_ptr<Player>& player) {
            if (player == ctx.m_player)
                return;
            ctx.m_player->v_sender.OnSpawn(player->get_spawn_data().get_all_raw());
            player->v_sender.OnSpawn(ctx.m_player->get_spawn_data().get_all_raw());
        });
    }
}