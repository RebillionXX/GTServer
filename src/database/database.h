#ifndef DATABASE__DATABASE_H
#define DATABASE__DATABASE_H
#include <constants.h>
#include <server/server_pool.h>
#include <utils/mysql_result.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace GTServer {
    class database { //some code has been taken from GrowXYZ
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
                fmt::print("SQL driver instance error: {}.\n", e.what());
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
    private:
        sql::Driver* m_driver;
        sql::Connection* m_connection;
        sql::Statement* m_statement;
    };
}

#endif // DATABASE__DATABASE_H