#pragma once
#include <algorithm>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include <config.h>
#include <server/server_pool.h>
#include <database/player_tribute.h>
#include <database/table/player_table.h>

namespace GTServer {
    class Database {
    public: 
        enum eDatabaseTable {
            DATABASE_PLAYER_TABLE
        };
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
        Database();
        ~Database();
        
        bool connect();
        
        bool is_player_exist(const std::string& name);
        std::pair<RegistrationResult, std::string> register_player(
            const std::string& name, 
            const std::string& password, 
            const std::string& verify_password, 
            const std::string& email,
            const std::string& discord
        );

        sqlpp::mysql::connection* get_connection() { return m_connection; }
        void* get_table(const eDatabaseTable& table);
    private:
        sqlpp::mysql::connection* m_connection;
        PlayerTable* m_player_table{ nullptr };
    };
}