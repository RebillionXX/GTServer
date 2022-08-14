#pragma once
#include <memory>
#include <unordered_map>
#include <enet/enet.h>
#include <player/player.h>

namespace GTServer {
    class PlayerPool {
    public:
        PlayerPool() = default;
        ~PlayerPool() = default;

        std::shared_ptr<Player> new_player(ENetPeer* peer) {
            m_players[peer->connectID] = std::make_shared<Player>(peer);
            return m_players[peer->connectID];
        }
        void remove_player(uint32_t connect_id) {
            m_players[connect_id].reset();
            m_players.erase(connect_id);
        }

        std::shared_ptr<Player> get_player(uint32_t connect_id) {
            for (auto& player : m_players) {
                if (player.first == connect_id)
                    return player.second;
            }
            return nullptr;
        }
    public:
        [[nodiscard]] std::unordered_map<uint32_t, std::shared_ptr<Player>> const get_players() { return m_players; }
        [[nodiscard]] std::size_t get_player_count() const { return m_players.size(); }

    private:
        std::unordered_map<uint32_t, std::shared_ptr<Player>> m_players{};
    };
}