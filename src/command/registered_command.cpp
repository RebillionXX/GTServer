#include <command/command_manager.h>

namespace GTServer {
    void CommandManager::command_help(const CommandContext& ctx)
    {
        if (!ctx.m_arguments.empty()) {
            auto it = std::find_if(m_commands.begin(), m_commands.end(), [&](const auto& command) {
                if (command.second->get_name() != ctx.m_arguments[0]) {
                    std::vector<std::string> aliases = command.second->get_aliases();
                    return std::find(aliases.cbegin(), aliases.cend(), ctx.m_arguments[0]) != aliases.cend();
                }
                return command.second->get_name() == ctx.m_arguments[0];
            });

            if (it != m_commands.end()) {
                if (ctx.m_player->get_role() >= (*it).second->get_role()) {
                    ctx.m_player->send_log((*it).second->get_description());
                    return;
                }
            }
            ctx.m_player->send_log("`4Unknown command. ``Enter `$/help`` for a list of valid commands.");
            return;
        }
        std::string commands{ "Supported commands are: " };
        for (const auto& command : m_commands) {
            if (ctx.m_player->get_role() < command.second->get_role())
                continue;
            commands.append(fmt::format("/{}, ", command.second->get_name()));
        }

        commands.resize(commands.size() - 2);
        ctx.m_player->send_log(commands);
    }
}