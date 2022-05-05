#include <server/server.h>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>
#include <events/event_manager.h>
#include <player/player.h>
#include <proton/packet.h>
#include <proton/utils/text_scanner.h>
#include <utils/packet.h>

namespace GTServer {
    ENetServer::ENetServer(const uint8_t& instanceId, const std::string& address, const uint16_t& port, const size_t& max_peers) : 
        m_instanceId(instanceId), m_address(address), m_port(port), m_max_peers(max_peers) {
    }
    ENetServer::~ENetServer() {
        if(!this->stop())
            return;
        delete m_host;
    }
    void ENetServer::set_event_manager(event_manager* ev) {
        this->m_event_manager = ev;
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
                    player->send({ NET_MESSAGE_SERVER_HELLO }, sizeof(TankUpdatePacket));
                    player->send_variant({ "OnConsoleMessage", fmt::format("Welcome to `w{}``!", "Buildo/GTserver")});
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                    break; //todo
                case ENET_EVENT_TYPE_RECEIVE: {
                    if(!m_event.peer || !m_event.peer->data)
                        return;
                    if(m_event.packet->dataLength < sizeof(TankUpdatePacket::type) + 1 || m_event.packet->dataLength > 0x200)
                        return;
                    NetAvatar* player = static_cast<NetAvatar*>(m_event.peer->data);
                    const auto& tank_packet = reinterpret_cast<TankUpdatePacket*>(m_event.packet->data);
                    
                    switch (tank_packet->type) {
                        case NET_MESSAGE_GENERIC_TEXT:
                        case NET_MESSAGE_GAME_MESSAGE: {
                            text_scanner text(utils::get_tank_update_data(m_event.packet));
                            if (!text.valid())
                                break;
                            std::string ev_function = text.get_all_raw().substr(0, text.get_all_raw().find('|'));
                            event_manager::context ctx{ player, this };
                            if (!m_event_manager->call(ev_function, ctx))
                                break;
                            break;
                        }
                        case NET_MESSAGE_GAME_PACKET: {
                            GameUpdatePacket* update_packet = reinterpret_cast<GameUpdatePacket*>(tank_packet->data);
                            if (!update_packet)
                                break;
                            break;
                        }
                    }
                    enet_packet_destroy(m_event.packet);
                }
                case ENET_EVENT_TYPE_NONE:
                default:
                    break;
            }
        }
    }
}