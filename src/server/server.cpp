#include <server/server.h>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <events/event_manager.h>
#include <player/player_pool.h>
#include <proton/packet.h>
#include <proton/utils/text_scanner.h>
#include <utils/packet.h>

namespace GTServer {
    Server::Server(const uint8_t& instanceId, const std::string& address, const uint16_t& port, const size_t& max_peers) : 
        m_instance_id(instanceId), m_address(address), m_port(port), m_max_peers(max_peers),
        m_player_pool{ std::make_shared<PlayerPool>() } {
    }
    Server::~Server() {
        if(!this->stop())
            return;
        delete m_host;
    }
    void Server::set_component(event_manager* ev, database* db) {
        this->m_event_manager = ev;
        this->m_database = db;
    }

    std::pair<std::string, uint16_t> Server::get_host() {
        return { m_address, m_port };
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
        fmt::print("starting instance_id: {}, {}:{} - {}\n", 1, m_address, m_port, std::chrono::system_clock::now());
        m_running.store(true);
        return true;
    }
    bool Server::stop() {
        for (auto& pair : m_player_pool->get_players())
            pair.second->disconnect(0U);
        m_running.store(false);
        return true;
    }

    void Server::start_service() {
        m_service = std::thread{ &Server::service, this };
        m_service.detach();
    }
    void Server::service() {
        while (m_running.load()) {
            if (enet_host_service(m_host, &m_event, 1000) < 1)
                continue;
            switch(m_event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    this->on_connect(m_event.peer);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    this->on_disconnect(m_event.peer);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    if (!m_event.peer || !m_event.peer->data)
                        break;
                    if (m_event.packet->dataLength < sizeof(TankUpdatePacket::type) + 1 || m_event.packet->dataLength > 0x200)
                        break;
                    this->on_receive(m_event.peer, m_event.packet);
                    enet_packet_destroy(m_event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_NONE:
                default:
                    break;
            }
        }
    }

    void Server::on_connect(ENetPeer* peer) {
        std::shared_ptr<Player> player{ m_player_pool->new_player(peer) };
        player->send_packet({ NET_MESSAGE_SERVER_HELLO }, sizeof(TankUpdatePacket));
    }
    void Server::on_disconnect(ENetPeer* peer) {
        
    }
    void Server::on_receive(ENetPeer* peer, ENetPacket* packet) {
        std::shared_ptr<Player> player{ m_player_pool->get_player(peer->connectID) };
        if (!player) {
            player->send_log("Server requested you to re-logon.");
            player->disconnect(0U);
            return;
        }
        const auto& tank_packet = reinterpret_cast<TankUpdatePacket*>(m_event.packet->data);

        switch (tank_packet->type) {
            case NET_MESSAGE_GENERIC_TEXT:
            case NET_MESSAGE_GAME_MESSAGE: {
                const auto& str = utils::get_tank_update_data(m_event.packet);
                event_manager::context ctx{ 
                    player, 
                    this, 
                    m_event_manager, 
                    m_database, 
                    text_scanner{ str } 
                };
                
                std::string ev_function = str.substr(0, str.find('|'));
                if (!m_event_manager->call({ ev_function, event_manager::text_event::TEXT }, ctx))
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
    }
}