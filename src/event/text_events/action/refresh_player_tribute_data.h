#pragma once

namespace GTServer::events {
    void refresh_player_tribute_data(EventContext& ctx) {
        ctx.m_player->send_log("One moment, updating player tribute data...");

        const auto& pair = ctx.m_database->get_tribute()->get_packet();
        ctx.m_player->send_packet(pair.second, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + pair.first);
    }
}