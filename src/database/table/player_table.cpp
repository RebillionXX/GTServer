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

    std::pair<PlayerTable::RegistrationResult, std::string> PlayerTable::register_player(
        const std::string& name, 
        const std::string& password, 
        const std::string& verify_password, 
        const std::string& email, 
        const std::string& discord) 
    {
        if (password.length() < 8 || password.length() > 24)
            return { 
                RegistrationResult::INVALID_PASSWORD_LENGTH,
                "`4Oops!``  Your password must be between `$8`` and `$24`` characters long."
            };
        if (verify_password != password)
            return {
                RegistrationResult::MISMATCH_VERIFY_PASSWORD,
                "`4Oops!``  Passwords don't match.  Try again."
            };
        if (!utils::is_valid_email(email) || !utils::is_valid_discord(discord))
            return {
                RegistrationResult::INVALID_EMAIL_OR_DISCORD,
                "`4Oops!``  Look, if you'd like to be able try retrieve your password if you lose it, you'd better enter a real email and discord id.  We promise to keep your data 100% private and never spam you."
            };
        std::string lower_case_name = name;
        if (!utils::to_lowercase(lower_case_name))
            return {
                RegistrationResult::INVALID_GROWID,
                "`4Oops!``  the name is includes invalid characters."
            };
        if (lower_case_name.length() < 3 || lower_case_name.length() > 18)
            return {
                RegistrationResult::INVALID_GROWID_LENGTH,
                "`4Oops!``  Your `wGrowID`` must be between `$3`` and `$18`` characters long."
            };
        if (this->is_tank_id_exist(lower_case_name))
            return {
                RegistrationResult::EXIST_GROWID,
                fmt::format("`4Oops!``  The name `w{}`` is so cool someone else has already taken it.  Please choose a different name.", name)
            };
        if (!m_connection->is_valid()) {
            m_connection->reconnect();
            return {
                RegistrationResult::BAD_CONNECTION,
                "`4Oops!``  Server's database had bad connection, please try again."
            };
        }
        return { RegistrationResult::SUCCESS, "" };
    }
}