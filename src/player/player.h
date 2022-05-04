#pragma once
#include <string>
#include <enet/enet.h>

class NetAvatar {
public:
    NetAvatar(ENetPeer* peer, ENetServer* server) 
    : m_peer(peer), m_server(server) {
        if(!m_peer)
            return;
        m_peer->data = this;

        m_ip_address.reserve(16);
        enet_address_get_host_ip(&m_peer->address, m_ip_address.data(), 16);
    }
public:
    std::string m_ip_address;
private:
    ENetPeer* m_peer;
    ENetServer* m_server;
};