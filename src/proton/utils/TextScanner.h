//  ***************************************************************
//  TextScanner - Creation date: 06/09/2009
//  -------------------------------------------------------------
//  Robinson Technologies Copyright (C) 2009 - All Rights Reserved
//
//  ***************************************************************
//  Programmer(s):  Seth A. Robinson (seth@rtsoft.com)
//  ***************************************************************

#ifndef PROTON_UTILS__TEXT_SCANNER_H
#define PROTON_UTILS__TEXT_SCANNER_H
#include <algorithm>
#include <string>
#include <vector>

class rt_parser
{
public:
	rt_parser(const std::string &data) {
		m_data = this->tokenize_line(data, "\n");
		for (auto &s : m_data)
			std::replace(s.begin(), s.end(), '\t', '\0');
	}
	~rt_parser() = default;

	std::vector<std::string> tokenize_line(const std::string &string, const std::string &delimiter = "|") {
		std::vector<std::string> tokens;
		std::size_t last_pos = string.find_first_not_of(delimiter, 0);
		std::size_t pos = string.find_first_of(delimiter, last_pos);

		while (pos != std::string::npos || last_pos != std::string::npos) {
			tokens.push_back(string.substr(last_pos, pos - last_pos));
			last_pos = string.find_first_not_of(delimiter, pos);
			pos = string.find_first_of(delimiter, last_pos);
		}
		return tokens;
	}

	std::string get(const std::string &key, int index, const std::string &token = "|", int key_index = 0) {
		if (m_data.empty())
			return std::string{};
		for (auto &data : m_data) {
			if (data.empty())
				continue;
			const auto& tokenize = this->tokenize_line(data, token);
			if (tokenize[key_index] != key)
				continue;
			if (index < 0 || index >= tokenize.size())
				return std::string{};
			return tokenize[key_index + index];
		}
		return std::string{};
	}
	template<typename T, typename std::enable_if_t<std::is_integral_v<T>, bool> = true>
	T get(const std::string &key, int index, const std::string &token = "|") {
		return std::atoi(this->get(key, index, token));
	}
	template<typename T, typename std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	T get(const std::string &key, int index, const std::string &token = "|") {
		if (std::is_same_v<T, double>)
			return std::stod(this->get(key, index, token));
		else if (std::is_same_v<T, long double>)
			return std::stold(this->get(key, index, token));
		return std::stof(this->get(key, index, token));
	}

	std::string get_all_raw() {
		std::string ret{};
		for (auto index = 0; index < m_data.size(); index++) {
			ret.append(m_data[index]);
			if (index + 1 >= m_data.size())
				continue;
			if (!m_data[index + 1].empty())
				ret.append("\n");
		}
		return ret;
	}

	bool empty() {
		return m_data.empty();
	}
	int get_line_count() {
		return (int)m_data.size();
	}
private:
	std::vector<std::string> m_data;
};


#endif // PROTON_UTILS__TEXT_SCANNER_H