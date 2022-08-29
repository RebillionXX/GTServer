#pragma once

namespace GTServer::events {
    void refresh_item_data(EventContext& ctx) {
        ctx.m_player->send_log("One moment, updating item data...");

        const auto& pair = ItemDatabase::get().get_packet();
        ctx.m_player->send_packet(pair.second, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + pair.first);
    }
}