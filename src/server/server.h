#pragma once
#include <atomic>
#include <stdint.h>
#include <string>
#include <thread>
#include <vector>
#include <enet/enet.h>

namespace GTServer {
    class EventPool;
    class Database;
    class PlayerPool;
    class Server {
    public:
        Server(const uint8_t& instanceId, const std::string& address, const uint16_t& port, const size_t& max_peers);
        ~Server();
        void set_component(std::shared_ptr<EventPool> events, std::shared_ptr<Database> database);
        
        std::pair<std::string, uint16_t> get_host();
        bool start();
        bool stop();

        void start_service();
        void service();
    
    public:
        void on_connect(ENetPeer* peer);
        void on_disconnect(ENetPeer* peer);
        void on_receive(ENetPeer* peer, ENetPacket* packet);

    private:
        uint8_t m_instance_id;
        std::string m_address{ "0.0.0.0" };
        uint16_t m_port;

        ENetHost* m_host;
        ENetEvent m_event;
        size_t m_max_peers;
        std::atomic<bool> m_running{ false };
        std::thread m_service;

        std::shared_ptr<PlayerPool> m_player_pool;
    private:
        std::shared_ptr<EventPool> m_events;
        std::shared_ptr<Database> m_database;
    };
}