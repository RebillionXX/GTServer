#pragma once
#include <string>
#include <list>
#include <player/objects/roles.h>

namespace GTServer {
    class TankUpdatePacket;
    class PlayerTribute {
    public:
        PlayerTribute();
        ~PlayerTribute();

        void insert_epic_player(const ePlayerRole& role, const std::string& name);
        void remove_epic_player(const std::string& name);
        std::list<std::string> get_epic_player(const ePlayerRole& role);

        bool build();
        bool destroy();

        [[nodiscard]] uint32_t get_hash() const { return m_hash; }
        [[nodiscard]] std::pair<std::size_t, TankUpdatePacket*> get_packet() const { return { m_size, m_packet }; }
    private:      
        std::size_t m_size;
        char* m_data;

        uint32_t m_hash;
        TankUpdatePacket* m_packet;

        std::list<std::pair<ePlayerRole, std::string>> m_epic_players;
    };
}