#ifndef DATABASE__DATABASE_H
#define DATABASE__DATABASE_H
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <constants.h>
#include <server/server_pool.h>
#include <utils/mysql_result.h>

namespace GTServer {
    class database { //some code has been taken from GrowXYZ
    public:
        enum class RegistrationResult {
            SUCCESS,
            EXIST_GROWID,
            INVALID_GROWID,
            INVALID_PASSWORD,
            INVALID_EMAIL,
            INVALID_DISCORD,
            INVALID_GROWID_LENGTH,
            INVALID_PASSWORD_LENGTH,
            BAD_CONNECTION
        };
    public:
        bool init() {
            try {
                using namespace constants;
                m_driver = get_driver_instance();
                m_connection = m_driver->connect(mysql::host.c_str(), mysql::username.c_str(), mysql::password.c_str());
                if (!m_connection)
                    return false;
                m_connection->setSchema(mysql::schema.c_str());
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

        RegistrationResult register_player(const std::string& name, const std::string& password, const std::string& verify_password, const std::string& email, const std::string& discord) {
            return RegistrationResult::BAD_CONNECTION;
        }
    private:
        sql::Driver* m_driver;
        sql::Connection* m_connection;
        sql::Statement* m_statement;
    };
}

#endif // DATABASE__DATABASE_H