#pragma once
#include <string>
#include <fmt/core.h>

namespace GTServer
{
	class DialogBuilder {
	public:
		enum size_type {
			SMALL,
			BIG
		};
		enum direction {
			NONE,
			LEFT,
			RIGHT,
			STATIC_BLUE_FRAME
		};
	private:
		[[nodiscard]] std::string get_size(const int& size) const {
			switch (size) {
				case size_type::SMALL:
					return "small";
				default:
					return "big";
			}
		}
		[[nodiscard]] std::string get_direction(const int& direction) const {
            switch (direction) {
            case direction::LEFT:
                return "left";
            case direction::RIGHT:
                return "right";
            case direction::STATIC_BLUE_FRAME:
                return "staticBlueFrame";
            default:
                return "";
            }
            return "";
        }

	public:
		DialogBuilder() { m_result.clear(); }
		~DialogBuilder() {}

		[[nodiscard]] std::string get() const { return m_result; }
		void clear() {
            m_result.clear();
        }
		
		DialogBuilder* set_default_color(const char& color) {
            m_result.append(fmt::format("\nset_default_color|`{}", color));
            return this;
        }
		DialogBuilder* text_scaling_string(const std::string& scale){
            m_result.append(fmt::format("\ntext_scaling_string|{}", scale));
            return this;
        }

		DialogBuilder* embed_data(const std::string& name, const std::string& embed_value);
        template<typename T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
		DialogBuilder* embed_data(const std::string& name, const T& embed_value);
        template<typename T, typename std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
		DialogBuilder* embed_data(const std::string& name, const T& embed_value);

		DialogBuilder* end_dialog(const std::string& name, const std::string& cancel, const std::string& ok) {
			m_result.append(fmt::format("\nend_dialog|{}|{}|{}|", name, cancel, ok));
			return this;
		}

		DialogBuilder* add_custom(const std::string& value);
		DialogBuilder* add_custom_break();
		DialogBuilder* add_spacer(const size_type& label_size = SMALL) {
			m_result.append(fmt::format("\nadd_spacer|{}|", this->get_size(label_size)));
			return this;
		}
		DialogBuilder* set_custom_spacing(const int& x, const int& y);
		DialogBuilder* add_label(const std::string& label, const int& start_direction = LEFT, const size_type& label_size = SMALL);
		DialogBuilder* add_custom_label(const std::string& label, const std::string& target, const double& top, const double& left, const size_type& size = SMALL);
		DialogBuilder* add_textbox(const std::string& label) {
			m_result.append(fmt::format("\nadd_textbox|{}|", label));
        	return this;
		}
		DialogBuilder* add_smalltext(const std::string& label);
		DialogBuilder* add_text_input(const std::string& name, const std::string& label, const std::string& label_inside, const int& max_length) {
			m_result.append(fmt::format("\nadd_text_input|{}|{}|{}|{}|", name, label, label_inside, max_length));
        	return this;
		}
		DialogBuilder* add_text_input_password(const std::string& name, const std::string& label, const std::string& label_inside, const int& max_length) {
			m_result.append(fmt::format("\nadd_text_input_password|{}|{}|{}|{}|", name, label, label_inside, max_length));
        	return this;
		}
		DialogBuilder* add_text_box_input(const std::string& name, const std::string& label, std::string text_inside, const int& max_length, const int& lines);
		DialogBuilder* add_button(const std::string& name, const std::string& label, const std::string& btn_flag = "noflags");
		DialogBuilder* add_button_with_icon(const std::string& name, const std::string& label, const int& item_id, const int& start_direction = LEFT);
		DialogBuilder* add_custom_button(const std::string& name, const std::string& image, const int& image_x, const int& image_y, const double& width);
		DialogBuilder* add_label_with_icon(const std::string& label, const int& itemId, const int& start_direction = LEFT, const size_type& label_size = SMALL) {
			m_result.append(fmt::format("\nadd_label_with_icon|{}|{}|{}|{}|", this->get_size(label_size), label, this->get_direction(start_direction), itemId));
			return this;
		}
		DialogBuilder* add_label_with_icon_button(const std::string& name, const std::string& label, const int& itemId, const int& start_direction = LEFT, const size_type& label_size = SMALL);
		DialogBuilder* add_dual_layer_icon_label(const std::string& label, const std::pair<int, int>& icon, const double& size, const int& start_direction = LEFT, const size_type& label_size = SMALL);
		DialogBuilder* add_player_info(const std::string& name, const int& level, const int& exp, const int& math_exp);
		DialogBuilder* add_item_picker(const std::string& name, const std::string& label, const std::string& floating_text);
		DialogBuilder* add_player_picker(const std::string& name, const std::string& label);
		DialogBuilder* add_checkbox(const std::string& name, const std::string& label, const bool& active);
		DialogBuilder* add_checkicon(const std::string& name, const std::string& label, const int& id, const std::string& frame_message, const bool& enabled);
		DialogBuilder* add_quick_exit();
	private:
		std::string m_result{};
	};
}