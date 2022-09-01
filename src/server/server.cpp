#include <server/server.h>
#include <player/player_pool.h>
#include <world/world_pool.h>

namespace GTServer {
    Server::Server(const uint8_t& instanceId, const std::string& address, const uint16_t& port, const size_t& max_peers) : 
        m_instance_id(instanceId), m_address(address), m_port(port), m_max_peers(max_peers),
        m_player_pool{ std::make_shared<PlayerPool>() },
        m_world_pool{ std::make_shared<WorldPool>() } {
    }
    Server::~Server() {
        if(!this->stop())
            return;
        enet_host_destroy(this->get_host());
    }

    bool Server::start() {
        ENetAddress address;
        enet_address_set_host(&address, m_address.c_str());
        address.port = m_port;

        m_host = enet_host_create(&address, m_max_peers, 2, 0, 0);
        if (!m_host)
            return false;
        
        m_host->checksum = enet_crc32;
        enet_host_compress_with_range_coder(m_host);
        return true;
    }
    bool Server::stop() {
        for (auto& pair : m_player_pool->get_players())
            pair.second->disconnect(0U);
        return true;
    }
}