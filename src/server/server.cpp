#include <server/server.h>
#include <fmt/core.h>
#include <fmt/chrono.h>

ENetServer::ENetServer(const uint32_t& address, const uint16_t& port, const size_t& max_peers) : 
    m_address(address), m_port(port), m_max_peers(max_peers) {
}
ENetServer::~ENetServer() {
    if(!this->stop())
        return;
    delete m_host;
}

std::pair<uint32_t, uint16_t> ENetServer::get_host() {
    return { m_address, m_port };
}
bool ENetServer::start() {
    enet_initialize();
    ENetAddress address;
    address.host = m_address;
    address.port = m_port;

    m_host = enet_host_create(&address, m_max_peers, 2, 0, 0);
    if (!m_host)
        return false;
    
    m_host->checksum = enet_crc32;
	enet_host_compress_with_range_coder(m_host);
    fmt::print("starting instanceId: {}, {}:{} - {}\n", 1, m_address, m_port, std::chrono::system_clock::now());
    return true;
}
bool ENetServer::stop() {
    for (auto& peer : m_peers)
        enet_peer_disconnect_later(peer, 0U);
    return true;
}
void ENetServer::start_service() {
    while(true);
}