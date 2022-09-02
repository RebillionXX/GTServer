#pragma once
#include <atomic>
#include <string>
#include <vector>
#include <server/server.h>

namespace GTServer {
    class EventPool;
    class ServerPool {
    public:
        explicit ServerPool(std::shared_ptr<EventPool> events);
        ~ServerPool();

        bool initialize_enet();
        void deinitialize_enet();
        
        std::shared_ptr<Server> start_instance();
        void stop_instance(std::shared_ptr<Server> server);

        void start_service();
        void stop_service();
        void service_poll();
    public:
        void set_user_id(const int& uid) { user_id = uid; }
        [[nodiscard]] int get_user_id(bool increase = true) { return increase ? ++user_id : user_id; }

    public:
        [[nodiscard]] bool is_running() const { return m_running.load(); }
        [[nodiscard]] std::vector<std::shared_ptr<Server>> get_servers() { return m_servers; }
        [[nodiscard]] std::size_t get_active_servers() { return m_servers.size(); }

        std::shared_ptr<EventPool> get_events() const { return m_events; }
        
    private:
        std::string m_address{ "0.0.0.0" };
        uint16_t m_port{ 17091 };
        size_t m_max_peers{ 0xFF };
        int user_id{ 0 };

        std::atomic<bool> m_running{ false };
        std::thread m_service;
        
    private:
        std::shared_ptr<EventPool> m_events;
        std::vector<std::shared_ptr<Server>> m_servers{};
    };
}