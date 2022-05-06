#include <events/event_manager.h>

namespace GTServer::events {
    void action(event_manager::context& ctx) {
        fmt::print("OnAction Event\n");
    }
}