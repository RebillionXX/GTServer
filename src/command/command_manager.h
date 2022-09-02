#pragma once
#include <command/command_context.h>
#include <command/command.h>

namespace GTServer {
    class CommandManager {
    public:
        CommandManager() = default;
        ~CommandManager();

        void register_commands();

    public:
        static CommandManager& get() { static CommandManager ret; return ret; }
        static std::unordered_map<std::string, Command*> get_commands() { return get().m_commands; }
        static bool execute(ServerPool* servers, std::shared_ptr<Server> server, std::shared_ptr<Player> player, const std::string& text) { return get().execute__interface(servers, server, player, text); }

    private:
        bool execute__interface(ServerPool* servers, std::shared_ptr<Server> server, std::shared_ptr<Player> player, const std::string& text);
        void command_help(const CommandContext& ctx);

    private:
        std::unordered_map<std::string, Command*> m_commands{};
    };
}