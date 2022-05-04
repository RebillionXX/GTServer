#include <server/server.h>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <player/player.h>
#include <proton/packet.h>

ENetServer::ENetServer(const uint8_t& instanceId, const std::string& address, const uint16_t& port, const size_t& max_peers) : 
    m_instanceId(instanceId), m_address(address), m_port(port), m_max_peers(max_peers) {
}
ENetServer::~ENetServer() {
    if(!this->stop())
        return;
    delete m_host;
}

std::pair<std::string, uint16_t> ENetServer::get_host() {
    return { m_address, m_port };
}
bool ENetServer::start() {
    ENetAddress address;
    enet_address_set_host(&address, m_address.c_str());
    address.port = m_port;

    m_host = enet_host_create(&address, m_max_peers, 2, 0, 0);
    if (!m_host)
        return false;
    
    m_host->checksum = enet_crc32;
	enet_host_compress_with_range_coder(m_host);
    fmt::print("starting instanceId: {}, {}:{} - {}\n", 1, m_address, m_port, std::chrono::system_clock::now());
    m_running.store(true);
    return true;
}
bool ENetServer::stop() {
    for (auto& peer : m_peers)
        enet_peer_disconnect_later(peer, 0U);
    m_running.store(false);
    return true;
}

void ENetServer::start_service() {
    m_service = std::thread{ &ENetServer::service, this };
    m_service.detach();
}
void ENetServer::service() {
    while (m_running.load()) {
        if (enet_host_service(m_host, &m_event, 1000) < 1)
            continue;
        switch(m_event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                NetAvatar* player = new NetAvatar(m_event.peer, this);
                fmt::print("Connection IP: {}\n", player->m_ip_address.data());
                break;
            }
            case ENET_EVENT_TYPE_NONE:
            default:
                break;
        }
    }
}