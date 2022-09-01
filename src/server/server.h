#pragma once
#include <atomic>
#include <string>
#include <thread>
#include <vector>
#include <enet/enet.h>

namespace GTServer {
    class PlayerPool;
    class WorldPool;
    class Server {
    public:
        Server(const uint8_t& instanceId, const std::string& address, const uint16_t& port, const size_t& max_peers);
        ~Server();
        
        bool start();
        bool stop();
    
        [[nodiscard]] uint8_t get_instance_id() const { return m_instance_id; }
        [[nodiscard]] std::string get_address() const { return m_address; }
        [[nodiscard]] uint16_t get_port() const { return m_port; }
        [[nodiscard]] ENetHost* get_host() const { return m_host; }

        std::shared_ptr<PlayerPool> get_player_pool() { return m_player_pool; }
        std::shared_ptr<WorldPool> get_world_pool() { return m_world_pool; }

    private:
        uint8_t m_instance_id;
        std::string m_address{ "0.0.0.0" };
        uint16_t m_port;

        ENetHost* m_host;
        size_t m_max_peers;

        std::shared_ptr<PlayerPool> m_player_pool;
        std::shared_ptr<WorldPool> m_world_pool;
    };
}