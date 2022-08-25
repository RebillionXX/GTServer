#include <database/player_tribute.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <utils/packet.h>
#include <utils/binary_writer.h>
#include <proton/utils/misc_utils.h>
namespace GTServer {
    PlayerTribute::PlayerTribute() : m_epic_players{} {
        this->insert_epic_player(PLAYER_ROLE_DEVELOPER, "Rebillion");
    }
    PlayerTribute::~PlayerTribute() {
        this->destroy();
    }

    std::list<std::string> PlayerTribute::get_epic_player(const ePlayerRole& role) {
        std::list<std::string> ret{};
        for (const auto& it : m_epic_players) {
            if (role != it.first)
                continue;
            ret.push_back(it.second);
        }
        return ret;
    }
    void PlayerTribute::insert_epic_player(const ePlayerRole& role, const std::string& name) {
        m_epic_players.push_back({ role, name });
    }
    void PlayerTribute::remove_epic_player(const std::string& name) {
        //TODO: remove
    }
        
    bool PlayerTribute::build() {
        std::string epic_players{ fmt::format(
            "`oDeveloper: {}\n"
            "`oManager: {}\n"
            "`oAdministrator: {}\n"
            "`5Moderator: {}\n\n"
            "`2Copyright GTServer (c)``", 
            fmt::join(get_epic_player(PLAYER_ROLE_DEVELOPER), "; "),
            fmt::join(get_epic_player(PLAYER_ROLE_MANAGER), "; "),
            fmt::join(get_epic_player(PLAYER_ROLE_ADMINISTRATOR), "; "),
            fmt::join(get_epic_player(PLAYER_ROLE_MODERATOR), "; "))
        };
        std::string exceptional_mentors {
            "`2T`3E`4E`5T`2T`3E`4E`5T"
        };

        m_size = sizeof(uint32_t) + epic_players.length() + exceptional_mentors.length();
        m_data = static_cast<uint8_t*>(std::malloc(m_size));

        BinaryWriter buffer{ m_data };
        buffer.write<uint16_t>(static_cast<uint16_t>(epic_players.length()));
        buffer.write(epic_players.c_str(), epic_players.length());
        buffer.write<uint16_t>(static_cast<uint16_t>(exceptional_mentors.length()));
        buffer.write(exceptional_mentors.c_str(), exceptional_mentors.length());

        m_hash = proton::utils::hash(reinterpret_cast<char*>(m_data), static_cast<int32_t>(m_size));
        m_packet = static_cast<TankUpdatePacket*>(std::malloc(sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + m_size));
        std::memset(m_packet, 0, sizeof(TankUpdatePacket) + sizeof(GameUpdatePacket) + m_size);
        m_packet->type = NET_MESSAGE_GAME_PACKET;
        m_packet->data = static_cast<char*>(std::malloc(sizeof(GameUpdatePacket) + m_size));

        GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(m_packet->data);
        update_packet->type = NET_GAME_PACKET_SEND_PLAYER_TRIBUTE_DATA;
        update_packet->net_id = -1;
        update_packet->flags |= NET_GAME_PACKET_FLAGS_EXTENDED;
        update_packet->data_size = (uint32_t)m_size;
        std::memcpy(&update_packet->data, m_data, m_size);
        std::memcpy(&m_packet->data, update_packet, sizeof(GameUpdatePacket) + m_size);
        return true;
    }
    bool PlayerTribute::destroy() {
        m_epic_players.clear();

        std::free(m_data);
        std::free(m_packet);
        return true;
    }
}