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
        world->send_data(player);

        const auto& spawn_pos{ world->get_tile_pos(ITEMTYPE_MAIN_DOOR) * 32 };
        player->set_world(world_name);
        player->set_net_id(world->add_player(player));
        player->set_position(spawn_pos.x, spawn_pos.y);
        player->m_inventory.send();

        player->v_sender.OnSpawn(player->get_spawn_data(true));
        player->v_sender.OnSetClothing(player->get_clothes(), player->get_skin_color(), false, player->get_net_id());
        player->send_character_state(player);

        world->broadcast([&](const std::shared_ptr<Player>& ply) {
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