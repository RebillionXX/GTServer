#ifndef DATABASE__DATABASE_H
#define DATABASE__DATABASE_H
#include <constants.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace GTServer {
    class database {
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
                return false; //failed to connect
            }
        }
    private:
        sql::Driver* m_driver;
        sql::Connection* m_connection;
        sql::Statement* m_statement;
    };
}

#endif // DATABASE__DATABASE_H