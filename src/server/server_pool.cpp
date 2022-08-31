#include <server/server_pool.h>
#include <event/event_pool.h>
#include <database/database.h>

namespace GTServer {
    ServerPool::ServerPool(std::shared_ptr<EventPool> events, std::shared_ptr<Database> database) :
        m_events{ events },
        m_database{ database } {
        fmt::print("Initializing ServerPool\n");
    }
    ServerPool::~ServerPool() {

    }
}