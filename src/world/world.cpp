#include <world/world.h>

namespace GTServer {
    World::World(const std::string& name, const uint32_t& width, const uint32_t& height) :
        m_flags(0),
        m_name{ std::move(name) },
        m_width(width),
        m_height(height) {

    }
    World::~World() {
        
    }
}