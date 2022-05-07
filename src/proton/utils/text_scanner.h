#ifndef PROTON_UTILS__TEXT_SCANNER_H
#define PROTON_UTILS__TEXT_SCANNER_H
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <fmt/core.h>

namespace GTServer
{
    class text_scanner
    {
    public:
        text_scanner() = default;
		text_scanner(const std::vector<std::pair<std::string, std::string>>& data) {
			for(const auto& it : data)
				m_data.insert_or_assign(std::move(it.first), std::move(it.second));
		}
		~text_scanner() = default;

        bool parse(const std::string& data) {
			try {
				std::string::size_type key_pos = 0;
				std::string::size_type i = 0;
				std::string key, val;

				while (i != std::string::npos) {
					if (i + 2 >= data.size()) 
						break;
					key_pos = data.find('|', i + 2);
					if (key_pos == std::string::npos)
						break;
					key = data.substr((i == 0 || i == 1 ? i : i + 1), key_pos - i - (i == 0 || i == 1 ? 0 : 1));
					if (i + 1 >= data.size()) break;

					i = data.find('\n', i + 1);
					if (i == std::string::npos)
						val = data.substr(key_pos + 1);
					else
						val = data.substr(key_pos + 1, i - key_pos - 1);
					m_data.insert_or_assign(std::move(key), std::move(val));
				}
				return true;
			}
			catch (std::exception e) {
				return false;
			}
			catch (...) {
				return false;
			}
		}

		bool contain(const std::string& key) {
			return m_data.find(key) != m_data.end();
		}
        const std::string& get(const std::string& key) noexcept {
			if (this->contain(key))
            	return m_data[key];
        	return m_data.begin()->second;
		}
        const std::unordered_map<std::string, std::string>& get_data() {
			return m_data;
		}

		bool try_get(const std::string& key, std::string& value) noexcept {
			auto it = m_data.find(key);
			if (it == m_data.end())
				return false;
			value = it->second;
			return true;
		}
		bool try_get(const std::string& key, int& value) noexcept {
			auto it = m_data.find(key);
			if (it == m_data.end())
				return false;
			value = std::atoi(it->second.c_str());
			return true;
		}
		bool try_get(const std::string& key, bool& value) noexcept {
			auto it = m_data.find(key);
			if (it == m_data.end())
				return false;
			value = it->second == "1";
			return true;
		}
    private:
        std::unordered_map<std::string, std::string> m_data;
    };
}

#endif // PROTON_UTILS__TEXT_SCANNER_H