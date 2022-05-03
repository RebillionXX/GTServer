#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <enet/enet.h>

class ENetServer {
public:
    ENetServer(const uint32_t& address, const uint16_t& port, const size_t& max_peers);
    ~ENetServer();
    
    std::pair<uint32_t, uint16_t> get_host();
    bool start();
    bool stop();
    void start_service();
private:
    uint32_t m_address;
    uint16_t m_port;

    ENetHost* m_host;
    size_t m_max_peers;

    std::vector<ENetPeer*> m_peers{};
};