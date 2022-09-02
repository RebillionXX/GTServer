#pragma once
#include <string>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include <world/world.h>

namespace GTServer {
    class WorldTable {
    public:
        WorldTable(sqlpp::mysql::connection* con) : m_connection(con) { }
        ~WorldTable() = default;

        bool is_exist(const std::string& name);

        uint32_t insert(std::shared_ptr<World> world);
        bool update(std::shared_ptr<World> world);
        bool load(std::shared_ptr<World> world);

    private:
        sqlpp::mysql::connection* m_connection;
    };
}