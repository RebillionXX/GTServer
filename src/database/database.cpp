#include <string>
#include <database/database.h>

namespace GTServer {
    database::database(const database::settings& setting) : m_settings(setting) {
        fmt::print("initializing database\n");
        fmt::print(" - {} mysql server {}@{} -> {}\n", this->init() ? 
            "connected to" : "failed to connect", 
        this->m_settings.m_host, this->m_settings.m_username, this->m_settings.m_schema);
    }
    database::~database() {
        if (!this->m_connection->isClosed())
            this->m_connection->close();
    }
    
    bool database::init() {
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
    sql::ResultSet* database::query(const std::string& query) {
        if (!m_connection->isValid())
            m_connection->reconnect();
        if (!m_connection || !m_connection->isValid())
            return nullptr;
        m_statement = m_connection->createStatement();
        return m_statement->executeQuery(query.c_str());
    }

    bool database::serialize_server_data(ServerPool* sv_pool) {
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

    bool database::is_player_exist(const std::string& name) {
        sql::ResultSet* result = this->query(fmt::format("SELECT * FROM `players` WHERE tank_id_name='{}' LIMIT 1", name));
        bool ret = (!result ? true : (result->rowsCount() > 0 ? true : false));
        delete result;
        return ret;
    }

    std::pair<database::RegistrationResult, std::string> database::register_player(
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
        if (!utils::text::is_valid_email(email) || !utils::text::is_valid_discord(discord))
            return {
                RegistrationResult::INVALID_EMAIL_OR_DISCORD,
                "`4Oops!``  Look, if you'd like to be able try retrieve your password if you lose it, you'd better enter a real email and discord id.  We promise to keep your data 100% private and never spam you."
            };
        std::string lower_case_name = name;
        if (!utils::text::to_lowercase(lower_case_name))
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
        if (!m_connection->isValid()) {
            if (!m_connection->reconnect())
                fmt::print("failed to reconnect database connection.\n");
            return {
                RegistrationResult::BAD_CONNECTION,
                "`4Oops!``  Server's database had bad connection, please try again."
            };
        }
        return { RegistrationResult::SUCCESS, "" };
    }
}