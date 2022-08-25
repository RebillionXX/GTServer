#include <database/database.h>
#include <string>
#include <utils/text.h>

namespace GTServer {
    Database::Database() : m_connection(nullptr) {
        fmt::print("Initializing Database\n");
        sqlpp::mysql::global_library_init();
    }
    Database::~Database() {
    }

    bool Database::connect() {
        auto config = std::make_shared<sqlpp::mysql::connection_config>();
        config->host = config::database::host;
        config->port = config::database::port;
        config->user = config::database::user;
        config->password = config::database::password;
        config->database = config::database::database;
        config->auto_reconnect = config::database::auto_reconnect;
        config->debug = config::database::debug;

        try {
            m_connection = new sqlpp::mysql::connection{ config };
            fmt::print(" - connection configuration\n"
            "  | host: {}\n"
            "  | user: {}\n"
            "  | database: {}\n", 
            config->host,
            config->user,
            config->database);

            m_player_table = new PlayerTable(m_connection);
            m_player_tribute = new PlayerTribute();
        }
        catch (const sqlpp::exception &e) {
            return false;
        }
        return true;
    }
    bool Database::is_player_exist(const std::string& name) {
        return true;
    }
    std::pair<Database::RegistrationResult, std::string> Database::register_player(
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
        if (this->is_player_exist(lower_case_name))
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

    void* Database::get_table(const eDatabaseTable& table) {
        switch (table) {
            case DATABASE_PLAYER_TABLE:
                return m_player_table;
            default:
                return nullptr;
        }
        return nullptr;
    }
}