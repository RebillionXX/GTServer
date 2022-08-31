#pragma once
#include <unordered_map>
#include <string>
#include <server/server.h>
#include <fmt/chrono.h>

namespace GTServer {
    class EventPool;
    class Database;
    class ServerPool {
    public:
        explicit ServerPool(std::shared_ptr<EventPool> events, std::shared_ptr<Database> database);
        ~ServerPool();

        bool initialize_enet() {
            if (enet_initialize() != 0) {
                fmt::print("failed to initialize enet service\n");
                return false;
            }
            return true;
        }
        void deinitialize_enet() {
            enet_deinitialize();
        }
        
        std::shared_ptr<Server> start_instance() {
            uint8_t instance_id = static_cast<uint8_t>(m_servers.size());
            m_servers[instance_id] = std::make_shared<Server>(++instance_id, m_address, m_port++, m_max_peers);

            auto server{ m_servers[instance_id] };
            server->set_component(m_events, m_database);
            if (!server->start()) {
                fmt::print("failed to start enet server -> {}:{}", server->get_host().first, server->get_host().second);
                return nullptr;
            }
            fmt::print("starting instance_id: {}, {}:{} - {}\n", server->get_instance_id(), server->get_address(), server->get_port(), std::chrono::system_clock::now());
            return m_servers[instance_id];
        }
        void stop_instance(const uint8_t& instance_id) {
            fmt::print("shutting down instance_id: {} - {}\n", instance_id, std::chrono::system_clock::now());
            if(auto it = m_servers.find(instance_id); it != m_servers.end()) {
                it->second->stop();
                m_servers[instance_id].reset();
                m_servers.erase(instance_id);
                return;
            }
            fmt::print("failed to shutdown instance_id: {}\n", instance_id);
            return;
        }

    public:
        void set_user_id(const int& uid) { user_id = uid; }
        [[nodiscard]] int get_user_id(bool increase = true) { return increase ? ++user_id : user_id; }

    public:
        [[nodiscard]] std::unordered_map<uint8_t, std::shared_ptr<Server>> get_servers() { return m_servers; }
        [[nodiscard]] std::size_t get_active_servers() { return m_servers.size(); }

    private:
        std::string m_address{ "0.0.0.0" };
        uint16_t m_port{ 17091 };
        size_t m_max_peers{ 0xFF };
        int user_id{ 0 };

    private:
        std::shared_ptr<EventPool> m_events;
        std::shared_ptr<Database> m_database;
        std::unordered_map<uint8_t, std::shared_ptr<Server>> m_servers{};
    };
}