#ifndef DATABASE__DATABASE_H
#define DATABASE__DATABASE_H
#include <algorithm>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <configs.h>
#include <server/server_pool.h>
#include <utils/mysql_result.h>
#include <utils/text.h>

namespace GTServer {
    class database {
    public:
        typedef struct {
            std::string m_host;
            std::string m_username;
            std::string m_password;
            std::string m_schema;
        } settings;
        
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
        explicit database(const database::settings& setting);
        ~database();

        bool init();
        sql::ResultSet* query(const std::string& query);

        bool serialize_server_data(ServerPool* sv_pool);
        
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

        database::settings m_settings;
    };
}

#endif // DATABASE__DATABASE_H