#include <events/event_manager.h>
#include <proton/utils/text_scanner.h>

namespace GTServer::events {
    void dialog_return(event_manager::context& ctx) {
        text_scanner* parser = static_cast<text_scanner*>(ctx.m_data);        
        std::string dialog_name;
        if (!parser->try_get("dialog_name", dialog_name))
            return;
        using namespace utils::text;
        const auto& dialog_hash = quick_hash(dialog_name);
        switch (dialog_hash) {
            default: {
                ctx.m_local->send_log(fmt::format("unhandled events::dialog_return: `wdialog_name`` -> `w{}``", dialog_name));
                break;
            }
        }
    }
}