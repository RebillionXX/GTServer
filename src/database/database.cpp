#include <database/database.h>
#include <string>
#include <utils/text.h>

namespace GTServer {
    Database::~Database() {
        if (m_connection->is_valid())
            delete m_connection;
        delete m_player_table;
        delete m_world_table;
    }

    bool Database::connect() {
        sqlpp::mysql::global_library_init();
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
            fmt::print("Initializing Database\n");
            fmt::print(" - connection configuration\n"
            "  | host: {}\n"
            "  | user: {}\n"
            "  | database: {}\n", 
            config->host,
            config->user,
            config->database);

            m_player_table = new PlayerTable(m_connection);
            m_world_table = new WorldTable(m_connection);
        }
        catch (const sqlpp::exception &e) {
            return false;
        }
        return true;
    }

    void* Database::get_table__interface(const eDatabaseTable& table) {
        switch (table) {
            case DATABASE_PLAYER_TABLE:
                return m_player_table;
            case DATABASE_WORLD_TABLE:
                return m_world_table;
            default:
                return nullptr;
        }
        return nullptr;
    }
}