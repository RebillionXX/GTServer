#include <database/table/player_table.h>
#include <database/interface/player_i.h>
#include <utils/text.h>

namespace GTServer {
    bool PlayerTable::is_guest_id_exist(const std::string& rid) const {
        PlayerDB acc{};
        for (const auto &row : (*m_connection)(select(all_of(acc)).from(acc).where(
            acc.relative_identifier == rid and acc.tank_id_name == std::string{}
        ))) {
            if (row._is_valid)
                true;
        }
        return false;
    }
    bool PlayerTable::is_tank_id_exist(const std::string& name) const {
        PlayerDB acc{};
        for (const auto &row : (*m_connection)(select(all_of(acc)).from(acc).where(
            acc.raw_name == name and acc.tank_id_name != std::string{}
        ))) {
            if (row._is_valid)
                true;
        }
        return false;
    }

    uint32_t PlayerTable::insert(std::shared_ptr<Player> player) {
        std::shared_ptr<LoginInformation> login{ player->m_login_info };

        if (this->is_guest_id_exist(login->m_rid))
            return 0;
        PlayerDB acc{};
        auto id = (*m_connection)(insert_into(acc).set(
            acc.requsted_name = player->get_raw_name(),
            acc.tank_id_name = std::string{},
            acc.tank_id_pass = std::string{},
            acc.raw_name = player->get_raw_name(),
            acc.display_name = player->get_display_name(),
            acc.relative_identifier = login->m_rid,
            acc.machine_address = login->m_mac,
            acc.role = player->get_role(),
            acc.inventory = player->m_inventory.pack()
        ));
        return id;
    }
    bool PlayerTable::update(std::shared_ptr<Player> player) {
        return false;
    }
    bool PlayerTable::load(std::shared_ptr<Player> player, const bool& guest) {
        try {
            if (guest) {
                PlayerDB acc{};
                for (const auto &row : (*m_connection)(select(all_of(acc)).from(acc).where(
                    acc.relative_identifier == player->get_login_info()->m_rid &&
                    acc.tank_id_name == std::string{}
                ))) {
                    if (row._is_valid) {
                        player->set_user_id(static_cast<uint32_t>(row.id));
                        player->set_raw_name(row.raw_name);
                        player->set_role(row.role);
                        player->m_inventory.serialize(row.inventory);
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
}