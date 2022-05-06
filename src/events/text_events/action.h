#include <events/event_manager.h>
#include <proton/utils/text_scanner.h>

namespace GTServer::events {
    void action(event_manager::context& ctx) {
        text_scanner* parser = static_cast<text_scanner*>(ctx.m_data);
        std::string action;
        if (!parser->try_get("action", action))
            return;
        if (!ctx.m_event_manager->call({ action, event_manager::text_event::ACTION }, ctx)) {
            ctx.m_local->send_log(fmt::format("unhandled events::text_events::action: `waction`` -> `w{}``", action));
            return;
        }
    }
}