#pragma once
#include <string>

namespace GTServer {
    class Color {
    public:
        Color(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a = 255) {
            m_color[0] = b;
            m_color[1] = g;
            m_color[2] = r;
            m_color[3] = a;
        }
        Color(const uint32_t& col) {
            m_color[0] = (col >> 24) & 0xFF;
            m_color[1] = (col >> 16) & 0xFF;
            m_color[2] = (col >> 8) & 0xFF;
            m_color[3] = (col) & 0xFF;
        }
        ~Color() = default;

        uint32_t get_uint() const {
            uint32_t result = 0;
            for (unsigned index = 0; index < sizeof(uint32_t); index++)
                result = (result << 8) + m_color[index];
            return result;
        }

        void set_red(const uint8_t& col) { m_color[2] = col; }
        [[nodiscard]] uint8_t get_red() const { return m_color[2]; }
        void set_green(const uint8_t& col) { m_color[1] = col; }
        [[nodiscard]] uint8_t get_green() const { return m_color[1]; }
        void set_blue(const uint8_t& col) { m_color[0] = col; }
        [[nodiscard]] uint8_t get_blue() const { return m_color[0]; }
        void set_alpha(const uint8_t& col) { m_color[3] = col; }
        [[nodiscard]] uint8_t get_alpha() const { return m_color[3]; }
    private:
        uint8_t m_color[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    };
}