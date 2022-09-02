#pragma once
#include <algorithm>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include <config.h>
#include <server/server_pool.h>
#include <database/player_tribute.h>
#include <database/table/player_table.h>
#include <database/table/world_table.h>

namespace GTServer {
    class Database {
    public: 
        enum eDatabaseTable {
            DATABASE_PLAYER_TABLE,
            DATABASE_WORLD_TABLE
        };

    public:
        Database() = default;
        ~Database();
        
        bool connect();

        static sqlpp::mysql::connection* get_connection() { return get().m_connection; }
        static void* get_table(const eDatabaseTable& table) { return get().get_table__interface(table); }

    public:
        static Database& get() { static Database ret; return ret; }

    public:
        void* get_table__interface(const eDatabaseTable& table);

    private:
        sqlpp::mysql::connection* m_connection{ nullptr };
        PlayerTable* m_player_table{ nullptr };
        WorldTable* m_world_table{ nullptr };
    };
}