#ifndef DATABASE__DATABASE_H
#define DATABASE__DATABASE_H
#include <algorithm>
#include <mariadb/conncpp.hpp>
#include <mariadb/conncpp/Driver.hpp>
#include <configs.h>
#include <server/server_pool.h>
#include <utils/mysql_result.h>
#include <utils/text.h>

namespace GTServer {
    class Database {
    public:
        typedef struct {
            std::string m_host;
            std::string m_username;
            std::string m_password;
            std::string m_schema;
        } Settings;
        
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
        Database(const Database::Settings& setting);
        ~Database();

        bool init();
        sql::ResultSet* query(const std::string& query);
        
        bool is_player_exist(const std::string& name);
        std::pair<RegistrationResult, std::string> register_player(
            const std::string& name, 
            const std::string& password, 
            const std::string& verify_password, 
            const std::string& email, const 
            std::string& discord
        );
    private:
        sql::Driver* m_driver;
        sql::Connection* m_connection;
        sql::Statement* m_statement;

        Database::Settings m_settings;
    };
}

#endif // DATABASE__DATABASE_H