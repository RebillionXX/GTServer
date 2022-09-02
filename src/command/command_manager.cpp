#include <command/command_manager.h>
#include <ranges>
#include <fmt/core.h>
#include <utils/text.h>

namespace GTServer {
    CommandManager::~CommandManager() {
        m_commands.clear();
    }
    
    void CommandManager::register_commands() {
        fmt::print("registering commands for CommandManager\n");

        m_commands.insert({ "help", new Command { 
            "help",
            { "?" },
            "`oInfo >> listing you all available commands.``",
            PLAYER_ROLE_DEFAULT,
            std::function<void(const CommandContext&)> {
                [this](auto&& PH1) { this->command_help(std::forward<decltype(PH1)>(PH1)); }
            }
         } });

        fmt::print(" - {} commands registered.\n", m_commands.size());
    }

    bool CommandManager::execute__interface(ServerPool* servers, std::shared_ptr<Server> server, std::shared_ptr<Player> player, const std::string& text) {
        std::vector<std::string> args = utils::split(text, " ");
        if (args.empty()) 
            return false;
        std::string command_name = args[0].substr(1);
        std::transform(command_name.cbegin(), command_name.cend(), command_name.begin(), ::tolower);
        args.erase(args.cbegin());

        return std::ranges::any_of(m_commands, [&](const auto& command) {
            std::vector<std::string> aliases = command.second->get_aliases();
            if (command.second->get_name() == command_name || std::ranges::any_of(aliases, [&](const auto& alias) {
                return alias == command_name;
            })) {
                player->send_log("`6{}``", text);

                CommandContext ctx{
                    .m_player = player,
                    .m_server = server,
                    .m_servers = servers,
                    .m_arguments = args
                };
                command.second->get_function()(ctx);
                return true;
            }

            return false;
        });
    }
}