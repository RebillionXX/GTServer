#pragma once
#include <string>

namespace GTServer {
    class World {
    public:
        explicit World(const std::string& name, const uint32_t& width = 100, const uint32_t& height = 60);
        ~World();

    private:
        uint32_t m_flags;

        std::string m_name;
        uint32_t m_width;
        uint32_t m_height;
    };
}