#include <unordered_map>
#include <string>
#include <server/server.h>

class server_pool {
public:
    server_pool() = default;
    ~server_pool() = default;

    std::unordered_map<uint8_t, ENetServer*> get_servers() {
        return m_servers;
    }
    ENetServer* start_instance() {
        auto server = new ENetServer(m_address, m_port++, m_max_peers);
        const uint8_t& instanceId = static_cast<uint8_t>(m_servers.size());
        m_servers.insert_or_assign(instanceId, server);
        return server;
    }
    void stop_instance(const uint8_t& instanceId) {
        fmt::print("shutting down instanceId: {} - {}\n", instanceId, std::chrono::system_clock::now());
        if(auto it = m_servers.find(instanceId); it != m_servers.end()) {
            it->second->stop();
            m_servers.erase(instanceId);
            return;
        }
        fmt::print("failed to shutdown instanceId: {}\n", instanceId);
        return;
    }
private:
    uint32_t m_address{ ENET_HOST_ANY };
    uint16_t m_port{ 1001 };
    size_t m_max_peers{ 0xFF };

    std::unordered_map<uint8_t, ENetServer*> m_servers{};
};