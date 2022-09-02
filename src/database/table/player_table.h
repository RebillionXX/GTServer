#pragma once
#include <fmt/core.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include <player/player.h>

namespace GTServer {
    class PlayerTable {
    public:
        enum class RegistrationResult {
            SUCCESS,
            EXIST_GROWID,
            INVALID_GROWID,
            INVALID_PASSWORD, //
            INVALID_EMAIL_OR_DISCORD,
            INVALID_GROWID_LENGTH,
            INVALID_PASSWORD_LENGTH,
            MISMATCH_VERIFY_PASSWORD,
            BAD_CONNECTION
        };
    public:
        PlayerTable(sqlpp::mysql::connection* con) : m_connection(con) { }
        ~PlayerTable() = default;

        bool is_guest_id_exist(const std::string& rid) const;
        bool is_tank_id_exist(const std::string& name) const;

        uint32_t insert(std::shared_ptr<Player> player);
        bool update(std::shared_ptr<Player> player);
        bool load(std::shared_ptr<Player> player, const bool& guest);

        std::pair<RegistrationResult, std::string> register_player(
            const std::string& name, 
            const std::string& password, 
            const std::string& verify_password, 
            const std::string& email, 
            const std::string& discord
        );
        
    private:
        sqlpp::mysql::connection* m_connection;
    };
}