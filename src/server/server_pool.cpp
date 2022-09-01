#include <server/server_pool.h>
#include <fmt/chrono.h>
#include <enet/enet.h>
#include <utils/packet.h>
#include <utils/text.h>
#include <event/event_pool.h>
#include <player/player_pool.h>
#include <database/database.h>

namespace GTServer {
    ServerPool::ServerPool(std::shared_ptr<EventPool> events, std::shared_ptr<Database> database) :
        m_events{ events },
        m_database{ database } {
        fmt::print("Initializing ServerPool\n");
    }
    ServerPool::~ServerPool() {
        //TODO: delete servers
        enet_deinitialize();
    }

    bool ServerPool::initialize_enet() {
        if (enet_initialize() != 0) {
            fmt::print("failed to initialize enet service\n");
            return false;
        }
        return true;
    }
    void ServerPool::deinitialize_enet() {
        enet_deinitialize();
    }

    std::shared_ptr<Server> ServerPool::start_instance() {
        uint8_t instance_id = static_cast<uint8_t>(m_servers.size());
        auto server{ std::make_shared<Server>(instance_id, m_address, m_port++, m_max_peers) };
        if (!server->start()) {
            fmt::print("failed to start enet server -> {}:{}", server->get_address(), server->get_port());
            return nullptr;
        }
        fmt::print("starting instance_id: {}, {}:{} - {}\n", server->get_instance_id(), server->get_address(), server->get_port(), std::chrono::system_clock::now());
        m_servers.push_back(std::move(server));
        return server;
    }
    void ServerPool::stop_instance(std::shared_ptr<Server> server) {
        if (!server)
            return;

        fmt::print("shutting down instance_id: {} - {}\n", server->get_instance_id(), std::chrono::system_clock::now());
        if (auto it = std::find(m_servers.begin(), m_servers.end(), server); it != m_servers.end()) {
            server->stop();
            server.reset();
            m_servers.erase(it);
            return;
        }
        fmt::print("failed to shutdown instance_id: {}\n", server->get_instance_id());
    }

    void ServerPool::start_service() {
        if (m_running.load())
            return;
        if (m_servers.empty())
            this->start_instance();
        m_running.store(true);
        m_service = std::thread{ &ServerPool::service_poll, this };
        m_service.detach();
    }
    void ServerPool::stop_service() {
        if (!m_running.load())
            return;
        m_running.store(false);
    }
    void ServerPool::service_poll() {
        ENetEvent event{};
        while (m_running.load())  {
        for (auto& server : m_servers) {
            while (enet_host_check_events(server->get_host(), &event)) {
                if (!event.peer)
                    break;

                switch(event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    std::shared_ptr<Player> player{ server->get_player_pool()->new_player(event.peer) };
                    player->send_packet({ NET_MESSAGE_SERVER_HELLO }, sizeof(TankUpdatePacket));
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    if (!event.peer->data)
                        break;
                    std::uint32_t connect_id{};
                    std::memcpy(&connect_id, event.peer->data, sizeof(std::uint32_t));
                    std::free(event.peer->data);
                    event.peer->data = NULL;
                    
                    if (!server->get_player_pool()->get_player(connect_id))
                        return;
                    server->get_player_pool()->remove_player(connect_id);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    if (event.packet->dataLength < sizeof(TankUpdatePacket::m_type) + 1 || event.packet->dataLength > 0x200) {
                        enet_packet_destroy(event.packet);
                        break;
                    }
                    std::shared_ptr<Player> player{ server->get_player_pool()->get_player(event.peer->connectID) };
                    if (!player) {
                        enet_packet_destroy(event.packet);
                        break;
                    }
                    switch (*((int32_t*)event.packet->data)) {
                    case NET_MESSAGE_GENERIC_TEXT:
                    case NET_MESSAGE_GAME_MESSAGE: {
                        const auto& str = utils::get_generic_text(event.packet);
                        EventContext ctx { 
                            .m_player = player,
                            .m_events = this->get_events(),
                            .m_database = this->get_database(),
                            .m_server = server,
                            .m_servers = this,
                            .m_parser = TextScanner{ str }, 
                            .m_update_packet = nullptr 
                        };

                        std::string event_data = str.substr(0, str.find('|'));
                        if (!m_events->execute(EVENT_TYPE_GENERIC_TEXT, event_data, ctx))
                            break;
                        break;
                    }
                    case NET_MESSAGE_GAME_PACKET: {
                        if (event.packet->dataLength < sizeof(GameUpdatePacket) - 5)
                            return;
                        GameUpdatePacket* update_packet{ reinterpret_cast<GameUpdatePacket*>(event.packet->data + 4) };
                        EventContext ctx { 
                            .m_player = player,
                            .m_events = this->get_events(),
                            .m_database = this->get_database(),
                            .m_server = server,
                            .m_servers = this,
                            .m_parser = TextScanner{}, 
                            .m_update_packet = update_packet 
                        };

                        if (!m_events->execute(EVENT_TYPE_GAME_PACKET, "gup_" + std::to_string(update_packet->m_type), ctx)) {
                            player->send_log("unhandled EVENT_TYPE_GAME_PACKET -> `w{}`o, ptr: `w{}``", update_packet->m_type, fmt::ptr(update_packet));
                            break;
                        }
                        break;
                    }
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_NONE:
                default:
                    break;
                }
            }

            enet_host_service(server->get_host(), nullptr, 0);
        } }
    }
}