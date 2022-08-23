#pragma once
#include <fmt/core.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include <database/interface/account_i.h>
#include <player/player.h>

namespace GTServer {
    class PlayerTable {
    public:
        PlayerTable(sqlpp::mysql::connection* con) : m_connection(con) { }
        ~PlayerTable() = default;

        uint32_t insert(std::shared_ptr<Player> player) {
            return 0;
        }
        bool update(std::shared_ptr<Player> player) {
            return false;
        }
        bool load(std::shared_ptr<Player> player, const bool& guest) {
            try {
                if (guest) {
                    Account acc{};
                    for (const auto &row : (*m_connection)(select(all_of(acc)).from(acc).where(
                        acc.relative_identifier == player->get_login_info()->m_rid &&
                        acc.tank_id_name == std::string{}
                    ))) {
                        if (row._is_valid) {
                            player->set_user_id(static_cast<uint32_t>(row.id));
                            return true;
                        }
                    }
                    fmt::print("[DEBUG]: Couldn't fetch account for {}\n", player->get_login_info()->m_rid);
                    return false;
                }
                //todo registered account
                return false;
            }
            catch(const std::exception &e) {
                return false;
            }
            return false;
        }

    private:
        sqlpp::mysql::connection* m_connection;
    };
}