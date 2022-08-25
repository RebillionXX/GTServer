#pragma once
#include <string>
#include <fmt/core.h>
#include <utils/color.h>

namespace GTServer {
    class WorldMenu {
    public:
        WorldMenu() { m_result.clear(); }
		~WorldMenu() = default;

		[[nodiscard]] std::string get() const { return m_result; }

        WorldMenu* add_floater(const std::string& text, const int& player_count, const double& scale, const Color& color) {
            m_result.append(fmt::format("add_floater|{}|{}|{}|{}\n", text, player_count, scale, color.get_uint()));
            return this;
        }
        WorldMenu* add_button(const std::string& text,const std::string& name, const int& scale, const Color& color) {
            m_result.append(fmt::format("add_button|{}|{}|{}|{}\n", text, name, scale, color.get_uint()));
            return this;
        }
        WorldMenu* set_default(const std::string& name) {
            m_result.append(fmt::format("default|{}\n", name));
            return this;
        }
        WorldMenu* add_heading(const std::string& text) {
            m_result.append(fmt::format("add_heading|{}\n", text));
            return this;
        }
        WorldMenu* add_filter() {
            m_result.append("add_filter\n");
            return this;
        }
        WorldMenu* set_max_rows(const int& count) {
            m_result.append(fmt::format("set_max_rows|{}\n", count));
            return this;
        }
        WorldMenu* setup_simple_menu() {
            m_result.append("setup_simple_menu\n");
            return this;
        }
    private:
        std::string m_result;
    };
}