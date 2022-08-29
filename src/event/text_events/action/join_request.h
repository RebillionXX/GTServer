#pragma once
#include <world/world.h>
#include <player/player_pool.h>

namespace GTServer::events {
    void join_request(EventContext& ctx) {
        std::shared_ptr<Player> player{ ctx.m_player };
        if (!player->is_flag_on(PLAYERFLAG_IS_IN)) {
            player->disconnect(0U);
            return;
        }
        std::string world_name = ctx.m_parser.get("name", 1);
        std::transform(world_name.begin(), world_name.end(), world_name.begin(), ::toupper);

        if (world_name.empty())
            world_name = std::string{ "START" };

        if (world_name.length() > 24) {
            player->v_sender.OnConsoleMessage("Sorry, the world name is too long!");
            player->v_sender.OnFailedToEnterWorld(true);
            return;
        }

        if (world_name == "EXIT" || !std::regex_match(world_name, std::regex{ "^[A-Z0-9]+$" })) {
            player->v_sender.OnConsoleMessage("Sorry, spaces and special characters are not allowed in world or door names. Try again.");
            player->v_sender.OnFailedToEnterWorld(true);
            return;
        }

        std::shared_ptr<WorldPool> world_pool{ ctx.m_server->get_world_pool() };
        std::shared_ptr<World> world{ world_pool->get_world(world_name) };
        if (!world)
            world = world_pool->new_world(world_name);
        if (world->has_player(player)) {
            world->remove_player(player); //TODO: remove avatar
            player->v_sender.OnFailedToEnterWorld(true);
            return;
        }
        const std::size_t& alloc = world->get_memory_usage();
        uint8_t* data{ world->serialize() };

        GameUpdatePacket* update_packet{ static_cast<GameUpdatePacket*>(std::malloc(sizeof(GameUpdatePacket) + alloc)) };
        update_packet->m_type = NET_GAME_PACKET_SEND_MAP_DATA;
        update_packet->m_net_id = -1;
        update_packet->m_flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
        update_packet->m_data_size = static_cast<uint32_t>(alloc);
        std::memcpy(&update_packet->m_data, data, alloc);
        player->send_packet(NET_MESSAGE_GAME_PACKET, update_packet, sizeof(GameUpdatePacket) + alloc);
        std::free(data);
        std::free(update_packet); 

        const auto& spawn_pos{ world->get_tile_pos(ITEMTYPE_MAIN_DOOR) * 32 };
        const uint32_t& net_id = world->add_player(player);
        player->set_world(world_name);
        player->set_net_id(net_id);
        player->set_position(spawn_pos.x, spawn_pos.y);
        player->v_sender.OnSpawn(player->get_spawn_data(true));
        player->v_sender.OnSetClothing(player->get_clothes(), player->get_skin_color(), false, net_id);
        player->send_character_state(player);

        world->foreach_player([&](const std::shared_ptr<Player>& ply) {
            if (ply == player)
                return;
            player->v_sender.OnSpawn(ply->get_spawn_data());
            player->v_sender.OnSetClothing(ply->get_clothes(), ply->get_skin_color(), false, ply->get_net_id());
            player->send_character_state(ply);

            ply->v_sender.OnSpawn(player->get_spawn_data());  
            ply->v_sender.OnSetClothing(player->get_clothes(), player->get_skin_color(), false, player->get_net_id());
            ply->send_character_state(player);
        });

        player->send_log("`oWorld `w{} ``entered. There are `w{} ``other people here, `w{} ``online", player->get_world(), world->get_players().size() - 1, ctx.m_server->get_player_pool()->get_players().size());
    }
}