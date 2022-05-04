#include <events/event_manager.h>

namespace GTServer::events {
    void requested_name(event_manager::context& ctx) {
        fmt::print("an text event has been found -> {}\n", ctx.m_local->get_ip_address());
    }
}