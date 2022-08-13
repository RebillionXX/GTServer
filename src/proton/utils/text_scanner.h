#ifndef PROTON_UTILS__TEXT_SCANNER_H
#define PROTON_UTILS__TEXT_SCANNER_H
#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <fmt/format.h>

namespace GTServer
{
    class text_scanner { //thanks to ztz who helped me on this
    public:
        text_scanner() : m_data() {}
        explicit text_scanner(const std::string& string) { 
            this->parse(string); 
        }
        explicit text_scanner(const std::vector<std::pair<std::string, std::string>>& data) {
            for (const auto& it : data)
                this->add(it.first, it.second);
        }
        ~text_scanner() = default;

        void parse(const std::string& string) {
            m_data = this->string_tokenize(string, "\n");
            for (auto &data : m_data) {
                std::replace(data.begin(), data.end(), '\r', '\0');
            }
        }
        static std::vector<std::string> string_tokenize(const std::string &string, const std::string &delimiter = "|") {
            std::vector<std::string> tokens{};
            for (const auto& word : std::views::split(string, delimiter))
                tokens.emplace_back(word.begin(), word.end());
            return tokens;
        }

        std::string get(const std::string &key, int index = 1, const std::string &token = "|", int key_index = 0)
        {
            if (m_data.empty())
                return "";

            for (auto &data : m_data) {
                if (data.empty())
                    continue;
                std::vector<std::string> tokenize = this->string_tokenize(data, token);
                if (tokenize[key_index] == key) {
                    if (index < 0 || index >= tokenize.size())
                        return "";
                    return tokenize[key_index + index];
                }
            }
            return "";
        }
        template<typename T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
        T get(const std::string &key, int index = 1, const std::string &token = "|") {
            return std::stoi(this->get(key, index, token));
        }
        template<typename T, typename std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        T get(const std::string &key, int index = 1, const std::string &token = "|") {
            if (std::is_same_v<T, double>)
                return std::stod(this->get(key, index, token));
            else if (std::is_same_v<T, long double>)
                return std::stold(this->get(key, index, token));
            return std::stof(this->get(key, index, token));
        }

		bool try_get(const std::string& key, std::string& value) noexcept {
			if (!this->contain(key))
				return false;
			value = this->get(key);
			return true;
		}
        template<typename T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
        bool try_get(const std::string& key, T &value) noexcept {
			if (!this->contain(key))
				return false;
			value = std::stoi(this->get(key));
			return true;
		}

        void add(const std::string &key, const std::string &value, const std::string &token = "|") {
            m_data.push_back(key + token + value);
        }
        template<typename T, typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, bool> = true>
        void add(const std::string &key, const T &value, const std::string &token = "|") {
            this->add(key, std::to_string(value), token);
        }
        void set(const std::string &key, const std::string &value, const std::string &token = "|") {
            if (m_data.empty())
                return;

            for (auto &data : m_data) {
                std::vector<std::string> tokenize = this->string_tokenize(data, token);
                if (tokenize[0] == key) {
                    data = std::string{ tokenize[0] };
                    data += token;
                    data += value;
                    break;
                }
            }
        }
        template<typename T, typename std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, bool> = true>
        void set(const std::string &key, const T &value, const std::string &token = "|") {
            this->set(key, std::to_string(value), token);
        }

		bool contain(const std::string& key) {
			return this->get(key) != "" ? true : false;
		}

        std::vector<std::string> get_all_array()
        {
            std::vector<std::string> ret{};
            for(int i = 0; i < m_data.size(); i++)
                ret.push_back(fmt::format("[{}]: {}", i, m_data[i]));

            return ret;
        }
        std::string get_all_raw()
        {
            std::string string{};
            for (int i = 0; i < m_data.size(); i++) {
                string += m_data.at(i);
                if (i + 1 >= m_data.size())
                    continue;

                if (!m_data.at(i + 1).empty())
                    string += '\n';
            }

            return string;
        }

        bool empty() { return m_data.empty(); }
        std::size_t size() { return m_data.size(); }

    private:
        std::vector<std::string> m_data;
    };
}

#endif // PROTON_UTILS__TEXT_SCANNER_H