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
        database(const database::settings& setting) : m_settings(setting) {
            fmt::print("initializing database\n");
            fmt::print(" - {} mysql server {}@{} -> {}\n", this->init() ? 
                "connected to" : "failed to connect", 
            this->m_settings.m_host, this->m_settings.m_username, this->m_settings.m_schema);
        }
        ~database();

        bool init() {
            try {
                m_driver = get_driver_instance();
                m_connection = m_driver->connect(
                    this->m_settings.m_host.c_str(), 
                    this->m_settings.m_username.c_str(), 
                    this->m_settings.m_password.c_str()
                );
                if (!m_connection)
                    return false;
                m_connection->setSchema(this->m_settings.m_schema.c_str());
                return true;
            } catch (const sql::SQLException& e) {
                return false;
            }
        }
        sql::ResultSet* query(const std::string& query) {
            if (!m_connection->isValid())
                m_connection->reconnect();
            if (!m_connection || !m_connection->isValid())
                return nullptr;
            m_statement = m_connection->createStatement();
            return m_statement->executeQuery(query.c_str());
        }

        bool serialize_server_data(server_pool* sv_pool) {
            sql::ResultSet* result = this->query("SELECT * FROM server_data");
            if (!result)
                return false;
            if (!result->next())
                return false;
            mysql_result res(result);
            sv_pool->set_user_id(res.get_int("user_identifier"));
            delete m_statement;
            return true;
        }

        
        bool is_player_exist(const std::string& name) {
            sql::ResultSet* result = this->query(fmt::format("SELECT * FROM `players` WHERE tank_id_name='{}' LIMIT 1", name));
            bool ret = (!result ? true : (result->rowsCount() > 0 ? true : false));
            delete result;
            return ret;
        }
        RegistrationResult register_player(const std::string& name, const std::string& password, const std::string& verify_password, const std::string& email, const std::string& discord) {
            if (password.length() < 8 || password.length() > 24)
                return RegistrationResult::INVALID_PASSWORD_LENGTH;
            if (verify_password != password)
                return RegistrationResult::MISMATCH_VERIFY_PASSWORD;
            if (!utils::text::is_valid_email(email) || !utils::text::is_valid_discord(discord))
                return RegistrationResult::INVALID_EMAIL_OR_DISCORD;
            std::string lower_case_name = name;
            if (!utils::text::to_lowercase(lower_case_name))
                return RegistrationResult::INVALID_GROWID;
            if (lower_case_name.length() < 3 || lower_case_name.length() > 18)
                return RegistrationResult::INVALID_GROWID_LENGTH;
            if (this->is_player_exist(lower_case_name))
                return RegistrationResult::EXIST_GROWID;
            if (!m_connection->isValid())
                return RegistrationResult::BAD_CONNECTION;
            return RegistrationResult::SUCCESS;
        }
    private:
        sql::Driver* m_driver;
        sql::Connection* m_connection;
        sql::Statement* m_statement;

        database::settings m_settings;
    };
}

#endif // DATABASE__DATABASE_H