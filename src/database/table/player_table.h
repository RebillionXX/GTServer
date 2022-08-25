#pragma once
#include <fmt/core.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include <player/player.h>

namespace GTServer {
    class PlayerTable {
    public:
        PlayerTable(sqlpp::mysql::connection* con) : m_connection(con) { }
        ~PlayerTable() = default;

        bool is_guest_id_exist(const std::string& rid) const;
        bool is_tank_id_exist(const std::string& name) const;

        uint32_t insert(std::shared_ptr<Player> player);
        bool update(std::shared_ptr<Player> player);
        bool load(std::shared_ptr<Player> player, const bool& guest);

    private:
        sqlpp::mysql::connection* m_connection;
    };
}