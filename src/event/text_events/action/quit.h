#pragma once

namespace GTServer::events {
    void quit(EventContext& ctx) {
        ctx.m_player->disconnect(0U);
    }
}